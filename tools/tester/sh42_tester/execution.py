"""Bac à sable isolé, exécution des deux shells, comparaison."""

from __future__ import annotations

import os
import pty
import re
import shutil
import signal as signalmod
import select
import subprocess
import tempfile
import threading
import time
import fcntl
import struct
import termios
from pathlib import Path

from .case import Case
from .config import FIXTURES, SH42, SIGNAL_ERRORS, VALGRIND_PATTERNS
from .result import Result

# arrêt coopératif (Ctrl+C), partagé entre workers
STOP = threading.Event()
RUNNING: set = set()
RUNNING_LOCK = threading.Lock()
_ANSI = re.compile(r"\033\[[0-9;?]*[ -/]*[@-~]")
_OSC = re.compile(r"\033\][^\a]*(?:\a|\033\\)")


class RunningPid:
    def __init__(self, pid: int):
        self.pid = pid

    def kill(self) -> None:
        try:
            os.killpg(self.pid, signalmod.SIGKILL)
        except (ProcessLookupError, PermissionError):
            pass


def build_sandbox(root: Path) -> None:
    """cwd isolé : outfiles/ vide + test_files/ (fixtures en symlink).

    invalid_permission : vrai fichier chmod 000 (échec d'écriture identique
    dans les deux shells).
    """
    (root / "outfiles").mkdir()
    tf = root / "test_files"
    tf.mkdir()
    if FIXTURES.is_dir():
        for entry in FIXTURES.iterdir():
            (tf / entry.name).symlink_to(entry.resolve())
    inv = tf / "invalid_permission"          # recréé chmod 000 à chaque fois
    inv.touch()
    inv.chmod(0o000)


def clean_stderr(raw: str) -> str:
    """Reproduit `sed 's/.*: *//'` ligne par ligne + strip des \\n finaux."""
    lines = [re.sub(r"^.*: *", "", ln) for ln in raw.split("\n")]
    return "\n".join(lines).rstrip("\n")


def _signum(name: str | None) -> int | None:
    """Nom de signal ("INT", "TERM"...) -> numéro, ou None."""
    if not name:
        return None
    return getattr(signalmod, f"SIG{name.upper()}", None)


def _norm_code(code: int | None) -> int:
    """Code Popen -> convention shell : tué par signal n => 128 + n."""
    if code is None:
        return 0
    return 128 - code if code < 0 else code


def _wait_status_code(status: int) -> int:
    """Status waitpid -> convention shell."""
    if os.WIFEXITED(status):
        return os.WEXITSTATUS(status)
    if os.WIFSIGNALED(status):
        return 128 + os.WTERMSIG(status)
    return 1


def clean_tty_stream(raw: str, steps) -> str:
    """Nettoie une capture de pseudo-terminal avant comparaison."""
    raw = _OSC.sub("", _ANSI.sub("", raw.replace("\r\n", "\n")
                                 .replace("\r", "\n")))
    sent = []
    for step in steps:
        if step.send:
            sent += [ln for ln in step.send.split("\n") if ln != ""]
    sent.append("exit $?")
    out = []
    for line in raw.split("\n"):
        stripped = line.strip()
        if not stripped:
            continue
        if "\a" in stripped:
            continue
        if stripped.startswith("bash: cannot set terminal process group"):
            continue
        if stripped == "bash: no job control in this shell":
            continue
        if stripped == "exit":
            continue
        if stripped.startswith("🌴Lapinou"):
            continue
        for s in sent:
            if stripped == s:
                stripped = ""
                break
            if stripped.endswith(s):
                stripped = stripped[:len(stripped) - len(s)].rstrip()
                if "🌴" in stripped:
                    stripped = stripped.split("🌴", 1)[0].rstrip()
                break
        if not stripped:
            continue
        if stripped.startswith("End of MiNIShell"):
            continue
        out.append(stripped)
    return "\n".join(out).rstrip("\n")


def _open_tty_pair() -> tuple[int, int]:
    master, slave = pty.openpty()
    fcntl.ioctl(slave, termios.TIOCSWINSZ, struct.pack("HHHH", 40, 200, 0, 0))
    return (master, slave)


def run_shell(argv: list[str], steps, cwd: Path, env: dict, timeout: float,
              valgrind_log: Path | None) -> tuple[str, str, int]:
    """Exécute une séquence de `steps` sur un shell ; renvoie (stdout, stderr, code)."""
    if STOP.is_set():
        return ("", "", 124)
    cmd = list(argv)
    if valgrind_log is not None:
        cmd = [ "valgrind", "--leak-check=full", "--error-exitcode=99", f"--log-file={valgrind_log}", ] + cmd
    needs_group = any(s.signal for s in steps)
    # chemin rapide (threadé, sûr pour les grosses sorties) : un seul send, pas de signal
    simple = (len(steps) == 1 and steps[0].signal is None
              and not steps[0].eof)
    run_env = dict(env)
    run_env["PWD"] = str(cwd)
    proc = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE, cwd=cwd, env=run_env,
                            start_new_session=needs_group)
    with RUNNING_LOCK:
        RUNNING.add(proc)
    try:
        if simple:
            out, err = proc.communicate(input=(steps[0].send or "").encode(),
                                        timeout=timeout)
        else:
            out, err = _run_steps(proc, steps, timeout)
        return (out.decode(errors="replace"),
                err.decode(errors="replace"),
                _norm_code(proc.returncode))
    except subprocess.TimeoutExpired:
        proc.kill()
        proc.communicate()
        return ("", "", 124)
    finally:
        with RUNNING_LOCK:
            RUNNING.discard(proc)


def run_shell_tty(argv: list[str], steps, cwd: Path, env: dict,
                  timeout: float) -> tuple[str, str, int]:
    """Exécute un shell avec stdin tty et stdout/stderr capturés séparément."""
    if STOP.is_set():
        return ("", "", 124)
    in_m, in_s = _open_tty_pair()
    out_r, out_w = os.pipe()
    err_r, err_w = os.pipe()
    run_env = dict(env)
    run_env["PWD"] = str(cwd)
    run_env.setdefault("TERM", "xterm")
    run_env["PS1"] = ""
    run_env["PS2"] = ""
    pid = os.fork()
    if pid == 0:
        try:
            os.setsid()
            fcntl.ioctl(in_s, termios.TIOCSCTTY, 0)
            os.chdir(cwd)
            # Python ignore SIGPIPE ; un vrai terminal le laisse à SIG_DFL.
            # Sans ce reset, le shell (et ses enfants, ex. `yes`) hérite du
            # SIG_IGN de Python et rapporte des « Broken pipe » fantômes.
            signalmod.signal(signalmod.SIGPIPE, signalmod.SIG_DFL)
            os.dup2(in_s, 0)
            os.dup2(out_w, 1)
            os.dup2(err_w, 2)
            for fd in (in_m, in_s, out_r, out_w, err_r, err_w):
                try:
                    os.close(fd)
                except OSError:
                    pass
            os.execvpe(argv[0], argv, run_env)
        except BaseException:
            os._exit(127)
    for fd in (in_s, out_w, err_w):
        os.close(fd)
    running = RunningPid(pid)
    with RUNNING_LOCK:
        RUNNING.add(running)
    out_chunks = []
    err_chunks = []
    exit_code = 124
    child_done = False
    deadline = time.monotonic() + timeout
    try:
        for step in steps:
            if STOP.is_set():
                break
            if step.send is not None:
                os.write(in_m, step.send.encode())
            if step.signal is not None:
                num = _signum(step.signal)
                if num is not None:
                    _wait_settled(pid)
                    try:
                        os.killpg(os.getpgid(pid), num)
                    except (ProcessLookupError, PermissionError):
                        pass
            if step.eof:
                os.write(in_m, b"\x04")
        os.write(in_m, b"exit $?\n")
        while time.monotonic() < deadline:
            fds = [fd for fd in (out_r, err_r) if fd >= 0]
            if not fds:
                if not child_done:
                    done, status = os.waitpid(pid, 0)
                    if done == pid:
                        exit_code = _wait_status_code(status)
                break
            ready, _, _ = select.select(fds, [], [], 0.02)
            for fd in ready:
                try:
                    data = os.read(fd, 4096)
                except OSError:
                    data = b""
                if data:
                    if fd == out_r:
                        out_chunks.append(data)
                    else:
                        err_chunks.append(data)
                else:
                    if fd == out_r:
                        os.close(out_r)
                        out_r = -1
                    else:
                        os.close(err_r)
                        err_r = -1
            if not child_done:
                done, status = os.waitpid(pid, os.WNOHANG)
                if done == pid:
                    child_done = True
                    exit_code = _wait_status_code(status)
        else:
            os.killpg(pid, signalmod.SIGKILL)
            os.waitpid(pid, 0)
            return ("", "", 124)
        out = b"".join(out_chunks).decode(errors="replace")
        err = b"".join(err_chunks).decode(errors="replace")
        return (clean_tty_stream(out, steps),
                clean_tty_stream(err, steps),
                exit_code)
    finally:
        for fd in (in_m, out_r, err_r):
            if fd < 0:
                continue
            try:
                os.close(fd)
            except OSError:
                pass
        with RUNNING_LOCK:
            RUNNING.discard(running)


def _run_steps(proc, steps, timeout: float):
    """Joue les steps dans l'ordre : écrit, signale (une fois quiescent), ferme."""
    for s in steps:
        if STOP.is_set():
            break
        if s.send is not None:
            try:
                proc.stdin.write(s.send.encode())
                proc.stdin.flush()
            except (BrokenPipeError, OSError):
                pass
        if s.signal is not None:
            num = _signum(s.signal)
            if num is not None:
                _wait_settled(proc.pid)      # sync sur l'état, pas sur un chrono
                try:
                    os.killpg(os.getpgid(proc.pid), num)
                except (ProcessLookupError, PermissionError):
                    pass
        if s.eof:
            _close_stdin(proc)
    _close_stdin(proc)                       # EOF final si stdin encore ouvert
    return proc.communicate(timeout=timeout)


def _close_stdin(proc) -> None:
    try:
        if proc.stdin and not proc.stdin.closed:
            proc.stdin.close()
    except (BrokenPipeError, OSError):
        pass


def _group_settled(pgid: int) -> bool:
    """Vrai si le groupe existe et qu'aucun de ses process n'est runnable ('R').

    Tous endormis => le shell est bloqué en lecture (heredoc/read) ou attend un
    enfant qui dort (sleep) : l'instant est reproductible, on peut signaler.
    """
    found = False
    for entry in os.listdir("/proc"):
        if not entry.isdigit():
            continue
        try:
            with open(f"/proc/{entry}/stat") as f:
                after = f.read().rsplit(") ", 1)[1].split()
            state, pgrp = after[0], int(after[2])   # champs 3 (state) et 5 (pgrp)
        except (OSError, IndexError, ValueError):
            continue
        if pgrp == pgid:
            found = True
            if state == "R":
                return False
    return found


def _wait_settled(pgid: int, timeout: float = 2.0, interval: float = 0.005) -> None:
    """Attend que le groupe soit quiescent (deux relevés stables), plafonné."""
    deadline = time.monotonic() + timeout
    stable = 0
    while time.monotonic() < deadline and not STOP.is_set():
        if _group_settled(pgid):
            stable += 1
            if stable >= 2:
                return
        else:
            stable = 0
        time.sleep(interval)


def outfiles_brief_diff(a: Path, b: Path) -> list[str]:
    """Différences (façon `diff --brief`) entre deux dossiers outfiles/."""
    def snapshot(d: Path) -> dict[str, bytes]:
        out: dict[str, bytes] = {}
        for f in sorted(d.rglob("*")):
            if f.is_file() and not f.is_symlink():
                out[str(f.relative_to(d))] = f.read_bytes()
        return out

    sa, sb = snapshot(a), snapshot(b)
    diffs = []
    for name in sorted(set(sa) | set(sb)):
        if name not in sa:
            diffs.append(f"Only in bash: {name}")
        elif name not in sb:
            diffs.append(f"Only in 42_sh: {name}")
        elif sa[name] != sb[name]:
            diffs.append(f"Files differ: {name}")
    return diffs


def classify_valgrind(code: int, log: Path | None) -> str:
    """Traduit un code de sortie / log valgrind en symboles d'erreur."""
    symbols = []
    if code == 99 and log is not None and log.exists():
        text = log.read_text(errors="replace").lower()
        for needles, _msg, sym in VALGRIND_PATTERNS:
            if any(n.lower() in text for n in needles):
                symbols.append(sym)
    elif code in SIGNAL_ERRORS:
        symbols.append(SIGNAL_ERRORS[code][1])
    elif 129 <= code <= 192:
        symbols.append("?")
    return "".join(symbols)


def run_one(case: Case, args, base_env: dict) -> Result:
    """Un cas : deux bacs à sable (42_sh, bash), puis comparaison."""
    res = Result(index=case.index, cmd=case.label)
    res.pipe_enabled = case.mode in ("dual", "pipe")
    res.tty_enabled = case.mode in ("dual", "tty")
    if case.env:
        base_env = {**base_env, **case.env}
    with tempfile.TemporaryDirectory(prefix="sh42_test_",
                                     dir=args.tmpdir) as tmp:
        tmp = Path(tmp)
        run_dir = tmp / "run"
        sh42_outfiles = tmp / "sh42_outfiles"
        tty_sh42_outfiles = tmp / "tty_sh42_outfiles"

        vlog = (tmp / "valgrind.log") if args.valgrind else None
        run_dir.mkdir()
        if res.pipe_enabled:
            build_sandbox(run_dir)
            sh42_out, sh42_err, res.sh42_code = run_shell(
                [str(SH42)], case.steps, run_dir, base_env, args.timeout, vlog)
            shutil.copytree(run_dir / "outfiles", sh42_outfiles)

            shutil.rmtree(run_dir)
            run_dir.mkdir()
            build_sandbox(run_dir)
            bash_out, bash_err, res.bash_code = run_shell(
                ["bash", "--posix", "--noprofile", "--norc"],
                case.steps, run_dir, base_env, args.timeout, None)

            res.sh42_out = sh42_out.rstrip("\n")
            res.bash_out = bash_out.rstrip("\n")
            res.sh42_err = clean_stderr(sh42_err)
            res.bash_err = clean_stderr(bash_err)
            res.outfiles_diff = outfiles_brief_diff(
                sh42_outfiles, run_dir / "outfiles")
            res.is_segfault = res.sh42_code == 139
            if args.valgrind and res.sh42_code != res.bash_code:
                res.critical = classify_valgrind(res.sh42_code, vlog)

        if res.tty_enabled:
            shutil.rmtree(run_dir)
            run_dir.mkdir()
            build_sandbox(run_dir)
            tty_sh42_out, tty_sh42_err, res.tty_sh42_code = run_shell_tty(
                [str(SH42)], case.steps, run_dir, base_env, args.timeout)
            shutil.copytree(run_dir / "outfiles", tty_sh42_outfiles)

            shutil.rmtree(run_dir)
            run_dir.mkdir()
            build_sandbox(run_dir)
            tty_bash_out, tty_bash_err, res.tty_bash_code = run_shell_tty(
                ["bash", "--posix", "--noprofile", "--norc", "-i"],
                case.steps, run_dir, base_env, args.timeout)

            res.tty_sh42_out = tty_sh42_out
            res.tty_bash_out = tty_bash_out
            res.tty_sh42_err = clean_stderr(tty_sh42_err)
            res.tty_bash_err = clean_stderr(tty_bash_err)
            res.tty_outfiles_diff = outfiles_brief_diff(
                tty_sh42_outfiles, run_dir / "outfiles")

    if res.pipe_enabled:
        res.ok_output = res.sh42_out == res.bash_out
        res.ok_code = res.sh42_code == res.bash_code
        res.ok_diff = not res.outfiles_diff
        res.ok_error = res.sh42_err == res.bash_err
        pipe_ok = res.ok_output and res.ok_code and res.ok_diff
    else:
        res.ok_output = res.ok_code = res.ok_diff = res.ok_error = True
        pipe_ok = True
    if res.tty_enabled:
        res.tty_ok_output = res.tty_sh42_out == res.tty_bash_out
        res.tty_ok_code = res.tty_sh42_code == res.tty_bash_code
        res.tty_ok_diff = not res.tty_outfiles_diff
        res.tty_ok_error = res.tty_sh42_err == res.tty_bash_err
        res.tty_strict_ok = (res.tty_ok_output and res.tty_ok_code
                             and res.tty_ok_diff and res.tty_ok_error)
    else:
        res.tty_ok_output = True
        res.tty_ok_code = True
        res.tty_ok_diff = True
        res.tty_ok_error = True
        res.tty_strict_ok = True
    res.strict_ok = pipe_ok and res.tty_strict_ok
    return res

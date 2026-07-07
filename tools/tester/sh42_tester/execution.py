"""Bac à sable isolé, exécution des deux shells, comparaison."""

from __future__ import annotations

import os
import re
import shutil
import signal as signalmod
import subprocess
import tempfile
import threading
import time
from pathlib import Path

from .case import Case
from .config import FIXTURES, SH42, SIGNAL_ERRORS, VALGRIND_PATTERNS
from .result import Result

# arrêt coopératif (Ctrl+C), partagé entre workers
STOP = threading.Event()
RUNNING: set = set()
RUNNING_LOCK = threading.Lock()


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
    if case.env:
        base_env = {**base_env, **case.env}
    with tempfile.TemporaryDirectory(prefix="sh42_test_",
                                     dir=args.tmpdir) as tmp:
        tmp = Path(tmp)
        run_dir = tmp / "run"
        sh42_outfiles = tmp / "sh42_outfiles"

        vlog = (tmp / "valgrind.log") if args.valgrind else None
        run_dir.mkdir()
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

    res.ok_output = res.sh42_out == res.bash_out
    res.ok_code = res.sh42_code == res.bash_code
    res.ok_diff = not res.outfiles_diff
    res.ok_error = res.sh42_err == res.bash_err
    res.strict_ok = res.ok_output and res.ok_code and res.ok_diff
    return res

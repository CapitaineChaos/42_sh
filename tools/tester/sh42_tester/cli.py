"""Parsing des arguments et orchestration : run_suite, report, main."""

from __future__ import annotations

import argparse
import os
import sys
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from .case import Case
from .config import DEFAULT_TIMEOUT, FAILURES_FILE
from .discovery import case_label, categories, discover, load_cases
from .environment import build_binary, make_env, pick_locale, preflight
from .execution import RUNNING, RUNNING_LOCK, STOP, run_one
from .ui import (C, _visible_len, banner, format_failure, print_icons,
                 prompt_retry, set_title)


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(description="Testeur différentiel 42_sh/bash")
    cats = "/".join(categories()) or "aucune"
    p.add_argument("filters", nargs="*", help=f"catégorie ({cats}) ou fichier .tests/.toml")
    p.add_argument("-v", "--valgrind", action="store_true", help="exécuter 42_sh sous valgrind")
    p.add_argument("-i", "--env-i", action="store_true", help="environnement minimal (comme env -i)")
    p.add_argument("-j", "--jobs", type=int, default=os.cpu_count() or 4, help="nombre de workers parallèles (défaut : nb cœurs)")
    p.add_argument("-t", "--timeout", type=float, default=DEFAULT_TIMEOUT, help=f"timeout par test en s (défaut : {DEFAULT_TIMEOUT})")
    mode = p.add_mutually_exclusive_group()
    mode.add_argument("-n", "--non-interactive-only", action="store_const",
                      const="pipe", dest="mode_filter",
                      help="exécuter seulement le mode non-interactif (stdin pipe)")
    mode.add_argument("-I", "--interactive-only", action="store_const",
                      const="tty", dest="mode_filter",
                      help="exécuter seulement le mode interactif (TTY)")
    p.add_argument("--no-build", action="store_true", help="ne pas recompiler")
    p.add_argument("--skip-preflight", action="store_true", help="ne pas sonder stdin/stdout avant la suite")
    p.add_argument("--tmpdir", default="/dev/shm" if Path("/dev/shm").is_dir() else None, help="racine des bacs à sable (défaut : /dev/shm)")
    return p.parse_args()


def apply_mode_filter(case: Case, mode_filter: str | None) -> Case | None:
    if mode_filter is None:
        return case
    if case.mode != "dual" and case.mode != mode_filter:
        return None
    case.mode = mode_filter
    return case


def run_suite(cases: list[Case], args, base_env: dict):
    """Exécute les cas en parallèle.

    Renvoie (totals, segfaults, failures, failed, interrupted) ;
    failed (cas non strict_ok) sert aux relances ciblées.
    """
    totals = dict(n=0, strict=0, out=0, code=0, diff=0, err=0,
                  pipe_n=0, tty_n=0,
                  tty_out=0, tty_code=0, tty_diff=0, tty_err=0,
                  crit=0, segv=0)
    segfaults: list[str] = []
    failures: list[str] = []
    failed: list[Case] = []

    total = len(cases)
    set_title(f"tester 0/{total}")
    pool = ThreadPoolExecutor(max_workers=args.jobs)
    futures = {i: pool.submit(run_one, c, args, base_env)
               for i, c in enumerate(cases)}
    interrupted = False
    try:
        current_file = None
        for i, c in enumerate(cases):
            if c.source != current_file:
                current_file = c.source
                print(banner(case_label(c.source)) + "\n")
            r = futures[i].result()
            totals["n"] += 1
            if r.pipe_enabled:
                totals["pipe_n"] += 1
                totals["out"] += r.ok_output
                totals["code"] += r.ok_code
                totals["diff"] += r.ok_diff
                totals["err"] += r.ok_error
            if r.tty_enabled:
                totals["tty_n"] += 1
                totals["tty_out"] += r.tty_ok_output
                totals["tty_code"] += r.tty_ok_code
                totals["tty_diff"] += r.tty_ok_diff
                totals["tty_err"] += r.tty_ok_error
            totals["strict"] += r.strict_ok
            if r.critical:
                totals["crit"] += 1
            if r.is_segfault:
                totals["segv"] += 1
                segfaults.append(r.cmd)
            if not r.strict_ok:
                failures.append(format_failure(r, case_label(c.source)))
                failed.append(c)
            set_title(f"tester {totals['n']}/{total} · {totals['strict']} ok")
            print_icons(r)
    except KeyboardInterrupt:
        interrupted = True
        STOP.set()
        with RUNNING_LOCK:
            for p in list(RUNNING):
                p.kill()
        pool.shutdown(wait=False, cancel_futures=True)
        print(f"\n{C.YELLOW}⏹  Interrompu (Ctrl+C) : Arrêt après "
              f"{totals['n']} test(s).{C.RESET}")
    else:
        pool.shutdown(wait=True)

    return totals, segfaults, failures, failed, interrupted


def view_log() -> None:
    if FAILURES_FILE.exists():
        print("\n" + FAILURES_FILE.read_text())
    else:
        print(f"{C.YELLOW}Aucun rapport.{C.RESET}")


def write_failures_file(t: dict, failures: list[str]) -> None:
    n = t["n"]
    head = [
        "#" * 72,
        f"# Rapport d'échecs : {len(failures)} test(s) en échec sur {n}",
        f"# strict {t['strict']}/{n} | stdout {t['out']}/{t['pipe_n']} | "
        f"exit {t['code']}/{t['pipe_n']} | outfiles {t['diff']}/{t['pipe_n']} | "
        f"stderr {t['err']}/{t['pipe_n']}",
        f"# tty stdout {t['tty_out']}/{t['tty_n']} | "
        f"tty exit {t['tty_code']}/{t['tty_n']} | "
        f"tty outfiles {t['tty_diff']}/{t['tty_n']} | "
        f"tty stderr {t['tty_err']}/{t['tty_n']}",
        "#" * 72, "",
    ]
    body = "\n\n".join(failures)
    FAILURES_FILE.write_text("\n".join(head) + "\n" + body + "\n")


def report(t: dict, segfaults: list[str], failures: list[str]) -> int:
    n = t["n"]
    print()

    def cell(val, total):
        mark = f"{C.GREEN}OK{C.RESET}" if val == total else f"{C.RED}KO{C.RESET}"
        return f"{mark} {val}/{total}"

    rows = [
        ("O", t["out"], t["pipe_n"], t["tty_out"], t["tty_n"]),
        ("C", t["code"], t["pipe_n"], t["tty_code"], t["tty_n"]),
        ("D", t["diff"], t["pipe_n"], t["tty_diff"], t["tty_n"]),
        ("E", t["err"], t["pipe_n"], t["tty_err"], t["tty_n"]),
    ]
    data = [(lbl, cell(pv, pt), cell(tv, tt)) for lbl, pv, pt, tv, tt in rows]
    lw = 3
    cw = max([_visible_len("pipe"), _visible_len("tty")]
             + [_visible_len(c) for _, c1, c2 in data for c in (c1, c2)]) + 2

    def padc(s, w):
        pad = w - _visible_len(s)
        left = pad // 2
        return " " * left + s + " " * (pad - left)

    def line(lft, mid, rgt):
        return lft + "─" * lw + mid + "─" * cw + mid + "─" * cw + rgt

    def trow(a, b, c):
        return f"│{padc(a, lw)}│{padc(b, cw)}│{padc(c, cw)}│"

    print(line("┌", "┬", "┐"))
    print(f"{C.BOLD}{trow('', 'pipe', 'tty')}{C.RESET}")
    print(line("├", "┼", "┤"))
    for lbl, c1, c2 in data:
        print(trow(f"{C.BOLD}{lbl}{C.RESET}", c1, c2))
    print(line("└", "┴", "┘"))
    if t["crit"]:
        print(f"{C.PURPLE}{t['crit']} erreur(s) critique(s){C.RESET}")
    else:
        print(f"{C.GREEN}Aucune erreur critique{C.RESET}")

    if segfaults:
        print(f"\n{C.RED}{C.BOLD}{len(segfaults)} SEGFAULT(s) (code 139){C.RESET}")

    if failures:
        write_failures_file(t, failures)
        print(f"\n{C.RED}{C.BOLD}{len(failures)} échec(s){C.RESET}"
              f"{C.RED} détaillé(s) dans le rapport :{C.RESET}")
        print(f"{C.BOLD}  cat {FAILURES_FILE}{C.RESET}")
    else:
        FAILURES_FILE.unlink(missing_ok=True)
        print(f"\n{C.GREEN}{C.BOLD}Aucun échec strict.{C.RESET}")

    return 0 if t["strict"] == n else 1


def main() -> int:
    args = parse_args()
    if args.valgrind:
        args.jobs = min(args.jobs, 4)      # valgrind coûteux : on plafonne
    if build_binary(args.no_build):
        time.sleep(2)                      # laisse voir le bilan de compilation

    files = discover(args.filters)
    if not files:
        sys.exit(f"{C.RED}Aucun fichier de test trouvé.{C.RESET}")

    locale = pick_locale()
    base_env = make_env(args.env_i, locale)
    if not args.skip_preflight:
        preflight(base_env)

    print(f"{C.GREEN}tester 42_sh · {args.jobs} workers · "
          f"locale {locale}{C.RESET}\n")
    print(f"{C.BLUE}      OCDE  OCDE  pipe tty · "
          f"O output · C code · D outfiles · E stderr{C.RESET}")

    cases: list[Case] = []
    idx = 0
    for f in files:
        for c in load_cases(f):
            c = apply_mode_filter(c, args.mode_filter)
            if c is None:
                continue
            idx += 1
            c.index = idx
            cases.append(c)
    if not cases:
        sys.exit(f"{C.RED}Aucun cas compatible avec le mode demandé.{C.RESET}")

    rc = 0
    run_no = 0
    while True:
        run_no += 1
        if run_no > 1:
            print("\n" + banner(f"RÉEXÉCUTION {run_no - 1} · "
                                 f"{len(cases)} cas en échec") + "\n")
        totals, segfaults, failures, failed, interrupted = \
            run_suite(cases, args, base_env)
        rc = report(totals, segfaults, failures)
        ok = not interrupted and totals["strict"] == totals["n"]
        set_title(f"tester {'OK' if ok else 'KO'} {totals['strict']}/{totals['n']}")

        if interrupted:
            return 130
        if not failed:
            return rc
        while True:
            action = prompt_retry(len(failed))
            if action == "v":
                view_log()
                continue
            break
        if action != "r":
            return rc
        cases = failed

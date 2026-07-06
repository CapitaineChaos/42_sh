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
from .ui import (C, banner, format_failure, print_icons,
                 prompt_retry, set_title)


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(description="Testeur différentiel 42_sh/bash")
    cats = "/".join(categories()) or "aucune"
    p.add_argument("filters", nargs="*", help=f"catégorie ({cats}) ou fichier .tests/.toml")
    p.add_argument("-v", "--valgrind", action="store_true", help="exécuter 42_sh sous valgrind")
    p.add_argument("-i", "--env-i", action="store_true", help="environnement minimal (comme env -i)")
    p.add_argument("-j", "--jobs", type=int, default=os.cpu_count() or 4, help="nombre de workers parallèles (défaut : nb cœurs)")
    p.add_argument("-t", "--timeout", type=float, default=DEFAULT_TIMEOUT, help=f"timeout par test en s (défaut : {DEFAULT_TIMEOUT})")
    p.add_argument("--no-build", action="store_true", help="ne pas recompiler")
    p.add_argument("--skip-preflight", action="store_true", help="ne pas sonder stdin/stdout avant la suite")
    p.add_argument("--tmpdir", default="/dev/shm" if Path("/dev/shm").is_dir() else None, help="racine des bacs à sable (défaut : /dev/shm)")
    return p.parse_args()


def run_suite(cases: list[Case], args, base_env: dict):
    """Exécute les cas en parallèle.

    Renvoie (totals, segfaults, failures, failed, interrupted) ;
    failed (cas non strict_ok) sert aux relances ciblées.
    """
    totals = dict(n=0, strict=0, out=0, code=0, diff=0, err=0, crit=0, segv=0)
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
            totals["out"] += r.ok_output
            totals["code"] += r.ok_code
            totals["diff"] += r.ok_diff
            totals["err"] += r.ok_error
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
        f"# strict {t['strict']}/{n} | stdout {t['out']}/{n} | "
        f"exit {t['code']}/{n} | outfiles {t['diff']}/{n} | stderr {t['err']}/{n}",
        "#" * 72, "",
    ]
    body = "\n\n".join(failures)
    FAILURES_FILE.write_text("\n".join(head) + "\n" + body + "\n")


def report(t: dict, segfaults: list[str], failures: list[str]) -> int:
    n = t["n"]
    print()

    def line(label, val):
        mark = f"{C.GREEN}OK{C.RESET}" if val == n else f"{C.RED}KO{C.RESET}"
        print(f"{mark}  {val}/{n} ({label})")
    line("outputs", t["out"])
    line("exit codes", t["code"])
    line("outfiles", t["diff"])
    line("errors", t["err"])
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
    print(f"{C.BLUE}      O C D E  "
          f"(Output | exit Code | Diff outfiles | Error msg){C.RESET}")

    cases: list[Case] = []
    idx = 0
    for f in files:
        for c in load_cases(f):
            idx += 1
            c.index = idx
            cases.append(c)

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

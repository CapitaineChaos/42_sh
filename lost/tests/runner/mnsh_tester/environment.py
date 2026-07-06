"""Préparation de l'environnement : build du binaire, locale, env, préflight."""

from __future__ import annotations

import os
import shlex
import shutil
import subprocess
import sys
from pathlib import Path

from .config import (LOCALE_CANDIDATES, MINISHELL, MINISHELL_SRC, PROJECT,
                     TEST_MAKE_ARGS)
from .ui import C


def accepts_pipes(binary: Path) -> bool:
    """Le binaire lit-il une commande sur stdin non-tty ?"""
    try:
        p = subprocess.run([str(binary)], input=b"echo __probe__\n", stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=5)
        return b"__probe__" in p.stdout
    except Exception:
        return False


def build_binary(no_build: bool) -> bool:
    """Compile si besoin. Renvoie True si une compilation a eu lieu."""
    if no_build:
        if MINISHELL.exists() and accepts_pipes(MINISHELL):
            return False
        if MINISHELL.exists():
            print(f"{C.YELLOW}Binaire en cache refuse l'entrée pipée ; "
                  f"rebuild (DLVL=0)...{C.RESET}")
    print(f"{C.CYAN}make {' '.join(TEST_MAKE_ARGS)}{C.RESET}")
    # clear/tput dans le Makefile plantent sans TERM
    build_env = {**os.environ, "TERM": os.environ.get("TERM") or "xterm"}
    r = subprocess.run(["make", "-C", str(PROJECT), *TEST_MAKE_ARGS], env=build_env)
    if r.returncode != 0:
        sys.exit(f"{C.RED}Erreur de compilation.{C.RESET}")
    if not MINISHELL_SRC.exists():
        sys.exit(f"{C.RED}Binaire introuvable : {MINISHELL_SRC}{C.RESET}")
    shutil.copy2(MINISHELL_SRC, MINISHELL)
    MINISHELL.chmod(0o755)
    MINISHELL_SRC.unlink(missing_ok=True)          # seule la copie locale reste
    print(f"{C.GREEN}Compilation réussie (minishell_test, DLVL=0).{C.RESET}")
    if not accepts_pipes(MINISHELL):
        sys.exit(f"{C.RED}minishell refuse toujours l'entrée non-interactive.\n"
                 f"-> vérifie all_config.h (DLVL) / interactive_shell_only.{C.RESET}")
    return True


def pick_locale() -> str:
    """Première locale disponible parmi LOCALE_CANDIDATES (C garanti en dernier)."""
    try:
        avail = subprocess.run(["locale", "-a"], stdout=subprocess.PIPE,
                               stderr=subprocess.DEVNULL, text=True).stdout.split()
    except Exception:
        avail = []
    low = {a.lower() for a in avail}
    for cand in LOCALE_CANDIDATES:
        if cand == "C" or cand.lower() in low:
            return cand
    return "C"


def make_env(env_i: bool, locale: str) -> dict:
    loc_env = {"LANG": locale, "LC_ALL": locale, "LANGUAGE": ""}
    if env_i:
        env = dict(loc_env)
        env["PATH"] = "/usr/bin:/bin:/usr/sbin:/sbin"
        return env
    env = dict(os.environ)
    env.update(loc_env)
    return env


def preflight(base_env: dict) -> None:
    """Abandonne si minishell ne tourne pas en non-interactif (pipe in/out)."""
    checks: list[tuple[str, bool]] = []

    # pipe-in + capture stdout
    checks.append(("stdin pipe-in + stdout capturable", accepts_pipes(MINISHELL)))

    # pipe-out : minishell | process
    ok_out = False
    try:
        p = subprocess.run(
            f"printf 'echo __pf__\\n' | {shlex.quote(str(MINISHELL))} | cat",
            shell=True, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL,
            timeout=5, env=base_env)
        ok_out = b"__pf__" in p.stdout
    except Exception:
        ok_out = False
    checks.append(("pipe-out (minishell | process)", ok_out))

    print(f"{C.CYAN}Préflight :{C.RESET}")
    for label, ok in checks:
        mark = f"{C.GREEN}OK{C.RESET}" if ok else f"{C.RED}KO{C.RESET}"
        print(f"  [{mark}] {label}")
    if not all(ok for _, ok in checks):
        sys.exit(f"{C.RED}Préflight échoué : minishell ne peut pas être testé "
                 f"en non-interactif.\n-> on n'exécute pas la suite "
                 f"(relance avec --skip-preflight pour forcer).{C.RESET}")

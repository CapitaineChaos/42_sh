"""Chemins et constantes partagés par tout le testeur."""

from __future__ import annotations

from pathlib import Path

HERE = Path(__file__).resolve().parent.parent   # tools/tester
PROJECT = HERE.parent.parent / "42_sh"          # racine du projet 42_sh
SH42_SRC = PROJECT / "42_sh_test"               # build normal (pipe autorisé)
SH42 = HERE / "42_sh"                           # copie locale
FIXTURES = HERE / "fixtures"
CASES = HERE / "cases"
FAILURES_FILE = HERE / "failures.log"

TEST_MAKE_ARGS = ["NAME=42_sh_test", "OBJ_DIR=obj_test", "CFLAGS=-Wall -Wextra -Werror -MMD -MP"]

# anglophones (stderr bash aligné), préférées -> repli ; pick_locale prend la 1re dispo
LOCALE_CANDIDATES = ("en_US.UTF-8", "en_US.utf8", "C.UTF-8", "C.utf8", "C")

DEFAULT_TIMEOUT = 10.0                   # s / test

# Codes de sortie -> (message, symbole)
SIGNAL_ERRORS = {
    124: ("TIMEOUT", "T"),
    126: ("NOT EXECUTABLE", "X"),
    127: ("FILE NOT EXISTS", "N"),
    130: ("INTERRUPTED (SIGINT)", "C"),
    132: ("ILLEGAL INSTRUCTION (SIGILL)", "L"),
    135: ("BUS ERROR (SIGBUS)", "B"),
    136: ("FLOATING POINT EXCEPTION (SIGFPE)", "F"),
    137: ("KILLED (SIGKILL)", "K"),
    139: ("SEGFAULT (SIGSEGV)", "S"),
    143: ("TERMINATED (SIGTERM)", "T"),
}

# Motifs valgrind -> (message, symbole) exit 99
VALGRIND_PATTERNS = [
    (("Invalid read", "Invalid write"), "INVALID MEMORY ACCESS", "M"),
    (("definitely lost:",), "MEMORY LEAK", "L"),
    (("Conditional jump or move depends on uninitialised value",), "UNINITIALIZED VALUE", "U"),
    (("Invalid free", "mismatched free"), "FREE ERROR", "F"),
    (("use-after-free",), "USE AFTER FREE", "A"),
    (("Source and destination overlap",), "MEMORY OVERLAP", "P"),
    (("Syscall param",), "SYSCALL PARAMETER", "C"),
    (("stack overflow",), "STACK OVERFLOW", "O"),
    (("stack smashing detected",), "STACK SMASHING", "S"),
    (("heap corruption",), "HEAP CORRUPTION", "H"),
    (("Invalid jump",), "INVALID JUMP", "J"),
]

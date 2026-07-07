"""Résultat de la comparaison d'un test (42_sh vs bash)."""

from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class Result:
    index: int
    cmd: str
    replay: str = ""
    ok_output: bool = False
    ok_code: bool = False
    ok_diff: bool = False
    ok_error: bool = False
    strict_ok: bool = False
    sh42_out: str = ""
    bash_out: str = ""
    sh42_code: int = 0
    bash_code: int = 0
    sh42_err: str = ""
    bash_err: str = ""
    outfiles_diff: list[str] = field(default_factory=list)
    critical: str = ""       # symboles d'erreurs valgrind/abort (mode -v)
    is_segfault: bool = False
    pipe_enabled: bool = True
    tty_enabled: bool = True
    tty_ok_output: bool = False
    tty_ok_code: bool = False
    tty_ok_diff: bool = False
    tty_ok_error: bool = True
    tty_strict_ok: bool = False
    tty_sh42_out: str = ""
    tty_bash_out: str = ""
    tty_sh42_code: int = 0
    tty_bash_code: int = 0
    tty_sh42_err: str = ""
    tty_bash_err: str = ""
    tty_outfiles_diff: list[str] = field(default_factory=list)

"""Résultat de la comparaison d'un test (minishell vs bash)."""

from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class Result:
    index: int
    cmd: str
    ok_output: bool = False
    ok_code: bool = False
    ok_diff: bool = False
    ok_error: bool = False
    strict_ok: bool = False
    mini_out: str = ""
    bash_out: str = ""
    mini_code: int = 0
    bash_code: int = 0
    mini_err: str = ""
    bash_err: str = ""
    outfiles_diff: list[str] = field(default_factory=list)
    critical: str = ""       # symboles d'erreurs valgrind/abort (mode -v)
    is_segfault: bool = False

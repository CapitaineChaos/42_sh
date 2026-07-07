"""Découverte des fichiers de cas (.tests, .toml) et chargement des Case."""

from __future__ import annotations

import sys
import shlex
import tomllib
from pathlib import Path

from .case import Case, Step
from .config import CASES, HERE
from .ui import C

PATTERNS = ("*.tests", "*.toml")     # tests simples ; tests spéciaux
MODE_DIRS = {"dual", "pipe", "tty"}


def categories() -> list[str]:
    """Sous-dossiers de cases/ contenant des fichiers de cas."""
    if not CASES.is_dir():
        return []
    return sorted(d.name for d in CASES.iterdir()
                  if d.is_dir()
                  and (d.name in MODE_DIRS
                       or any(any(d.rglob(p)) for p in PATTERNS)))


def discover(filters: list[str]) -> list[Path]:
    files: list[Path] = []
    if not filters:
        for cat in categories():
            for pat in PATTERNS:
                files += sorted((CASES / cat).rglob(pat))
        return files
    for arg in filters:
        # chemin de fichier direct
        hit = next((p for p in (Path(arg), HERE / arg, CASES / arg)
                    if p.is_file()), None)
        if hit:
            files.append(hit)
            continue
        catdir = CASES / arg.rstrip("_")
        if catdir.is_dir():
            for pat in PATTERNS:
                files += sorted(catdir.rglob(pat))
        else:
            sys.exit(f"{C.RED}Aucun test trouvé pour : {arg}{C.RESET}")
    return files


def case_label(f: Path) -> str:
    """ex: 'common/pipes.tests'."""
    try:
        return str(f.relative_to(CASES))
    except ValueError:
        return f.name


def load_cases(f: Path) -> list[Case]:
    """Charge les Case d'un fichier, selon son extension."""
    if f.suffix == ".toml":
        return _load_toml(f)
    return _load_plain(f)


def _case_mode(f: Path, explicit: str | None = None) -> str:
    aliases = {
        "dual": "dual",
        "pipe": "pipe",
        "tty": "tty",
    }
    if explicit in aliases:
        return aliases[explicit]
    try:
        rel = f.resolve().relative_to(CASES)
        root = rel.parts[0].lower()
    except (ValueError, IndexError):
        root = ""
    if root in aliases:
        return aliases[root]
    raise ValueError(f"case file must live under cases/dual, cases/pipe or cases/tty: {f}")


def _load_plain(f: Path) -> list[Case]:
    """.tests : une ligne = un cas (lignes blanches comprises)."""
    lines = f.read_text(errors="replace").split("\n")
    if lines and lines[-1] == "":        # ignore le cas vide dû au \n final
        lines.pop()
    mode = _case_mode(f)
    return [_plain_case(f, ln, mode) for ln in lines]


def _plain_case(f: Path, line: str, mode: str) -> Case:
    env = {}
    cmd = line
    if line.startswith("@env "):
        head, sep, tail = line[5:].partition(" -- ")
        if sep:
            for item in shlex.split(head):
                key, eq, value = item.partition("=")
                if eq:
                    env[key] = value
            cmd = tail
    return Case(source=f, index=0, steps=[Step(send=cmd + "\n")],
                env=env, mode=mode)


def _load_toml(f: Path) -> list[Case]:
    """.toml : tests spéciaux (multi-lignes, heredocs, interruptions)."""
    data = tomllib.loads(f.read_text(errors="replace"))
    return [Case(source=f, index=0, name=t.get("name", ""), steps=_steps(t),
                 env={k: str(v) for k, v in t.get("env", {}).items()},
                 mode=_case_mode(f, t.get("mode")))
            for t in data.get("test", [])]


def _steps(t: dict) -> list[Step]:
    """Steps explicites (`steps = [...]`) ou sucre `input` + `signal`."""
    if "steps" in t:
        return [Step(send=d.get("send"), signal=d.get("signal"),
                     eof=bool(d.get("eof", False)))
                for d in t["steps"]]
    steps = [Step(send=t["input"] + "\n")]
    if "signal" in t:
        steps.append(Step(signal=t["signal"]))
    return steps

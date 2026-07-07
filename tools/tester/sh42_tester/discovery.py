"""Découverte des fichiers de cas (.tests, .toml) et chargement des Case."""

from __future__ import annotations

import sys
import tomllib
from pathlib import Path

from .case import Case, Step
from .config import CASES, HERE
from .ui import C

PATTERNS = ("*.tests", "*.toml")     # tests simples ; tests spéciaux


def categories() -> list[str]:
    """Sous-dossiers de cases/ contenant des fichiers de cas."""
    if not CASES.is_dir():
        return []
    return sorted(d.name for d in CASES.iterdir()
                  if d.is_dir() and any(any(d.glob(p)) for p in PATTERNS))


def discover(filters: list[str]) -> list[Path]:
    files: list[Path] = []
    if not filters:
        for cat in categories():
            for pat in PATTERNS:
                files += sorted((CASES / cat).glob(pat))
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
                files += sorted(catdir.glob(pat))
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


def _load_plain(f: Path) -> list[Case]:
    """.tests : une ligne = un cas (lignes blanches comprises)."""
    lines = f.read_text(errors="replace").split("\n")
    if lines and lines[-1] == "":        # ignore le cas vide dû au \n final
        lines.pop()
    return [Case(source=f, index=0, steps=[Step(send=ln + "\n")])
            for ln in lines]


def _load_toml(f: Path) -> list[Case]:
    """.toml : tests spéciaux (multi-lignes, heredocs, interruptions)."""
    data = tomllib.loads(f.read_text(errors="replace"))
    return [Case(source=f, index=0, name=t.get("name", ""), steps=_steps(t),
                 env={k: str(v) for k, v in t.get("env", {}).items()})
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

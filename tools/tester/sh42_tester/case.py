"""Spec d'un cas de test : une séquence d'étapes (entrée / signal / attente)."""

from __future__ import annotations

from dataclasses import dataclass, field
from pathlib import Path


@dataclass
class Step:
    """Une étape de la séquence. Un seul champ actif à la fois.

    send   : octets écrits sur stdin (inclut ses propres \\n)
    signal : nom de signal envoyé au groupe ("INT", "QUIT", "TSTP", ...) une fois
             le groupe quiescent (bloqué en lecture / en sleep) — pas de chrono
    eof    : ferme stdin (CTRL-D)
    """
    send: str | None = None
    signal: str | None = None
    eof: bool = False


@dataclass
class Case:
    source: Path
    index: int
    steps: list[Step] = field(default_factory=list)
    name: str = ""           # à défaut, le label est dérivé des `send`

    @property
    def has_signal(self) -> bool:
        return any(s.signal for s in self.steps)

    @property
    def label(self) -> str:
        """Texte affiché sur la ligne de statut."""
        if self.name:
            return self.name
        text = "".join(s.send for s in self.steps if s.send)
        base = " ".join(text.splitlines()) or "(vide)"
        sigs = [s.signal for s in self.steps if s.signal]
        return f"{base} ⟨{'+'.join(sigs)}⟩" if sigs else base

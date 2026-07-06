"""Affichage : couleurs, titre du terminal, bannières, statuts, échecs, menu."""

from __future__ import annotations

import os
import re
import sys
import termios
import tty

from .result import Result

LINE_WIDTH = 80
_ANSI = re.compile(r"\033\[[0-9;]*m")


def _visible_len(s: str) -> int:
    """Longueur en caractères, codes couleur ANSI exclus."""
    return len(_ANSI.sub("", s))


def _fit(prefix: str, cmd: str) -> str:
    """prefix (coloré) + cmd (brut) coupé à LINE_WIDTH colonnes ; ... si tronqué."""
    budget = LINE_WIDTH - _visible_len(prefix)
    if len(cmd) > budget:
        cmd = cmd[:budget - 3] + "..." if budget > 3 else "." * max(0, budget)
    return prefix + cmd


def set_title(text: str) -> None:
    """Renomme le terminal via la séquence OSC (no-op si pas de tty)."""
    if sys.stdout.isatty():
        sys.stdout.write(f"\033]0;{text}\007")
        sys.stdout.flush()


class C:
    """Codes couleur ANSI (désactivés si pas de tty)."""
    on = sys.stdout.isatty()
    RESET = "\033[0m" if on else ""
    BOLD = "\033[1m" if on else ""
    GREY = "\033[38;5;244m" if on else ""
    RED = "\033[0;31m" if on else ""
    GREEN = "\033[0;32m" if on else ""
    YELLOW = "\033[0;33m" if on else ""
    BLUE = "\033[1;34m" if on else ""
    CYAN = "\033[1;36m" if on else ""
    PURPLE = "\033[0;35m" if on else ""


_BANNER_BORDER = " " + "-+H+" * 15 + "- "
_BANNER_WIDTH = len(_BANNER_BORDER)


def banner(title: str) -> str:
    """Encadre `title` centré entre deux bordures -+H+ de largeur fixe."""
    return (f"{C.YELLOW}{_BANNER_BORDER}{C.RESET}\n"
            f"{C.RED}{title.center(_BANNER_WIDTH)}{C.RESET}\n"
            f"{C.YELLOW}{_BANNER_BORDER}{C.RESET}")


def print_icons(r: Result) -> None:
    def ic(ok: bool) -> str:
        return f"{C.GREEN}✅{C.RESET}" if ok else f"{C.RED}❌{C.RESET}"
    line = (f"{C.YELLOW}{r.index:>4}: {ic(r.ok_output)}{ic(r.ok_code)}"
            f"{ic(r.ok_diff)}{ic(r.ok_error)}")
    if r.mini_code == 99:
        line += " 🥶"
    if r.critical:
        line += f" {C.PURPLE}💥{r.critical}{C.RESET}"
    print(_fit(f"{line}{C.GREY} ", r.cmd) + C.RESET)


def _fail_field(label: str, value: str) -> str:
    """Rend un champ (potentiellement multi-lignes) indenté et encadré."""
    lines = value.split("\n") if value != "" else ["(vide)"]
    out = [f"    {label}"]
    out += [f"      | {ln}" for ln in lines]
    return "\n".join(out)


def format_failure(r: Result, label: str) -> str:
    """Bloc texte propre décrivant un échec (ASCII, pas de couleurs)."""
    sep = "=" * 72
    p = [sep, f" Test {r.index}  |  {label}", sep,
         " Commande essayée :", f"   {r.cmd}", ""]
    if not r.ok_output:
        p.append(" [STDOUT] DIFFÉRENT")
        p.append(_fail_field("attendu  (bash)      :", r.bash_out))
        p.append(_fail_field("obtenu   (minishell) :", r.mini_out))
        p.append("")
    if not r.ok_code:
        p.append(f" [EXIT CODE] DIFFÉRENT : attendu={r.bash_code}"
                 f"  obtenu={r.mini_code}")
        p.append("")
    if not r.ok_diff:
        p.append(" [OUTFILES] DIFFÉRENT")
        p += [f"   {d}" for d in r.outfiles_diff]
        p.append("")
    if not r.ok_error:
        p.append(" [STDERR] DIFFÉRENT")
        p.append(_fail_field("attendu  (bash)      :", r.bash_err))
        p.append(_fail_field("obtenu   (minishell) :", r.mini_err))
        p.append("")
    if r.is_segfault:
        p.append(" [SEGFAULT] minishell a segfault (exit 139)")
        p.append("")
    if r.mini_code == 124:
        p.append(" [TIMEOUT] minishell n'a pas rendu la main (blocage ?)")
        p.append("")
    if r.critical:
        p.append(f" [VALGRIND] erreurs : {r.critical}")
        p.append("")
    return "\n".join(p)


def _read_key() -> str:
    """Lit une seule touche sans Entrée (mode cbreak). '' si EOF/Ctrl+C/non-tty."""
    fd = sys.stdin.fileno()
    try:
        old = termios.tcgetattr(fd)
    except (termios.error, ValueError):
        return ""
    try:
        tty.setcbreak(fd)
        data = os.read(fd, 1)
    except (KeyboardInterrupt, OSError):
        return ""
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old)
    return data.decode(errors="replace")


def prompt_retry(n_failed: int) -> str:
    """Menu à une touche : 'r' relancer, 'v' voir le rapport, sinon 'q'."""
    if not sys.stdin.isatty():
        return "q"

    width = 46

    def row(text: str) -> str:
        pad = " " * max(0, width - 1 - len(text))
        return f"║ {text}{pad}║"

    top = f"╔{'═' * width}╗"
    mid = f"╠{'═' * width}╣"
    bot = f"╚{'═' * width}╝"

    print("\n".join([
        "",
        top,
        row(f"{n_failed} cas en échec"),
        mid,
        row("R) Réexécuter les cas en échec"),
        row("V) Voir le rapport d'erreurs"),
        row(""),
        row("Appuyer sur une touche pour quitter"),
        bot,
    ]))

    key = _read_key().lower()
    if key == "r":
        return "r"
    if key == "v":
        return "v"
    return "q"

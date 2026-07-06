#!/usr/bin/env python3
"""Point d'entrée. Usage : ./tester.py --help. Détails : mnsh_tester/README.md."""

from __future__ import annotations

import sys

from mnsh_tester.cli import main
from mnsh_tester.execution import RUNNING, RUNNING_LOCK, STOP
from mnsh_tester.ui import C


if __name__ == "__main__":
    try:
        sys.exit(main())
    except KeyboardInterrupt:
        STOP.set()
        with RUNNING_LOCK:
            for p in list(RUNNING):
                p.kill()
        print(f"\n{C.YELLOW}⏹  Interrompu.{C.RESET}")
        sys.exit(130)

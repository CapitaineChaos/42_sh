#!/usr/bin/env python3
"""Point d'entrée. Usage : ./tester.py --help. Détails : sh42_tester/README.md."""

from __future__ import annotations

import sys

from sh42_tester.cli import main
from sh42_tester.execution import RUNNING, RUNNING_LOCK, STOP
from sh42_tester.ui import C


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

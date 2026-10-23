#!/usr/bin/env python3
"""Fail closed if a read-only probe acquires trophy mutation entry points."""
from pathlib import Path
import re
import sys

source = Path("source/probe.c").read_text(encoding="utf-8")
forbidden = (
    "sceNpTrophyUnlockTrophy",
    "sceNpTrophyNetworkSync",
    "sceNpTrophySetSoundLevel",
)
# Only actual C symbol references matter, not this Python file.
hits = [name for name in forbidden if re.search(r"\\b" + name + r"\\b", source)]
if hits:
    sys.exit("Unexpected write-capable references: " + ", ".join(hits))
print("Read-only probe source gate passed.")

#!/usr/bin/env bash
# Conformance runner for the Pascal port. Usage: run.sh OUT_DIR VECTOR_DIR...
# One variant, "single": the unit only has Single precision.
# -Sc (C-style operators like *=) is needed by the unit; it usually comes from fpc.cfg, so pass it explicitly.
# -B rebuilds the unit into OUT_DIR even if a fastnoiselite.ppu from another build sits next to its source.
set -euo pipefail
out=$1; shift
here=$(cd "$(dirname "$0")" && pwd)
fpc -B -O2 -Sc -Fu"$here/.." -FU"$out" -FE"$out" "$here/driver.pas"
for v in "$@"; do
    "$out/driver" "$v" > "$out/$(basename "$v").single.txt" || true
done

#!/usr/bin/env bash
# Conformance build for the Pascal port. See conformance/README.md; run.py runs this in OUT_DIR.
# One variant, "single": the unit only has Single precision.
# -Sc (C-style operators like *=) is needed by the unit; it usually comes from fpc.cfg, so pass it explicitly.
# -B rebuilds the unit here even if a fastnoiselite.ppu from another build sits next to its source.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
fpc -B -O2 -Sc -Fu"$here/.." -FU. -FE. "$here/driver.pas"
cat > variants.txt <<EOF
single ./driver
EOF

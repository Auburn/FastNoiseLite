#!/usr/bin/env bash
# Conformance build for the Haxe port. See conformance/README.md; run.py runs this in OUT_DIR.
# Variants: interp (Haxe's eval interpreter, haxe --run) and js (JS target run with node).
# Haxe's Float is 64-bit on both, so they are double-precision ports.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
haxe -cp "$here" -cp "$here/.." -main Driver --interp --no-output # type-check for the interp variant
haxe -cp "$here" -cp "$here/.." -main Driver -js driver.js
cat > variants.txt <<EOF
interp haxe -cp "$here" -cp "$here/.." --run Driver
js node driver.js
EOF

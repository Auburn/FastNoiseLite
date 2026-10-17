#!/usr/bin/env bash
# Conformance runner for the Haxe port. Usage: run.sh OUT_DIR VECTOR_DIR...
# Variants: interp (Haxe's eval interpreter, haxe --run) and js (JS target run with node).
# Haxe's Float is 64-bit on both, so they are double-precision ports.
set -euo pipefail
out=$1; shift
here=$(cd "$(dirname "$0")" && pwd)
cp=(-cp "$here" -cp "$here/..")
haxe "${cp[@]}" -main Driver --interp --no-output # type-check only: --run below can't fail the build
haxe "${cp[@]}" -main Driver -js "$out/driver.js"
for v in "$@"; do
    haxe "${cp[@]}" --run Driver "$v" > "$out/$(basename "$v").interp.txt" || true
    node "$out/driver.js" "$v" > "$out/$(basename "$v").js.txt" || true
done

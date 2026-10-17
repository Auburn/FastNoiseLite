#!/usr/bin/env bash
# Conformance runner for the JavaScript port. Usage: run.sh OUT_DIR VECTOR_DIR...
# One variant, "dist": the built dist/FastNoiseLite.js (JS computes in double precision).
set -euo pipefail
out=$1; shift
here=$(cd "$(dirname "$0")" && pwd)
for v in "$@"; do
    node "$here/driver.mjs" "$v" > "$out/$(basename "$v").dist.txt" || true
done

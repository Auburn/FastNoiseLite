#!/usr/bin/env bash
# Conformance runner for the Odin port. Usage: run.sh OUT_DIR VECTOR_DIR...
# Variants: f32 (as shipped) and f64 (-define:USE_F64=true). CI uses odin dev-2025-04; dev-2024-10 gives the
# same output, and dev-2026-09 folds typed f32 constants in f32, which moves some values by 1 ulp.
set -euo pipefail
out=$1; shift
here=$(cd "$(dirname "$0")" && pwd)
ODIN=${ODIN:-odin}
"$ODIN" build "$here/driver.odin" -file -o:speed -out:"$out/driver_f32"
"$ODIN" build "$here/driver.odin" -file -o:speed -define:USE_F64=true -out:"$out/driver_f64"
for v in "$@"; do
    for t in f32 f64; do
        "$out/driver_$t" "$v" > "$out/$(basename "$v").$t.txt" || true
    done
done

#!/usr/bin/env bash
# Conformance runner for the C port. Usage: run.sh OUT_DIR VECTOR_DIR...
# Variants: float (header as shipped) and double (FNLfloat switched to double in a copy).
set -euo pipefail
out=$1; shift
here=$(cd "$(dirname "$0")" && pwd)
CC=${CC:-gcc}
sed 's|^typedef float FNLfloat;|typedef double FNLfloat;|' "$here/../FastNoiseLite.h" > "$out/FastNoiseLite_double.h"
"$CC" -std=c99 -O2 -fwrapv -ffp-contract=off "$here/driver.c" -o "$out/driver_float" -lm
"$CC" -std=c99 -O2 -fwrapv -ffp-contract=off -DFNL_HEADER="\"$out/FastNoiseLite_double.h\"" "$here/driver.c" -o "$out/driver_double" -lm
for v in "$@"; do
    for t in float double; do
        "$out/driver_$t" "$v" > "$out/$(basename "$v").$t.txt" || true
    done
done

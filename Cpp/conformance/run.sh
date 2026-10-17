#!/usr/bin/env bash
# Conformance runner for the C++ reference. Usage: run.sh OUT_DIR VECTOR_DIR...
# Writes OUT_DIR/<set>.float.txt and OUT_DIR/<set>.double.txt for each vector set.
# Flags are pinned so the reference is identical on every machine (-ffp-contract=off stops FMA
# fusion; -fwrapv makes the 32-bit hash wrap-around well defined).
set -euo pipefail
out=$1; shift
here=$(cd "$(dirname "$0")" && pwd)
CXX=${CXX:-g++}
for t in float double; do
    "$CXX" -std=c++11 -O2 -fwrapv -ffp-contract=off -DFNL_COORD=$t "$here/driver.cpp" -o "$out/driver_$t"
done
for v in "$@"; do
    for t in float double; do
        "$out/driver_$t" "$v" > "$out/$(basename "$v").$t.txt" || true
    done
done

#!/usr/bin/env bash
# Conformance runner for the Zig port. Usage: run.sh OUT_DIR VECTOR_DIR...
# Needs Zig 0.15.x, taken from $ZIG or `zig` on PATH. Variants: f32 and f64 built with ReleaseFast, and
# f32-safe built with ReleaseSafe (runtime safety checks on): there a panic stops the driver and the
# rest of that output file counts as missing.
set -euo pipefail
out=$1; shift
here=$(cd "$(dirname "$0")" && pwd)
ZIG=${ZIG:-zig}
for mode in ReleaseFast ReleaseSafe; do
    "$ZIG" build-exe -O $mode --cache-dir "$out/zig-cache" --dep fastnoise -Mroot="$here/driver.zig" \
        -Mfastnoise="$here/../fastnoise.zig" -femit-bin="$out/driver_$mode"
done
for v in "$@"; do
    s=$(basename "$v")
    "$out/driver_ReleaseFast" "$v" f32 > "$out/$s.f32.txt" || true
    "$out/driver_ReleaseFast" "$v" f64 > "$out/$s.f64.txt" || true
    "$out/driver_ReleaseSafe" "$v" f32 > "$out/$s.f32-safe.txt" || true
done

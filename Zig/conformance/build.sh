#!/usr/bin/env bash
# Conformance build for the Zig port. See conformance/README.md; run.py runs this in OUT_DIR.
# Needs Zig 0.15.x, taken from $ZIG or `zig` on PATH. Variants: f32 and f64 built with ReleaseFast, and
# f32-safe built with ReleaseSafe (runtime safety checks on): there a panic stops the driver and the
# rest of that output counts as missing.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
ZIG=${ZIG:-zig}
for mode in ReleaseFast ReleaseSafe; do
    "$ZIG" build-exe -O $mode --cache-dir zig-cache --dep fastnoise -Mroot="$here/driver.zig" \
        -Mfastnoise="$here/../fastnoise.zig" -femit-bin=driver_$mode
done
cat > variants.txt <<EOF
f32 ./driver_ReleaseFast f32
f64 ./driver_ReleaseFast f64
f32-safe ./driver_ReleaseSafe f32
EOF

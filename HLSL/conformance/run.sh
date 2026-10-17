#!/usr/bin/env bash
# Conformance runner for the HLSL port. Usage: run.sh OUT_DIR VECTOR_DIR...
# Variants: float (file as shipped) and double (FNLfloat switched to double in a copy).
# driver.hlsl is compiled with DXC to SPIR-V, translated to GLSL 4.30 with spirv-cross, and driver.py
# runs it with moderngl. Needs dxc, spirv-cross, python3 with moderngl, and EGL with GL 4.3.
set -euo pipefail
out=$1; shift
here=$(cd "$(dirname "$0")" && pwd)
DXC=${DXC:-dxc}
mkdir -p "$out/double"
sed 's|^typedef float FNLfloat;|typedef double FNLfloat;|' "$here/../FastNoiseLite.hlsl" > "$out/double/FastNoiseLite.hlsl"
for t in float double; do
    inc=$here/..
    [ $t = double ] && inc=$out/double
    "$DXC" -T cs_6_0 -E main -spirv -I "$inc" "$here/driver.hlsl" -Fo "$out/kernel_$t.spv"
    spirv-cross --version 430 "$out/kernel_$t.spv" --output "$out/kernel_$t.comp"
done
for v in "$@"; do
    for t in float double; do
        python3 "$here/driver.py" "$out/kernel_$t.comp" $t "$v" > "$out/$(basename "$v").$t.txt" || true
    done
done

#!/usr/bin/env bash
# Conformance build for the HLSL port. See conformance/README.md; run.py runs this in OUT_DIR.
# Variants: float (file as shipped) and double (FNLfloat switched to double in a copy).
# driver.hlsl is compiled with DXC to SPIR-V, translated to GLSL 4.30 with spirv-cross, and driver.py
# runs it with moderngl. Needs dxc, spirv-cross, python3 with moderngl, and EGL with GL 4.3.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
DXC=${DXC:-dxc}
mkdir -p double
sed 's|^typedef float FNLfloat;|typedef double FNLfloat;|' "$here/../FastNoiseLite.hlsl" > double/FastNoiseLite.hlsl
for t in float double; do
    inc=$here/..
    [ $t = double ] && inc=double
    "$DXC" -T cs_6_0 -E main -spirv -I "$inc" "$here/driver.hlsl" -Fo kernel_$t.spv
    spirv-cross --version 430 kernel_$t.spv --output kernel_$t.comp
done
cat > variants.txt <<EOF
float python3 "$here/driver.py" kernel_float.comp float
double python3 "$here/driver.py" kernel_double.comp double
EOF

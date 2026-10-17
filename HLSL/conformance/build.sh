#!/usr/bin/env bash
# Conformance build for the HLSL port. See conformance/README.md; run.py runs this in OUT_DIR.
# Variants: float (file as shipped) and double (FNLfloat switched to double in a copy).
# driver.hlsl is compiled with DXC to SPIR-V and translated to GLSL 4.30 with spirv-cross; conformance/gpu.py
# runs it with moderngl. Needs dxc, spirv-cross, python3 with moderngl, and EGL with GL 4.3.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
gpu=$here/../../conformance/gpu.py
DXC=${DXC:-dxc}
mkdir -p double
bash "$here/../../conformance/replace.sh" "$here/../FastNoiseLite.hlsl" double/FastNoiseLite.hlsl 'typedef float FNLfloat;' 'typedef double FNLfloat;'
for t in float double; do
    inc=$here/..
    [ $t = double ] && inc=double
    "$DXC" -T cs_6_0 -E main -spirv -I "$inc" "$here/driver.hlsl" -Fo kernel_$t.spv
    spirv-cross --version 430 kernel_$t.spv --output kernel_$t.comp
    python3 "$gpu" kernel_$t.comp $t   # fail here if the translated shader does not compile
done
cat > variants.txt <<EOF
float python3 "$gpu" kernel_float.comp float
double python3 "$gpu" kernel_double.comp double
EOF

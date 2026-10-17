#!/usr/bin/env bash
# Conformance build for the GLSL port. See conformance/README.md; run.py runs this in OUT_DIR.
# One variant, "float": FastNoiseLite.glsl as shipped plus driver.glsl, run as a GL 4.3 compute shader by
# conformance/gpu.py. No double variant: the port's documented `#define FNLfloat double` option does not compile.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
gpu=$here/../../conformance/gpu.py
{ echo "#version 430"; cat "$here/../FastNoiseLite.glsl"; echo; cat "$here/driver.glsl"; } > kernel.comp
python3 "$gpu" kernel.comp float   # the shader is compiled at run time; fail here if it does not compile
cat > variants.txt <<EOF
float python3 "$gpu" kernel.comp float
EOF

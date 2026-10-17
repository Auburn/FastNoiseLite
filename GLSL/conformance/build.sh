#!/usr/bin/env bash
# Conformance build for the GLSL port. See conformance/README.md; run.py runs this in OUT_DIR.
# One variant, "float": FastNoiseLite.glsl as shipped, in a GL 4.3 compute shader via moderngl + EGL
# (Mesa llvmpipe on machines without a GPU). No double variant: the port's documented
# `#define FNLfloat double` option does not compile.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
python3 "$here/driver.py" --build   # the shader is compiled at run time; fail here if it does not compile
cat > variants.txt <<EOF
float python3 "$here/driver.py"
EOF

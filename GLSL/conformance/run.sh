#!/usr/bin/env bash
# Conformance runner for the GLSL port. Usage: run.sh OUT_DIR VECTOR_DIR...
# One variant, "float": FastNoiseLite.glsl as shipped, in a GL 4.3 compute shader via moderngl + EGL
# (Mesa llvmpipe on machines without a GPU). No double variant: the port's documented
# `#define FNLfloat double` option does not compile.
set -euo pipefail
out=$1; shift
here=$(cd "$(dirname "$0")" && pwd)
python3 "$here/driver.py" --build   # the shader is compiled at run time; fail here if it does not compile
for v in "$@"; do
    python3 "$here/driver.py" "$v" > "$out/$(basename "$v").float.txt" || true
done

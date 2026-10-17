#!/usr/bin/env bash
# Conformance build for the Fortran port. See conformance/README.md; run.py runs this in OUT_DIR.
# One variant, float: the module's coordinate kind, fnl_float = kind(c_double), is the kind of the integer
# constant c_double (4), i.e. c_float, and there is no working double switch.
# The module needs -ffree-line-length-none (lines over 132 characters) and -fwrapv (its hashes rely on
# 32-bit wrap-around); the other flags match the C++ reference build.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
FC=${FC:-gfortran}
flags=(-O2 -fwrapv -ffp-contract=off -ffree-line-length-none -J .)
"$FC" "${flags[@]}" -c "$here/../fast_noise_lite.f90" -o fast_noise_lite.o
"$FC" "${flags[@]}" "$here/driver.f90" fast_noise_lite.o -o driver
cat > variants.txt <<EOF
float ./driver
EOF

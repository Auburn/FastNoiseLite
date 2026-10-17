#!/usr/bin/env bash
# Conformance runner for the Fortran port. Usage: run.sh OUT_DIR VECTOR_DIR...
# One variant, float: the module's coordinate kind, fnl_float = kind(c_double), is the kind of the integer
# constant c_double (4), i.e. c_float, and there is no working double switch.
# The module needs -ffree-line-length-none (lines over 132 characters) and -fwrapv (its hashes rely on
# 32-bit wrap-around); the other flags match the C++ reference build.
set -euo pipefail
out=$1; shift
here=$(cd "$(dirname "$0")" && pwd)
FC=${FC:-gfortran}
flags=(-O2 -fwrapv -ffp-contract=off -ffree-line-length-none -J "$out")
"$FC" "${flags[@]}" -c "$here/../fast_noise_lite.f90" -o "$out/fast_noise_lite.o"
"$FC" "${flags[@]}" "$here/driver.f90" "$out/fast_noise_lite.o" -o "$out/driver"
for v in "$@"; do
    "$out/driver" "$v" > "$out/$(basename "$v").float.txt" || true
done

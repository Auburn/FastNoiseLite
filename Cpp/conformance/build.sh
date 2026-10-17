#!/usr/bin/env bash
# Conformance build for the C++ reference. See conformance/README.md; run.py runs this in OUT_DIR.
# Variants: float and double coordinates. Flags are pinned so the reference is identical on every machine
# (-ffp-contract=off stops FMA fusion; -fwrapv makes the 32-bit hash wrap-around well defined).
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
CXX=${CXX:-g++}
for t in float double; do
    "$CXX" -std=c++11 -O2 -fwrapv -ffp-contract=off -DFNL_COORD=$t "$here/driver.cpp" -o driver_$t
done
cat > variants.txt <<EOF
float ./driver_float
double ./driver_double
EOF

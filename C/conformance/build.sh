#!/usr/bin/env bash
# Conformance build for the C port. See conformance/README.md; run.py runs this in OUT_DIR.
# Variants: float (header as shipped) and double (FNLfloat switched to double in a copy).
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
CC=${CC:-gcc}
bash "$here/../../conformance/replace.sh" "$here/../FastNoiseLite.h" FastNoiseLite_double.h 'typedef float FNLfloat;' 'typedef double FNLfloat;'
"$CC" -std=c99 -O2 -fwrapv -ffp-contract=off "$here/driver.c" -o driver_float -lm
"$CC" -std=c99 -O2 -fwrapv -ffp-contract=off -DFNL_HEADER="\"$PWD/FastNoiseLite_double.h\"" "$here/driver.c" -o driver_double -lm
cat > variants.txt <<EOF
float ./driver_float
double ./driver_double
EOF

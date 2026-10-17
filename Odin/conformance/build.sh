#!/usr/bin/env bash
# Conformance build for the Odin port. See conformance/README.md; run.py runs this in OUT_DIR.
# Variants: f32 (as shipped) and f64 (-define:USE_F64=true). CI uses odin dev-2025-04; dev-2024-10 gives the
# same output, and dev-2026-09 folds typed f32 constants in f32, which moves some values by 1 ulp.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
ODIN=${ODIN:-odin}
"$ODIN" build "$here/driver.odin" -file -o:speed -out:driver_f32
"$ODIN" build "$here/driver.odin" -file -o:speed -define:USE_F64=true -out:driver_f64
cat > variants.txt <<EOF
f32 ./driver_f32
f64 ./driver_f64
EOF

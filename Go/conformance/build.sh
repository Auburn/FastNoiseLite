#!/usr/bin/env bash
# Conformance build for the Go port. See conformance/README.md; run.py runs this in OUT_DIR.
# Variants: float32 and float64 (the two instantiations of the generic State[T]). Needs Go 1.20+ (go -C).
# -buildvcs=false: no git lookup, so the build also works in a checkout owned by another user.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
go -C "$here" build -buildvcs=false -o "$PWD/driver" .
cat > variants.txt <<EOF
float32 ./driver float32
float64 ./driver float64
EOF

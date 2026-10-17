#!/usr/bin/env bash
# Conformance runner for the Go port. Usage: run.sh OUT_DIR VECTOR_DIR...
# Variants: float32 and float64 (the two instantiations of the generic State[T]). Needs Go 1.20+ (go -C).
# -buildvcs=false: no git lookup, so the build also works in a checkout owned by another user.
set -euo pipefail
out=$(cd "$1" && pwd); shift
here=$(cd "$(dirname "$0")" && pwd)
go -C "$here" build -buildvcs=false -o "$out/driver" .
for v in "$@"; do
    for t in float32 float64; do
        "$out/driver" "$v" "$t" > "$out/$(basename "$v").$t.txt" || true
    done
done

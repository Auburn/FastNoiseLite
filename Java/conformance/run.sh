#!/usr/bin/env bash
# Conformance runner for the Java port. Usage: run.sh OUT_DIR VECTOR_DIR...
# Variants: float (port as shipped) and double (the port's documented "/*FNLfloat*/ float" ->
# "/*FNLfloat*/ double" replace, applied to copies of the port and the driver).
set -euo pipefail
out=$1; shift
here=$(cd "$(dirname "$0")" && pwd)
for t in float double; do
    mkdir -p "$out/$t"
    sed "s|/\*FNLfloat\*/ float|/*FNLfloat*/ $t|g" "$here/../FastNoiseLite.java" > "$out/$t/FastNoiseLite.java"
    sed "s|/\*FNLfloat\*/ float|/*FNLfloat*/ $t|g" "$here/Driver.java" > "$out/$t/Driver.java"
    javac -d "$out/$t" "$out/$t/FastNoiseLite.java" "$out/$t/Driver.java"
done
for v in "$@"; do
    for t in float double; do
        java -cp "$out/$t" Driver "$v" > "$out/$(basename "$v").$t.txt" || true
    done
done

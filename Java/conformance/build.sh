#!/usr/bin/env bash
# Conformance build for the Java port. See conformance/README.md; run.py runs this in OUT_DIR.
# Variants: float (port as shipped) and double (the port's documented "/*FNLfloat*/ float" ->
# "/*FNLfloat*/ double" replace, applied to copies of the port and the driver).
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
for t in float double; do
    mkdir -p $t
    for f in "$here/../FastNoiseLite.java" "$here/Driver.java"; do
        bash "$here/../../conformance/replace.sh" "$f" "$t/$(basename "$f")" '/*FNLfloat*/ float' "/*FNLfloat*/ $t"
    done
    javac -d $t $t/FastNoiseLite.java $t/Driver.java
done
cat > variants.txt <<EOF
float java -cp float Driver
double java -cp double Driver
EOF

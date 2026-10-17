#!/usr/bin/env bash
# Conformance build for the JavaScript port. See conformance/README.md; run.py runs this in OUT_DIR.
# One variant, "dist": the built dist/FastNoiseLite.js (JS computes in double precision). Nothing to build;
# the version check fails the build if node is missing.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
node --version
cat > variants.txt <<EOF
dist node "$here/driver.mjs"
EOF

#!/usr/bin/env bash
# Conformance build for the PowerShell port. See conformance/README.md; run.py runs this in OUT_DIR.
# One variant, "float": the port's fields and API are Single (PowerShell does the arithmetic itself
# in Double). Nothing to build; the version check fails the build if pwsh is missing.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
pwsh -NoProfile -NonInteractive -Command '$PSVersionTable.PSVersion.ToString()'
cat > variants.txt <<EOF
float pwsh -NoProfile -NonInteractive -File "$here/driver.ps1"
EOF

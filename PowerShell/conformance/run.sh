#!/usr/bin/env bash
# Conformance runner for the PowerShell port. Usage: run.sh OUT_DIR VECTOR_DIR...
# One variant, "float": the port's fields and API are Single (PowerShell does the arithmetic itself
# in Double). Nothing to build; the version check fails the run if pwsh is missing.
set -euo pipefail
out=$1; shift
here=$(cd "$(dirname "$0")" && pwd)
pwsh -NoProfile -NonInteractive -Command '$PSVersionTable.PSVersion.ToString()'
for v in "$@"; do
    pwsh -NoProfile -NonInteractive -File "$here/driver.ps1" "$v" > "$out/$(basename "$v").float.txt" || true
done

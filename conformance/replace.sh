#!/usr/bin/env bash
# Usage: replace.sh SRC DEST FROM TO
# Copies SRC to DEST with every occurrence of the literal text FROM replaced by TO. Build scripts use it to
# make a double-precision copy of a port. It fails when SRC doesn't contain FROM, so a renamed or reformatted
# precision switch breaks the build instead of quietly producing a second float variant (which would still
# pass, because outputs are compared with both the float and the double reference).
set -euo pipefail
src=$1 dest=$2 from=$3 to=$4
IFS= read -r -d '' text < "$src" || true   # the whole file, trailing newlines included
if [[ $text != *"$from"* ]]; then
    echo "replace.sh: '$from' not found in $src" >&2
    exit 1
fi
printf '%s' "${text//"$from"/"$to"}" > "$dest"

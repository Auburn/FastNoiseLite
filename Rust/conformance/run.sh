#!/usr/bin/env bash
# Conformance runner for the Rust port. Usage: run.sh OUT_DIR VECTOR_DIR...
# Variants: f32 (default features), f64 (--features f64), both release; f32-debug (debug profile, so
# integer overflow panics; the driver skips the lines that panic).
# The driver is built as a scratch crate in OUT_DIR with a path dependency on the port, so nothing is
# written inside Rust/.
set -euo pipefail
out=$(cd "$1" && pwd); shift
here=$(cd "$(dirname "$0")" && pwd)
crate="$out/crate"
mkdir -p "$crate/src"
cp "$here/driver.rs" "$crate/src/main.rs"
cp "$here/../Cargo.lock" "$crate/Cargo.lock"   # same dependency versions as the port
cat > "$crate/Cargo.toml" <<EOF
[package]
name = "driver"
version = "0.0.0"
edition = "2021"

[workspace] # standalone, even if OUT_DIR is inside another Cargo workspace

[dependencies]
fastnoise-lite = { path = "$here/.." }

[features]
f64 = ["fastnoise-lite/f64"]
EOF
export CARGO_TARGET_DIR="$out/target"
(
    cd "$crate"
    cargo build --release
    cp "$CARGO_TARGET_DIR/release/driver" "$out/driver_f32"
    cargo build --release --features f64
    cp "$CARGO_TARGET_DIR/release/driver" "$out/driver_f64"
    cargo build   # debug profile: overflow checks on
    cp "$CARGO_TARGET_DIR/debug/driver" "$out/driver_f32-debug"
)
for v in "$@"; do
    for t in f32 f64 f32-debug; do
        "$out/driver_$t" "$v" > "$out/$(basename "$v").$t.txt" || true
    done
done

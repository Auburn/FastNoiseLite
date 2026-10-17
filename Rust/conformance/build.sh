#!/usr/bin/env bash
# Conformance build for the Rust port. See conformance/README.md; run.py runs this in OUT_DIR.
# Variants: f32 (default features), f64 (--features f64), both release; f32-debug (debug profile, so
# integer overflow panics; the driver skips the lines that panic).
# The driver is built as a scratch crate with a path dependency on the port, so nothing is written inside Rust/.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
mkdir -p crate/src
cp "$here/driver.rs" crate/src/main.rs
cp "$here/../Cargo.lock" crate/Cargo.lock   # same dependency versions as the port
cat > crate/Cargo.toml <<EOF
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
export CARGO_TARGET_DIR="$PWD/target"
(
    cd crate
    cargo build --release
    cp "$CARGO_TARGET_DIR/release/driver" ../driver_f32
    cargo build --release --features f64
    cp "$CARGO_TARGET_DIR/release/driver" ../driver_f64
    cargo build   # debug profile: overflow checks on
    cp "$CARGO_TARGET_DIR/debug/driver" ../driver_f32-debug
)
cat > variants.txt <<EOF
f32 ./driver_f32
f64 ./driver_f64
f32-debug ./driver_f32-debug
EOF

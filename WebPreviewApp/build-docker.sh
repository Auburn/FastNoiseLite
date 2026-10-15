#!/usr/bin/env sh
# Builds the web preview app with Emscripten inside Docker (no local emsdk needed).
# Output: WebPreviewApp/dist/index.html (single self-contained file, open it directly in a browser)
set -e

EMSDK_VER=3.1.52
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_DIR="$(dirname "$SCRIPT_DIR")"

docker run --rm --platform linux/amd64 \
    -v "$REPO_DIR":/src \
    -w /src/WebPreviewApp \
    -u "$(id -u):$(id -g)" \
    -e HOME=/tmp \
    emscripten/emsdk:$EMSDK_VER \
    sh -c "emcmake cmake -Bbuild-docker -DCMAKE_BUILD_TYPE=Release && cmake --build build-docker -j"

mkdir -p "$SCRIPT_DIR/dist"
cp "$SCRIPT_DIR/build-docker/FastNoiseLitePreview.html" "$SCRIPT_DIR/dist/index.html"
echo "Built: $SCRIPT_DIR/dist/index.html"

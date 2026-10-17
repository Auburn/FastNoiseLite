#!/usr/bin/env python3
"""Conformance driver for the shader ports (GLSL and HLSL). See README.md.

usage: gpu.py KERNEL float|double [VECTORS_DIR]

KERNEL is a GLSL 4.30 compute shader holding the port and its conformance kernel; the second argument is
the kernel's FNLfloat type. The shader runs through moderngl on the EGL default device (Mesa llvmpipe on a
machine without a GPU). Without VECTORS_DIR the kernel is only compiled, which the build scripts use to
fail the build on a compile error. With it, the results are printed in the format README.md describes.

Every kernel (GLSL/conformance/driver.glsl, HLSL/conformance/driver.hlsl) follows this layout:
  dispatch    one invocation per (point i, instance c), at global invocation ID (i, c). Instance 0 is
              the default state, instance c > 0 is line c - 1 of configs.txt.
  binding 0   Config configs[]: the 15 configs.txt fields in order, each a 4-byte int or float.
  binding 1   FNLfloat points[]: 5 per point, the 2D point x y then the 3D point x y z. The shorter
              point list is padded with zeros.
  binding 2   FNLfloat results[]: 7 per (instance, point), starting at (c * points + i) * 7:
              GetNoise 2D, GetNoise 3D, DomainWarp 2D x y, DomainWarp 3D x y z.
"""
import argparse
import os
import struct
import sys
from array import array

import moderngl

CONFIG = struct.Struct("<ifiiiiffffiifif")  # the Config struct, in configs.txt field order
RESULTS = 7


def rows(vectors_dir, name):
    with open(os.path.join(vectors_dir, name)) as f:
        return [line.split() for line in f if line.strip()]


def main():
    parser = argparse.ArgumentParser(description="Run a conformance compute shader. See the module docstring.")
    parser.add_argument("kernel")
    parser.add_argument("type", choices=["float", "double"])
    parser.add_argument("vectors", nargs="?", help="omit to only compile the kernel")
    args = parser.parse_args()

    ctx = moderngl.create_standalone_context(backend="egl", require=430)
    print("GL_RENDERER:", ctx.info["GL_RENDERER"], file=sys.stderr)
    with open(args.kernel) as f:
        shader = ctx.compute_shader(f.read())
    if args.vectors is None:
        return 0

    p2, p3, configs = (rows(args.vectors, name) for name in ("points2d.txt", "points3d.txt", "configs.txt"))
    n = max(len(p2), len(p3))
    points = array("d" if args.type == "double" else "f")
    for i in range(n):
        points.extend(map(float, (p2[i] if i < len(p2) else [0, 0]) + (p3[i] if i < len(p3) else [0, 0, 0])))
    types = CONFIG.format[1:]
    cfg = b"".join(CONFIG.pack(*(int(v) if t == "i" else float(v) for t, v in zip(types, c))) for c in configs)
    instances = len(configs) + 1
    buffers = [ctx.buffer(cfg or bytes(CONFIG.size)), ctx.buffer(points.tobytes()),
               ctx.buffer(reserve=instances * n * RESULTS * points.itemsize)]
    for binding, b in enumerate(buffers):
        b.bind_to_storage_buffer(binding)
    shader.run(n, instances)
    ctx.memory_barrier()  # GL requires a barrier before reading back buffers that a shader wrote
    res = array(points.typecode, buffers[2].read())

    out = []
    for c in range(instances):
        r = lambda i, k: f"{res[(c * n + i) * RESULTS + k]:.17g}"
        out += [f"N2 {c - 1} {i} {r(i, 0)}" for i in range(len(p2))]
        out += [f"N3 {c - 1} {i} {r(i, 1)}" for i in range(len(p3))]
        out += [f"W2 {c - 1} {i} {r(i, 2)} {r(i, 3)}" for i in range(len(p2))]
        out += [f"W3 {c - 1} {i} {r(i, 4)} {r(i, 5)} {r(i, 6)}" for i in range(len(p3))]
    sys.stdout.write("\n".join(out) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())

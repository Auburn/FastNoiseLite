#!/usr/bin/env python3
"""FastNoiseLite conformance driver (HLSL). See conformance/README.md.
Usage: driver.py KERNEL.comp float|double VECTORS_DIR > output.txt
KERNEL.comp is driver.hlsl translated to GLSL by run.sh; it runs as an OpenGL compute shader on the
EGL default device (Mesa llvmpipe on a machine without a GPU). The variant names FNLfloat's type."""
import os
import struct
import sys
from array import array

import moderngl

if len(sys.argv) != 4:
    sys.exit("usage: driver.py KERNEL.comp float|double VECTORS_DIR")
kernel, variant, vdir = sys.argv[1:]
code = "d" if variant == "double" else "f"  # array typecode of FNLfloat


def rows(name):
    return [line.split() for line in open(os.path.join(vdir, name)) if line.strip()]


p2, p3, configs = rows("points2d.txt"), rows("points3d.txt"), rows("configs.txt")
n = max(len(p2), len(p3))
points = array(code)  # per point: x y (2D), x y z (3D); the shorter list is padded with zeros
for i in range(n):
    points.extend(map(float, (p2[i] if i < len(p2) else [0, 0]) + (p3[i] if i < len(p3) else [0, 0, 0])))
types = "ifiiiiffffiifif"  # the Config struct in driver.hlsl
cfg = b"".join(struct.pack("<" + types, *(int(v) if t == "i" else float(v) for t, v in zip(types, c)))
               for c in configs)

ctx = moderngl.create_standalone_context(backend="egl", require=430)
shader = ctx.compute_shader(open(kernel).read())
instances = len(configs) + 1
buffers = [ctx.buffer(cfg or bytes(60)), ctx.buffer(points.tobytes()),
           ctx.buffer(reserve=instances * n * 7 * points.itemsize)]
for i, b in enumerate(buffers):
    b.bind_to_storage_buffer(i)
shader.run(n, instances)
ctx.memory_barrier()  # GL requires a barrier before reading back buffers that a shader wrote
res = array(code, buffers[2].read())

out = []
for c in range(instances):
    r = lambda i, k: f"{res[(c * n + i) * 7 + k]:.17g}"
    out += [f"N2 {c - 1} {i} {r(i, 0)}" for i in range(len(p2))]
    out += [f"N3 {c - 1} {i} {r(i, 1)}" for i in range(len(p3))]
    out += [f"W2 {c - 1} {i} {r(i, 2)} {r(i, 3)}" for i in range(len(p2))]
    out += [f"W3 {c - 1} {i} {r(i, 4)} {r(i, 5)} {r(i, 6)}" for i in range(len(p3))]
sys.stdout.write("\n".join(out) + "\n")

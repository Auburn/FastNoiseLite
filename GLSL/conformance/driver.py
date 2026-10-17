#!/usr/bin/env python3
"""FastNoiseLite conformance driver (GLSL). See conformance/README.md.
Usage: python3 driver.py VECTORS_DIR > output.txt
       python3 driver.py --build    (only compile the shader; run.sh uses this as its build step)

Runs ../FastNoiseLite.glsl in an OpenGL 4.3 compute shader (headless EGL, e.g. Mesa llvmpipe)
through moderngl: one invocation per (instance, point index), results read back from an SSBO.
"""
import os
import struct
import sys
from array import array

import moderngl

HERE = os.path.dirname(os.path.abspath(__file__))

# Config fields in configs.txt order, with their GLSL types. In std430 the Config struct below is
# 15 packed 4-byte scalars (stride 60, no padding), the same layout struct.pack produces.
# The port's FNL_* enum #defines have the C++ ordinals, so enum fields are passed through unchanged.
FIELDS = [("seed", "int"), ("frequency", "float"), ("noise_type", "int"), ("rotation_type_3d", "int"),
          ("fractal_type", "int"), ("octaves", "int"), ("lacunarity", "float"), ("gain", "float"),
          ("weighted_strength", "float"), ("ping_pong_strength", "float"), ("cellular_distance_func", "int"),
          ("cellular_return_type", "int"), ("cellular_jitter_mod", "float"), ("domain_warp_type", "int"),
          ("domain_warp_amp", "float")]

# Instance c = 0 is the default instance, c > 0 is line c - 1 of configs.txt. Output slot per
# (c, i): N2, W2 x y, N3, W3 x y z. The library needs GLSL >= 4.20 (it uses {} array initializers).
KERNEL = """
layout(local_size_x = 1) in;
struct Config { %s };
layout(std430, binding = 0) readonly buffer Configs { Config configs[]; };
layout(std430, binding = 1) readonly buffer Points2 { float p2[]; };
layout(std430, binding = 2) readonly buffer Points3 { float p3[]; };
layout(std430, binding = 3) writeonly buffer Results { float res[]; };
uniform int n2;
uniform int n3;

void main()
{
    int c = int(gl_WorkGroupID.y), i = int(gl_WorkGroupID.x);
    int o = (c * int(gl_NumWorkGroups.x) + i) * 7;
    // The port has no zero-argument fnlCreateState(); 1337 is its documented default seed.
    fnl_state s = fnlCreateState(1337);
    if (c > 0)
    {
        Config f = configs[c - 1];
        %s
    }
    if (i < n2)
    {
        float x = p2[2 * i], y = p2[2 * i + 1];
        res[o] = fnlGetNoise2D(s, x, y);
        fnlDomainWarp2D(s, x, y);
        res[o + 1] = x;
        res[o + 2] = y;
    }
    if (i < n3)
    {
        float x = p3[3 * i], y = p3[3 * i + 1], z = p3[3 * i + 2];
        res[o + 3] = fnlGetNoise3D(s, x, y, z);
        fnlDomainWarp3D(s, x, y, z);
        res[o + 4] = x;
        res[o + 5] = y;
        res[o + 6] = z;
    }
}
""" % (" ".join(f"{t} {n};" for n, t in FIELDS), "\n        ".join(f"s.{n} = f.{n};" for n, _ in FIELDS))


def build():
    with open(os.path.join(HERE, "..", "FastNoiseLite.glsl")) as f:
        lib = f.read()
    ctx = moderngl.create_standalone_context(backend="egl", require=430)
    print("GL_RENDERER:", ctx.info["GL_RENDERER"], file=sys.stderr)
    return ctx, ctx.compute_shader("#version 430\n" + lib + "\n" + KERNEL)


def rows(d, name):
    with open(os.path.join(d, name)) as f:
        return [line.split() for line in f if line.strip()]


def main():
    if len(sys.argv) != 2:
        print(__doc__, file=sys.stderr)
        return 2
    ctx, cs = build()
    if sys.argv[1] == "--build":
        return 0
    d = sys.argv[1]
    p2 = [float(v) for r in rows(d, "points2d.txt") for v in r[:2]]
    p3 = [float(v) for r in rows(d, "points3d.txt") for v in r[:3]]
    n2, n3 = len(p2) // 2, len(p3) // 3
    configs = rows(d, "configs.txt")
    fmt = "<" + "".join("i" if t == "int" else "f" for _, t in FIELDS)
    cfg = b"".join(struct.pack(fmt, *(int(v) if t == "int" else float(v) for v, (_, t) in zip(r, FIELDS)))
                   for r in configs)
    nc, npts = len(configs) + 1, max(n2, n3)

    bufs = [ctx.buffer(cfg), ctx.buffer(array("f", p2).tobytes()),
            ctx.buffer(array("f", p3).tobytes()), ctx.buffer(reserve=nc * npts * 7 * 4)]
    for binding, b in enumerate(bufs):
        b.bind_to_storage_buffer(binding)
    cs["n2"], cs["n3"] = n2, n3
    cs.run(group_x=npts, group_y=nc)
    ctx.memory_barrier()  # GL requires a barrier before reading back buffers that a shader wrote
    res = array("f", bufs[3].read())

    out = []
    for c in range(nc):
        slot = [res[(c * npts + i) * 7:(c * npts + i + 1) * 7] for i in range(npts)]
        out += [f"N2 {c - 1} {i} {slot[i][0]:.9g}" for i in range(n2)]
        out += [f"N3 {c - 1} {i} {slot[i][3]:.9g}" for i in range(n3)]
        out += [f"W2 {c - 1} {i} {slot[i][1]:.9g} {slot[i][2]:.9g}" for i in range(n2)]
        out += [f"W3 {c - 1} {i} {slot[i][4]:.9g} {slot[i][5]:.9g} {slot[i][6]:.9g}" for i in range(n3)]
    sys.stdout.write("\n".join(out) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())

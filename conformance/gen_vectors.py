#!/usr/bin/env python3
"""Generate the conformance test vectors in vectors/<set>/.

Each set has:
  configs.txt   one config per line, 15 whitespace-separated fields (see README.md)
  points2d.txt  one "x y" per line
  points3d.txt  one "x y z" per line
Every coordinate is exactly representable in float32, so float and double ports
read identical inputs.

Changing the vectors changes the reference output: run
`python3 conformance/run.py Cpp --update-reference` afterwards and commit reference.sha256.
"""
import os
import struct

HERE = os.path.dirname(os.path.abspath(__file__))
INT_MAX = 2147483647
INT_MIN = -2147483648


def cfg(out, seed=1337, freq=0.01, nt=0, rot=0, ft=0, octaves=3, lac=2.0, gain=0.5,
        weighted=0.0, pingpong=2.0, cdist=1, cret=1, jitter=1.0, wt=0, amp=1.0):
    out.append((seed, freq, nt, rot, ft, octaves, lac, gain, weighted,
                pingpong, cdist, cret, jitter, wt, amp))


def main_set():
    """Broad coverage: every noise type, fractal, rotation, cellular mode and warp mode."""
    seeds = [1337, 0, -1, INT_MAX, INT_MIN, 42, 987654321, -123456789]
    c = []
    # Every noise type x fractal (None/FBm/Ridged/PingPong) x 3D rotation
    i = 0
    for nt in range(6):
        for ft in range(4):
            for rot in range(3):
                cfg(c, seed=seeds[i % len(seeds)], freq=[0.01, 0.05, 0.0137][i % 3],
                    nt=nt, rot=rot, ft=ft, octaves=[3, 5, 4][i % 3],
                    lac=[2.0, 1.7, 2.5][i % 3], gain=[0.5, 0.65, 0.4][i % 3],
                    weighted=[0.0, 0.5, 0.25, 1.0][i % 4], pingpong=[2.0, 1.5, 3.0][i % 3])
                i += 1
    # Cellular distance function x return type
    i = 0
    for cdist in range(4):
        for cret in range(7):
            cfg(c, seed=seeds[(i + 3) % len(seeds)], freq=0.02, nt=2, rot=i % 3,
                ft=1 if i % 3 == 0 else 0, cdist=cdist, cret=cret, jitter=[1.0, 0.45, 1.5][i % 3])
            i += 1
    cfg(c, nt=2, freq=0.02, jitter=0.0, cdist=0, cret=0)
    cfg(c, nt=2, freq=0.02, jitter=0.0, cdist=0, cret=2)
    # Frequency 1.0 (exact lattice and half-integer coordinates)
    for nt in range(6):
        cfg(c, seed=7, freq=1.0, nt=nt)
    cfg(c, seed=7, freq=1.0, nt=2, cdist=2, cret=0)
    cfg(c, seed=7, freq=1.0, nt=2, cdist=3, cret=6)
    # Domain warp type x warp fractal (single/progressive/independent) x rotation
    i = 0
    for wt in range(3):
        for ft in (0, 4, 5):
            for rot in range(3):
                cfg(c, seed=seeds[(i + 5) % len(seeds)], freq=[0.01, 0.03][i % 2], rot=rot,
                    ft=ft, wt=wt, amp=[30.0, 5.5, -10.0][i % 3], octaves=[3, 4, 2][i % 3],
                    lac=[2.0, 2.2][i % 2], gain=[0.5, 0.6][i % 2], nt=[0, 1, 3][i % 3])
                i += 1
    cfg(c, ft=1, wt=0, amp=20.0, octaves=5, gain=0.7)
    cfg(c, ft=2, wt=2, amp=20.0, octaves=2, gain=0.3, rot=1)
    # Edge-case parameters
    cfg(c, nt=0, ft=1, octaves=1)
    cfg(c, nt=1, ft=1, octaves=8, gain=0.6)
    cfg(c, nt=3, ft=1, gain=0.0)
    cfg(c, nt=3, ft=2, gain=1.0, octaves=4)
    cfg(c, nt=4, ft=1, lac=0.5)
    cfg(c, nt=5, ft=3, pingpong=0.5, weighted=1.0)
    cfg(c, nt=0, freq=-0.02)
    cfg(c, nt=2, freq=-0.02, cret=0)
    cfg(c, nt=1, freq=0.0)
    cfg(c, nt=0, ft=4)
    cfg(c, nt=2, ft=5, rot=2)
    cfg(c, nt=0, ft=0, wt=1, amp=0.0)
    cfg(c, nt=0, ft=4, wt=1, amp=15.0, octaves=1)
    cfg(c, nt=5, ft=1, weighted=2.0, gain=0.8)
    # Extreme seeds with every fractal and warp fractal (seed++ must wrap at 32 bits)
    for s in (INT_MAX, INT_MIN, -1):
        for nt in range(6):
            cfg(c, seed=s, nt=nt, ft=1 + (nt % 3), octaves=4, freq=0.02, rot=nt % 3)
        cfg(c, seed=s, ft=4, wt=0, amp=25.0, octaves=4)
        cfg(c, seed=s, ft=5, wt=2, amp=25.0, octaves=4, rot=2)
        cfg(c, seed=s, ft=4, wt=1, amp=25.0, octaves=4, rot=1)

    p2 = [
        (0, 0), (0.5, 0.5), (1, 1), (-1, -1), (-0.5, 0.5), (0.5, -0.5), (-0.5, -0.5),
        (1.5, 2.5), (-1.5, -2.5), (2.5, -3.5), (10, 20), (-10, -20), (100, 100),
        (-100, 37.5), (37.25, -12.75), (123.015625, 456.984375), (-321.5, 654.25),
        (1000, -1000), (-2048.5, 1024.25), (4096.125, 4095.875), (12345.671875, -9876.546875),
        (-65536.5, 65535.25), (100000.015625, 99999.984375), (0.015625, -0.015625),
        (7.75, 3.25), (-7.25, -3.75), (250.5, 250.5), (-250.5, -250.5),
        (31.96875, -48.03125), (-0.984375, 0.984375), (512, 0), (0, -512),
        (63.5, -63.5), (1.25, -1.25), (-17.375, 29.625), (88.875, -0.125),
    ]
    p3 = [
        (0, 0, 0), (0.5, 0.5, 0.5), (1, 1, 1), (-1, -1, -1), (-0.5, 0.5, -0.5),
        (0.5, -0.5, 0.5), (-0.5, -0.5, -0.5), (1.5, 2.5, -3.5), (-1.5, -2.5, 3.5),
        (10, 20, 30), (-10, -20, -30), (100, 100, 100), (-100, 37.5, 12.25),
        (37.25, -12.75, 99.5), (123.015625, 456.984375, -789.5), (-321.5, 654.25, 0.75),
        (1000, -1000, 500), (-2048.5, 1024.25, -512.75), (4096.125, 4095.875, -4096.5),
        (12345.671875, -9876.546875, 5555.5), (-65536.5, 65535.25, 1.5),
        (100000.015625, 99999.984375, -100000.5), (0.015625, -0.015625, 0.015625),
        (7.75, 3.25, -5.5), (-7.25, -3.75, 5.25), (250.5, 250.5, 250.5),
        (-250.5, -250.5, -250.5), (31.96875, -48.03125, 64.015625), (-0.984375, 0.984375, -0.984375),
        (512, 0, 0), (0, -512, 0), (0, 0, 512), (63.5, -63.5, 63.5), (1.25, -1.25, 1.25),
        (-17.375, 29.625, -41.875), (88.875, -0.125, 7.0625),
    ]
    # Points with |coord| >= 4096, where float vs double coordinates alone change results
    sensitive = "2d 18 19 20 21 22\n3d 18 19 20 21\n"
    return c, p2, p3, sensitive


def edge_set():
    """Exact-arithmetic edge cases at frequency 1: rounding ties at negative half-integers
    (with jitter > 1 the chosen cell matters) and lattice indices large enough that a
    non-wrapping 32-bit multiply loses bits. Only noise types whose results do not depend
    on float vs double coordinates are used, so every correct port must match closely."""
    c = []
    for cdist in range(4):
        for cret in (0, 1, 3):
            for jit in (1.0, 1.5, 2.0):
                cfg(c, seed=[1337, -1, INT_MAX][cret % 3], freq=1.0, nt=2, cdist=cdist,
                    cret=cret, jitter=jit, wt=2)
    for nt in (3, 4, 5):
        cfg(c, seed=1337, freq=1.0, nt=nt, wt=2)
    p2 = [
        (-0.5, -0.5), (-2.5, 3.5), (2.5, -3.5), (-7.5, -7.5), (10.5, -10.5), (-1, -1),
        (-3, 4), (0.5, 7999999.5), (-0.5, -7999999.5), (8000000.5, -1.5), (-8000000.5, 3.5),
        (5000000.5, -6000000.5),
    ]
    p3 = [
        (-0.5, -0.5, -0.5), (-2.5, 3.5, -4.5), (1.5, -2.5, 3.5), (-1, -1, -1),
        (0.5, 0.5, 6000000.5), (-0.5, -0.5, -6000000.5), (7.5, 7999999.5, -3.5),
        (8000000.5, 0.5, -0.5), (-2.5, -5000000.5, 6000000.5), (3.5, -7.5, -5500000.5),
    ]
    # Coordinates stay below 2^23 so cell indices and half-integers are exact in float32.
    # The large points are precision-sensitive only for the default instance (OpenSimplex2).
    sensitive = "2d config=-1 7 8 9 10 11\n3d config=-1 4 5 6 7 8 9\n"
    return c, p2, p3, sensitive


def fmt(v):
    return str(v) if isinstance(v, int) else repr(float(v))


def write(name, configs, p2, p3, sensitive):
    d = os.path.join(HERE, "vectors", name)
    os.makedirs(d, exist_ok=True)
    for p in p2 + p3:
        for v in p:
            assert abs(v) < 2 ** 23 or v == int(v), p
            assert struct.unpack("f", struct.pack("f", v))[0] == v, f"{p} is not exact in float32"
    with open(os.path.join(d, "configs.txt"), "w") as f:
        f.writelines(" ".join(fmt(v) for v in c) + "\n" for c in configs)
    with open(os.path.join(d, "points2d.txt"), "w") as f:
        f.writelines(" ".join(fmt(float(v)) for v in p) + "\n" for p in p2)
    with open(os.path.join(d, "points3d.txt"), "w") as f:
        f.writelines(" ".join(fmt(float(v)) for v in p) + "\n" for p in p3)
    if sensitive:
        with open(os.path.join(d, "precision_sensitive.txt"), "w") as f:
            f.write(sensitive)
    print(f"{name}: {len(configs)} configs, {len(p2)} 2D points, {len(p3)} 3D points")


if __name__ == "__main__":
    write("main", *main_set())
    write("edge", *edge_set())

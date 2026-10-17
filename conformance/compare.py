"""Compare a port's conformance output with the C++ reference output. Used by run.py.

Each output line is "N2 cfg pt v", "N3 cfg pt v", "W2 cfg pt x y" or "W3 cfg pt x y z".
Every line is compared with each reference file (float and double C++ builds) and the
closest one counts, so float and double ports are judged against the matching build.

Each value is compared with the reference value b. Its tier is the first one whose bound the
difference d does not exceed:

  bound = absolute + relative * scale + one float32 step at b

where scale is max(1, |b|) for a noise value and max(1, |b - input coordinate|) for a warped
coordinate, so a warp is judged by how far it moved the point, not by how large the coordinate
is. The float32 step allows for the output's own rounding.

  Tier      absolute  relative          meaning
            noise  warp  noise  warp
  EXACT     0      0     1e-6   1e-6
  TOL       2e-4   2e-4  0      2e-5  float rounding
  SMALL     0.02   0.02  0      1e-4  the size of difference float vs double arithmetic alone
                                      produces; at points in precision_sensitive.txt the absolute
                                      bound is 0.05 (noise) or 0.15 (warp). Fine unless systematic.
  MISMATCH  anything larger, or a non-finite value where the reference is finite (or the
            reverse). Almost always a logic difference from C++.

A line takes the worst tier of its values. An output's bad count = MISMATCH lines + missing
lines + unparsable or duplicate lines (+ SMALL lines at ordinary points when there are more than
40 of them, which means the difference is systematic rather than rounding).
"""
import enum
import math
import os
from collections import Counter, namedtuple
from dataclasses import dataclass

KINDS = ("N2", "N3", "W2", "W3")
NT = ["OpenSimplex2", "OpenSimplex2S", "Cellular", "Perlin", "ValueCubic", "Value"]
ROT = ["None", "ImproveXYPlanes", "ImproveXZPlanes"]
FT = ["None", "FBm", "Ridged", "PingPong", "DomainWarpProgressive", "DomainWarpIndependent"]
CD = ["Euclidean", "EuclideanSq", "Manhattan", "Hybrid"]
CR = ["CellValue", "Distance", "Distance2", "Distance2Add", "Distance2Sub", "Distance2Mul", "Distance2Div"]
WT = ["OpenSimplex2", "OpenSimplex2Reduced", "BasicGrid"]
SYSTEMATIC_SMALL = 40


class Tier(enum.IntEnum):
    EXACT = 0
    TOL = 1
    SMALL = 2
    MISMATCH = 3


Mismatch = namedtuple("Mismatch", "key got expected diff")


@dataclass
class Result:
    counts: dict  # line kind -> Counter of Tier
    mismatches: list  # of Mismatch
    missing: list  # of (kind, cfg, pt)
    unparsable: int  # unparsable or duplicate lines
    ordinary_small: int  # SMALL lines at points not in precision_sensitive.txt
    total: int

    @property
    def systematic_small(self):
        return self.ordinary_small if self.ordinary_small > SYSTEMATIC_SMALL else 0

    @property
    def bad(self):
        return len(self.mismatches) + len(self.missing) + self.unparsable + self.systematic_small


def load(path):
    """Return {(kind, cfg, pt): [values]} and the number of unparsable or duplicate lines."""
    data, bad = {}, 0
    with open(path, errors="replace") as f:
        for line in f:
            parts = line.split()
            if not parts or parts[0] not in KINDS:
                continue
            try:
                key = (parts[0], int(parts[1]), int(parts[2]))
                values = [float(v) for v in parts[3:]]
            except (ValueError, IndexError):
                bad += 1
                continue
            if key in data:
                bad += 1
            else:
                data[key] = values
    return data, bad


def load_points(vectors_dir):
    """The input points: {"2": [(x, y), ...], "3": [(x, y, z), ...]}."""
    points = {}
    for dim in "23":
        with open(os.path.join(vectors_dir, f"points{dim}d.txt")) as f:
            points[dim] = [tuple(map(float, line.split())) for line in f if line.strip()]
    return points


def load_sensitive(vectors_dir):
    """Precision-sensitive points from precision_sensitive.txt, as a set of (dim, config, point).

    Each line is "2d|3d [config=N] point...": the points where float vs double coordinates
    alone move results, for every config (config None) or only for config N.
    """
    rules = set()
    path = os.path.join(vectors_dir, "precision_sensitive.txt")
    if os.path.exists(path):
        for line in open(path):
            parts = line.split()
            if not parts or parts[0] not in ("2d", "3d"):
                continue
            cfg = None
            if len(parts) > 1 and parts[1].startswith("config="):
                cfg = int(parts[1][len("config="):])
                parts = parts[:1] + parts[2:]
            rules.update((parts[0][0], cfg, int(p)) for p in parts[1:])
    return rules


def describe(vectors_dir, cfg):
    if cfg == -1:
        return "default instance (no setters called)"
    try:
        lines = [l for l in open(os.path.join(vectors_dir, "configs.txt")) if l.strip()]
        s, fr, nt, rot, ft, oc, lac, g, ws, pp, cd, cr, jit, wt, amp = lines[cfg].split()
    except (IndexError, ValueError, OSError):
        return f"config {cfg}"
    return (f"seed={s} freq={fr} {NT[int(nt)]} rot={ROT[int(rot)]} fractal={FT[int(ft)]} octaves={oc} "
            f"lac={lac} gain={g} weighted={ws} pingpong={pp} cellular={CD[int(cd)]}/{CR[int(cr)]} "
            f"jitter={jit} warp={WT[int(wt)]} amp={amp}")


def bounds(kind, sensitive):
    """(tier, absolute, relative) bound of each tier below MISMATCH, for a line kind at a point."""
    if kind[0] == "N":
        return [(Tier.EXACT, 0.0, 1e-6), (Tier.TOL, 2e-4, 0.0), (Tier.SMALL, 0.05 if sensitive else 0.02, 0.0)]
    return [(Tier.EXACT, 0.0, 1e-6), (Tier.TOL, 2e-4, 2e-5), (Tier.SMALL, 0.15 if sensitive else 0.02, 1e-4)]


def float32_step(v):
    """The distance between adjacent float32 values at the finite value v."""
    return math.ldexp(1.0, math.frexp(v)[1] - 24)


def value_tier(a, b, origin, tier_bounds):
    """(tier, difference) of output value a against reference value b.

    origin is the input coordinate for a warped coordinate and 0 for a noise value.
    """
    if not (math.isfinite(a) and math.isfinite(b)):
        same = a == b or (math.isnan(a) and math.isnan(b))
        return (Tier.EXACT, 0.0) if same else (Tier.MISMATCH, math.inf)
    d, scale = abs(a - b), max(1.0, abs(b - origin))
    for tier, absolute, relative in tier_bounds:
        if d <= absolute + relative * scale + float32_step(b):
            return tier, d
    return Tier.MISMATCH, d


def line_tier(values, ref_values, origin, tier_bounds):
    """(tier, difference) of the worst value of an output line against a reference line."""
    if len(values) != len(ref_values):
        return Tier.MISMATCH, math.inf
    return max(value_tier(a, b, o, tier_bounds) for a, b, o in zip(values, ref_values, origin))


def compare(output_path, vectors_dir, reference_paths):
    """Compare one output file with the reference files. Returns a Result."""
    port, unparsable = load(output_path)
    refs = [load(p)[0] for p in reference_paths]
    points = load_points(vectors_dir)
    sensitive = load_sensitive(vectors_dir)
    keys = sorted(set().union(*refs))
    counts = {k: Counter() for k in KINDS}
    mismatches, missing, ordinary_small = [], [], 0
    for key in keys:
        if key not in port:
            missing.append(key)
            continue
        kind, cfg, pt = key
        is_sensitive = (kind[1], None, pt) in sensitive or (kind[1], cfg, pt) in sensitive
        tier_bounds = bounds(kind, is_sensitive)
        origin = points[kind[1]][pt] if kind[0] == "W" else (0.0,)
        (tier, d), expected = min(((line_tier(port[key], r[key], origin, tier_bounds), r[key]) for r in refs),
                                  key=lambda t: t[0])
        counts[kind][tier] += 1
        if tier is Tier.MISMATCH:
            mismatches.append(Mismatch(key, port[key], expected, d))
        elif tier is Tier.SMALL and not is_sensitive:
            ordinary_small += 1
    return Result(counts, mismatches, missing, unparsable, ordinary_small, len(keys))


def report(res, vectors_dir, show, indent):
    """Human-readable summary of a compare() result."""
    out = []
    for k in KINDS:
        out.append(f"{indent}{k}: " + " ".join(f"{t.name}={res.counts[k][t]}" for t in Tier))
    out.append(f"{indent}missing={len(res.missing)} unparsable-or-duplicate={res.unparsable} "
               f"systematic-small={res.systematic_small} -> bad={res.bad} of {res.total}")
    if res.missing and len(res.missing) < res.total:
        out.append(f"{indent}first missing: {res.missing[:6]}")
    by_cfg = Counter(m.key[1] for m in res.mismatches)
    for cfg, n in by_cfg.most_common(show and 8):
        out.append(f"{indent}  config {cfg}: {n} mismatches :: {describe(vectors_dir, cfg)}")
    for m in sorted(res.mismatches)[:show]:
        out.append(f"{indent}  {m.key[0]} config {m.key[1]} point {m.key[2]}: got {m.got} expected {m.expected} "
                   f"(diff {m.diff:.3g})")
    return "\n".join(out)

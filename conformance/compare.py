#!/usr/bin/env python3
"""Compare a port's conformance output with the C++ reference output.

usage: compare.py OUTPUT VECTORS_DIR REFERENCE... [--show N]

Each output line is "N2 cfg pt v", "N3 cfg pt v", "W2 cfg pt x y" or "W3 cfg pt x y z".
Every line is compared with each reference file (float and double C++ builds) and the
closest one counts, so float and double ports are judged against the matching build.

Tiers, per value (d = absolute difference, r = |reference value|; a line takes the worst
tier of its values, so each warp coordinate is judged on its own scale):
  EXACT     d <= 1e-6 * max(1, r)
  TOL       noise: d <= 2e-4        warp: d <= 2e-4 + 2e-5 * max(1, r)
  SMALL     noise: d <= 0.02        warp: d <= 0.02 + 1e-4 * max(1, r)
            (0.05 / 0.15 at points listed in precision_sensitive.txt). This is the size of
            difference float vs double arithmetic alone produces. Fine unless systematic.
  MISMATCH  anything larger, a non-finite value where the reference is finite, or a
            missing line. Almost always a logic difference from C++.

A port's "bad" count = MISMATCH + missing + unparsable or duplicate lines (+ SMALL values
at ordinary points when there are more than 40 of them, which means the difference is
systematic rather than rounding).
"""
import math
import os
import sys
from collections import Counter

KINDS = ("N2", "N3", "W2", "W3")
NT = ["OpenSimplex2", "OpenSimplex2S", "Cellular", "Perlin", "ValueCubic", "Value"]
ROT = ["None", "ImproveXYPlanes", "ImproveXZPlanes"]
FT = ["None", "FBm", "Ridged", "PingPong", "DomainWarpProgressive", "DomainWarpIndependent"]
CD = ["Euclidean", "EuclideanSq", "Manhattan", "Hybrid"]
CR = ["CellValue", "Distance", "Distance2", "Distance2Add", "Distance2Sub", "Distance2Mul", "Distance2Div"]
WT = ["OpenSimplex2", "OpenSimplex2Reduced", "BasicGrid"]
SYSTEMATIC_SMALL = 40


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


def load_sensitive(vectors_dir):
    """Precision-sensitive points from precision_sensitive.txt.

    Each line is "2d|3d [config=N] point...": the points where float vs double coordinates
    alone move results, for every config or only for config N. Returns a predicate.
    """
    rules = []
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
            rules.append((parts[0][0], cfg, {int(p) for p in parts[1:]}))
    return lambda kind, c, pt: any(kind[1] == dim and cfg in (None, c) and pt in pts for dim, cfg, pts in rules)


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


def value_tier(kind, sensitive, a, b):
    """Tier of one output value a against reference value b."""
    if not (math.isfinite(a) and math.isfinite(b)):
        return 0 if a == b or (math.isnan(a) and math.isnan(b)) else 3
    d, r = abs(a - b), max(1.0, abs(b))
    if d <= 1e-6 * r:
        return 0
    if kind[0] == "N":
        if d <= 2e-4:
            return 1
        return 2 if d <= (0.05 if sensitive else 0.02) else 3
    if d <= 2e-4 + 2e-5 * r:
        return 1
    return 2 if d <= (0.15 if sensitive else 0.02 + 1e-4 * r) else 3


def line_tier(kind, sensitive, pv, rv):
    """(tier, largest difference) of one output line against one reference line."""
    if len(pv) != len(rv):
        return 3, math.inf
    t = max(value_tier(kind, sensitive, a, b) for a, b in zip(pv, rv))
    d = max((abs(a - b) if math.isfinite(a) and math.isfinite(b) else (0.0 if a == b else math.inf)
             for a, b in zip(pv, rv)), default=0.0)
    return t, d


def compare(output_path, vectors_dir, reference_paths):
    """Compare one output file. Returns a dict with tier counts, the bad count and details."""
    port, unparsable = load(output_path) if os.path.exists(output_path) else ({}, 0)
    refs = [load(p)[0] for p in reference_paths]
    sensitive = load_sensitive(vectors_dir)
    keys = sorted(set().union(*[r.keys() for r in refs]))
    counts = {k: [0, 0, 0, 0] for k in KINDS}
    missing, mismatches, ordinary_small = [], [], []
    for key in keys:
        if key not in port:
            missing.append(key)
            continue
        best = None
        is_sensitive = sensitive(*key)
        for r in refs:
            if key in r:
                t, d = line_tier(key[0], is_sensitive, port[key], r[key])
                if best is None or (t, d) < best[:2]:
                    best = (t, d, r[key])
        counts[key[0]][best[0]] += 1
        if best[0] == 3:
            mismatches.append((key, port[key], best[2], best[1]))
        elif best[0] == 2 and not is_sensitive:
            ordinary_small.append(key)
    systematic = len(ordinary_small) > SYSTEMATIC_SMALL
    return {
        "counts": counts,
        "mismatches": mismatches,
        "missing": missing,
        "unparsable": unparsable,
        "systematic_small": len(ordinary_small) if systematic else 0,
        "bad": len(mismatches) + len(missing) + unparsable + (len(ordinary_small) if systematic else 0),
        "total": len(keys),
    }


def report(res, vectors_dir, show=20, indent="  "):
    """Human-readable summary of a compare() result."""
    out = []
    for k in KINDS:
        e, t, s, m = res["counts"][k]
        out.append(f"{indent}{k}: exact={e} tol={t} small={s} MISMATCH={m}")
    out.append(f"{indent}missing={len(res['missing'])} unparsable-or-duplicate={res['unparsable']} "
               f"systematic-small={res['systematic_small']} -> bad={res['bad']} of {res['total']}")
    if res["missing"] and len(res["missing"]) < res["total"]:
        out.append(f"{indent}first missing: {res['missing'][:6]}")
    by_cfg = Counter(m[0][1] for m in res["mismatches"])
    for cfg, n in by_cfg.most_common(show and 8):
        out.append(f"{indent}  config {cfg}: {n} mismatches :: {describe(vectors_dir, cfg)}")
    for key, pv, rv, d in sorted(res["mismatches"])[:show]:
        out.append(f"{indent}  {key[0]} config {key[1]} point {key[2]}: got {pv} expected {rv} (diff {d:.3g})")
    return "\n".join(out)


def main():
    args = sys.argv[1:]
    show = 20
    if "--show" in args:
        i = args.index("--show")
        show = int(args[i + 1])
        del args[i:i + 2]
    if len(args) < 3:
        print(__doc__)
        return 2
    res = compare(args[0], args[1], args[2:])
    print(report(res, args[1], show))
    return 0 if res["bad"] == 0 else 1


if __name__ == "__main__":
    sys.exit(main())

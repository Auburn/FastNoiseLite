#!/usr/bin/env python3
"""Run the FastNoiseLite conformance suite. See README.md.

usage: run.py [PORT ...] [--show N] [--update-baseline] [--update-reference]

  PORT                 port folder names (e.g. C Rust GLSL). Default: every folder that has
                       conformance/run.sh. "Cpp" alone only checks the reference.
  --show N             print up to N mismatching values per failing output (default 10)
  --update-baseline    write the current bad counts of the ports run into baseline.json
  --update-reference   accept a changed C++ output and rewrite reference.sha256

Steps: build and run the C++ driver to produce the reference output (and check it against
reference.sha256), run each port's conformance/run.sh, compare every output file with the
reference, and compare each bad count with baseline.json. Exits 1 if anything got worse.
"""
import hashlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
import time

import compare

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
OUT = os.path.join(HERE, "out")
VECTORS = sorted(os.path.join(HERE, "vectors", d) for d in os.listdir(os.path.join(HERE, "vectors")))
BASELINE = os.path.join(HERE, "baseline.json")
REFERENCE_HASHES = os.path.join(HERE, "reference.sha256")
GITHUB = os.environ.get("GITHUB_ACTIONS") == "true"


def annotate(level, msg):
    if GITHUB:
        print(f"::{level}::{msg}")


def run_port(name, out):
    """Run <name>/conformance/run.sh into the empty directory out. Returns (ok, seconds, output files)."""
    shutil.rmtree(out, ignore_errors=True)
    os.makedirs(out)
    script = os.path.join(ROOT, name, "conformance", "run.sh")
    start = time.time()
    with open(os.path.join(out, "log.txt"), "w") as log:
        rc = subprocess.call(["bash", script, out] + VECTORS, stdout=log, stderr=subprocess.STDOUT, cwd=out)
    files = sorted(f for f in os.listdir(out) if f.endswith(".txt") and f != "log.txt")
    return rc == 0, time.time() - start, files


def tail(path, n=15):
    try:
        return "".join(open(path, errors="replace").readlines()[-n:])
    except OSError:
        return ""


def check_reference(ref, update):
    ok, secs, files = run_port("Cpp", ref)
    if not ok:
        print("C++ reference failed to build:\n" + tail(os.path.join(ref, "log.txt")))
        return False
    hashes = {f: hashlib.sha256(open(os.path.join(ref, f), "rb").read()).hexdigest() for f in files}
    if update:
        with open(REFERENCE_HASHES, "w") as fh:
            fh.writelines(f"{h}  {f}\n" for f, h in sorted(hashes.items()))
        print(f"reference.sha256 updated ({len(hashes)} files)")
        return True
    expected = {}
    if os.path.exists(REFERENCE_HASHES):
        for line in open(REFERENCE_HASHES):
            h, f = line.split()
            expected[f] = h
    if hashes != expected:
        changed = sorted(f for f in set(hashes) | set(expected) if hashes.get(f) != expected.get(f))
        msg = ("C++ reference output changed: " + ", ".join(changed) +
               ". If the change to Cpp/FastNoiseLite.h or the vectors is intended, run "
               "`python3 conformance/run.py Cpp --update-reference`, commit reference.sha256 and "
               "update the ports to match.")
        print(msg)
        annotate("error", msg)
        return False
    print(f"C++ reference: OK ({len(files)} files, {secs:.0f}s)")
    return True


def main():
    args = sys.argv[1:]
    show = 10
    if "--show" in args:
        i = args.index("--show")
        show = int(args[i + 1])
        del args[i:i + 2]
    update_baseline = "--update-baseline" in args
    update_reference = "--update-reference" in args
    ports = [a for a in args if not a.startswith("--")]
    if not ports:
        ports = sorted(d for d in os.listdir(ROOT)
                       if os.path.exists(os.path.join(ROOT, d, "conformance", "run.sh")))
    unknown = [p for p in ports if not os.path.exists(os.path.join(ROOT, p, "conformance", "run.sh"))]
    if unknown:
        print("no conformance/run.sh for: " + ", ".join(unknown))
        return 2

    # Each run builds its own copy of the reference, so several runs can share out/.
    os.makedirs(OUT, exist_ok=True)
    ref = tempfile.mkdtemp(prefix="reference-", dir=OUT)
    try:
        return run(ports, ref, show, update_baseline, update_reference)
    finally:
        shutil.rmtree(ref, ignore_errors=True)


def run(ports, ref, show, update_baseline, update_reference):
    if not check_reference(ref, update_reference):
        return 1
    baseline = json.load(open(BASELINE)) if os.path.exists(BASELINE) else {}
    sets = {os.path.basename(v): v for v in VECTORS}
    failed = False

    for name in ports:
        if name == "Cpp":
            continue
        ok, secs, files = run_port(name, os.path.join(OUT, name))
        print(f"\n{name} ({secs:.0f}s)")
        if not ok:
            failed = True
            msg = f"{name}: conformance/run.sh failed (build error?), see conformance/out/{name}/log.txt"
            print("  " + msg + "\n" + tail(os.path.join(OUT, name, "log.txt")))
            annotate("error", msg)
            continue
        seen = set()
        for f in files:
            parts = f[:-4].split(".", 1)
            if len(parts) != 2 or parts[0] not in sets:
                print(f"  ignoring {f} (expected <set>.<variant>.txt)")
                continue
            vset, variant = parts
            key = f"{name}/{vset}/{variant}"
            seen.add(key)
            refs = [os.path.join(ref, f"{vset}.{t}.txt") for t in ("float", "double")]
            res = compare.compare(os.path.join(OUT, name, f), sets[vset], refs)
            bad, allowed = res["bad"], baseline.get(key, 0)
            if update_baseline:
                status = "recorded in baseline" if bad else "pass"
            elif bad > allowed:
                status = f"REGRESSED (baseline {allowed})"
                failed = True
                annotate("error", f"{key}: {bad} bad values, baseline allows {allowed}")
            elif bad < allowed:
                status = f"IMPROVED from {allowed}: lower the baseline (run.py {name} --update-baseline)"
                annotate("notice", f"{key}: {bad} bad values, baseline {allowed}. Update baseline.json.")
            elif bad == 0:
                status = "pass"
            else:
                status = "known deviations (baseline)"
            print(f"  {vset}/{variant}: {bad} bad of {res['total']} - {status}")
            if bad > allowed and not update_baseline:
                print(compare.report(res, sets[vset], show, indent="    "))
            if update_baseline:
                if bad:
                    baseline[key] = bad
                else:
                    baseline.pop(key, None)
        for key in sorted(k for k in baseline if k.startswith(name + "/") and k not in seen):
            if update_baseline:
                del baseline[key]
            else:
                failed = True
                msg = f"{key}: listed in baseline.json but no output was produced"
                print("  " + msg)
                annotate("error", msg)
        for vset in sets:
            if not any(k.startswith(f"{name}/{vset}/") for k in seen):
                failed = True
                print(f"  no output for vector set '{vset}'")

    if update_baseline:
        with open(BASELINE, "w") as fh:
            json.dump(dict(sorted(baseline.items())), fh, indent=2)
            fh.write("\n")
        print("\nbaseline.json updated")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())

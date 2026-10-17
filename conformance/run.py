#!/usr/bin/env python3
"""Run the FastNoiseLite conformance suite. See README.md.

Steps: build and run the C++ reference (and check its output against reference.sha256), then for
each port run its conformance/build.sh, run every variant's driver on every vector set and compare
each output with the reference. Exits 1 if a build fails, a driver exits with an error or an output
has a bad value.
"""
import argparse
import collections
import hashlib
import os
import shlex
import shutil
import subprocess
import sys
import time

import compare

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
OUT = os.path.join(HERE, "out")
VECTORS = {d: os.path.join(HERE, "vectors", d) for d in sorted(os.listdir(os.path.join(HERE, "vectors")))
           if os.path.isdir(os.path.join(HERE, "vectors", d))}
REFERENCE = "Cpp"
REFERENCE_HASHES = os.path.join(HERE, "reference.sha256")
GITHUB = os.environ.get("GITHUB_ACTIONS") == "true"

Output = collections.namedtuple("Output", "set variant path status")


def annotate(level, msg):
    if GITHUB:
        print(f"::{level}::{msg}")


def build_script(port):
    return os.path.join(ROOT, port, "conformance", "build.sh")


def tail(path, n=15):
    try:
        return "".join(open(path, errors="replace").readlines()[-n:])
    except OSError:
        return ""


def build(port):
    """Run <port>/conformance/build.sh in the emptied directory out/<port>.

    Returns the directory and {variant: driver command}, or None for the variants if the build failed.
    """
    out = os.path.join(OUT, port)
    shutil.rmtree(out, ignore_errors=True)
    os.makedirs(out)
    with open(os.path.join(out, "log.txt"), "w") as log:
        failed = subprocess.call(["bash", build_script(port)], stdout=log, stderr=subprocess.STDOUT, cwd=out)
    variants = os.path.join(out, "variants.txt")
    if failed or not os.path.exists(variants):
        return out, None
    with open(variants) as f:
        return out, {name: cmd for name, *cmd in (shlex.split(line) for line in f if line.strip())}


def run_drivers(out, variants):
    """Run every variant's driver on every vector set in out, writing out/<set>.<variant>.txt.

    Driver errors go to log.txt. Returns a list of Output.
    """
    outputs = []
    with open(os.path.join(out, "log.txt"), "a") as log:
        for vset, vdir in VECTORS.items():
            for variant, cmd in variants.items():
                path = os.path.join(out, f"{vset}.{variant}.txt")
                log.write(f"\n$ {shlex.join(cmd + [vdir])} > {os.path.basename(path)}\n")
                log.flush()
                with open(path, "w") as fh:
                    try:
                        status = subprocess.call(cmd + [vdir], stdout=fh, stderr=log, cwd=out)
                    except OSError as e:
                        log.write(f"{e}\n")
                        status = 127
                outputs.append(Output(vset, variant, path, status))
    return outputs


def exit_text(status):
    return f"driver killed by signal {-status}" if status < 0 else f"driver exited with status {status}"


def reference(update):
    """Build and run the C++ reference and check its output against reference.sha256.

    Returns {vector set: [reference output paths]}, or None if the reference can't be used.
    """
    start = time.time()
    out, variants = build(REFERENCE)
    if variants is None:
        print("C++ reference failed to build:\n" + tail(os.path.join(out, "log.txt")))
        return None
    outputs = run_drivers(out, variants)
    for o in outputs:
        if o.status:
            print(f"C++ reference {o.set}/{o.variant}: {exit_text(o.status)}:\n" + tail(os.path.join(out, "log.txt")))
            return None
    hashes = {os.path.basename(o.path): hashlib.sha256(open(o.path, "rb").read()).hexdigest() for o in outputs}
    if update:
        with open(REFERENCE_HASHES, "w") as fh:
            fh.writelines(f"{h}  {f}\n" for f, h in sorted(hashes.items()))
        print(f"reference.sha256 updated ({len(hashes)} files)")
    else:
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
            return None
        print(f"C++ reference: OK ({len(outputs)} outputs, {time.time() - start:.0f}s)")
    refs = {}
    for o in outputs:
        refs.setdefault(o.set, []).append(o.path)
    return refs


def check(port, refs, show):
    """Build and run one port and compare its outputs with the reference.

    Prints a report and returns (summary table rows, whether the port passed).
    """
    start = time.time()
    out, variants = build(port)
    if variants is None:
        msg = f"{port}: conformance/build.sh failed, see conformance/out/{port}/log.txt"
        print(f"\n{port}\n  {msg}\n{tail(os.path.join(out, 'log.txt'))}")
        annotate("error", msg)
        return [(port, "build", "failed")], False
    outputs = run_drivers(out, variants)
    print(f"\n{port} ({time.time() - start:.0f}s)")
    rows, passed = [], True
    for o in outputs:
        res = compare.compare(o.path, VECTORS[o.set], refs[o.set])
        name = f"{o.set}/{o.variant}"
        result = f"{res.bad} bad of {res.total}"
        if o.status:
            result += f", {exit_text(o.status)} (see log.txt)"
        ok = res.bad == 0 and o.status == 0
        print(f"  {name}: {result} - {'pass' if ok else 'FAIL'}")
        rows.append((port, name, result))
        if not ok:
            passed = False
            annotate("error", f"{port} {name}: {result}")
            print(compare.report(res, VECTORS[o.set], show, indent="    "))
    return rows, passed


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("ports", nargs="*", metavar="PORT",
                        help='port folder names (e.g. C Rust GLSL). Default: every folder that has '
                             'conformance/build.sh. "Cpp" alone only checks the reference.')
    parser.add_argument("--show", type=int, default=10, metavar="N",
                        help="print up to N mismatching values per failing output (default 10)")
    parser.add_argument("--update-reference", action="store_true",
                        help="accept a changed C++ output and rewrite reference.sha256")
    args = parser.parse_args()
    ports = args.ports or sorted(d for d in os.listdir(ROOT) if os.path.exists(build_script(d)))
    unknown = [p for p in ports if not os.path.exists(build_script(p))]
    if unknown:
        parser.error("no conformance/build.sh for: " + ", ".join(unknown))

    refs = reference(args.update_reference)
    if refs is None:
        return 1
    rows, failed = [], False
    for port in ports:
        if port != REFERENCE:
            port_rows, passed = check(port, refs, args.show)
            rows += port_rows
            failed |= not passed

    summary = os.environ.get("GITHUB_STEP_SUMMARY")
    if summary:
        with open(summary, "a") as fh:
            fh.write("| Port | Output | Result |\n|---|---|---|\n")
            fh.writelines(f"| {port} | {output} | {result} |\n" for port, output, result in rows)
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())

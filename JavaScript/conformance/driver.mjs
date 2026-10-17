// FastNoiseLite conformance driver (JavaScript). See conformance/README.md.
// Usage: node driver.mjs VECTORS_DIR > output.txt
// Tests dist/FastNoiseLite.js, the file npm users import. JS enums are looked up by name
// because their numeric values differ from the C++ ordinals used in configs.txt.
import fs from "node:fs";
import path from "node:path";
import FastNoiseLite from "../dist/FastNoiseLite.js";

const dir = process.argv[2];
if (!dir) {
    console.error("usage: node driver.mjs VECTORS_DIR");
    process.exit(2);
}

const ENUMS = [
    ["NoiseType", ["OpenSimplex2", "OpenSimplex2S", "Cellular", "Perlin", "ValueCubic", "Value"]],
    ["RotationType3D", ["None", "ImproveXYPlanes", "ImproveXZPlanes"]],
    ["FractalType", ["None", "FBm", "Ridged", "PingPong", "DomainWarpProgressive", "DomainWarpIndependent"]],
    ["CellularDistanceFunction", ["Euclidean", "EuclideanSq", "Manhattan", "Hybrid"]],
    ["CellularReturnType", ["CellValue", "Distance", "Distance2", "Distance2Add", "Distance2Sub", "Distance2Mul", "Distance2Div"]],
    ["DomainWarpType", ["OpenSimplex2", "OpenSimplex2Reduced", "BasicGrid"]],
];
const enumValue = (i, ordinal) => FastNoiseLite[ENUMS[i][0]][ENUMS[i][1][ordinal]];

const rows = (name) => fs.readFileSync(path.join(dir, name), "utf8").split("\n")
    .map((l) => l.trim()).filter(Boolean).map((l) => l.split(/\s+/).map(Number));
const p2 = rows("points2d.txt");
const p3 = rows("points3d.txt");
const out = [];

function run(n, c) {
    p2.forEach(([x, y], i) => out.push(`N2 ${c} ${i} ${n.GetNoise(x, y)}`));
    p3.forEach(([x, y, z], i) => out.push(`N3 ${c} ${i} ${n.GetNoise(x, y, z)}`));
    p2.forEach(([x, y], i) => {
        const v = { x, y };
        n.DomainWarp(v);
        out.push(`W2 ${c} ${i} ${v.x} ${v.y}`);
    });
    p3.forEach(([x, y, z], i) => {
        const v = { x, y, z };
        n.DomainWarp(v);
        out.push(`W3 ${c} ${i} ${v.x} ${v.y} ${v.z}`);
    });
}

run(new FastNoiseLite(), -1);
rows("configs.txt").forEach(([seed, freq, nt, rot, ft, octaves, lac, gain, ws, pp, cdist, cret, jitter, wt, amp], c) => {
    const n = new FastNoiseLite();
    n.SetSeed(seed);
    n.SetFrequency(freq);
    n.SetNoiseType(enumValue(0, nt));
    n.SetRotationType3D(enumValue(1, rot));
    n.SetFractalType(enumValue(2, ft));
    n.SetFractalOctaves(octaves);
    n.SetFractalLacunarity(lac);
    n.SetFractalGain(gain);
    n.SetFractalWeightedStrength(ws);
    n.SetFractalPingPongStrength(pp);
    n.SetCellularDistanceFunction(enumValue(3, cdist));
    n.SetCellularReturnType(enumValue(4, cret));
    n.SetCellularJitter(jitter);
    n.SetDomainWarpType(enumValue(5, wt));
    n.SetDomainWarpAmp(amp);
    run(n, c);
});
process.stdout.write(out.join("\n") + "\n");

import FastNoiseLite, { CellularDistanceFunction, CellularReturnType, DomainWarpType, FractalType, NoiseType, RotationType3D, type Vector2, type Vector3 } from "./dist/FastNoiseLite.js";
import { PNG } from "pngjs";
import { createWriteStream } from "node:fs";

/*
 * This script generates an image with various types of noise.
 * Use this to check if the noise output has changed.
 */

const testCases: { noise: FastNoiseLite, warp?: FastNoiseLite }[] = [];

// basic noise types and fractal types
for (const noiseType of [
    NoiseType.OpenSimplex2,
    NoiseType.OpenSimplex2S,
    NoiseType.Perlin,
    NoiseType.Value,
    NoiseType.ValueCubic
]) {
    for (const fractalType of [
        FractalType.None,
        FractalType.FBm,
        FractalType.PingPong,
        FractalType.Ridged
    ]) {
        const noise = new FastNoiseLite();
        noise.SetDomainWarpAmp(0);
        noise.SetNoiseType(noiseType);
        noise.SetFractalType(fractalType)
        testCases.push({ noise });
    }
}

// cellular options
for (const distFunc of [
    CellularDistanceFunction.Euclidean,
    CellularDistanceFunction.EuclideanSq,
    CellularDistanceFunction.Manhattan,
    CellularDistanceFunction.Hybrid,
]) {
    for (const returnType of [
        CellularReturnType.CellValue,
        CellularReturnType.Distance,
        CellularReturnType.Distance2,
        CellularReturnType.Distance2Add,
        CellularReturnType.Distance2Div,
        CellularReturnType.Distance2Mul,
        CellularReturnType.Distance2Sub,
    ]) {
        const noise = new FastNoiseLite();
        noise.SetDomainWarpAmp(0);
        noise.SetNoiseType(NoiseType.Cellular);
        noise.SetCellularDistanceFunction(distFunc);
        noise.SetCellularReturnType(returnType);
        testCases.push({ noise });
    }
}

// domain warp
for (const warpType of [
    DomainWarpType.BasicGrid,
    DomainWarpType.OpenSimplex2,
    DomainWarpType.OpenSimplex2Reduced,
]) {
    for (const warpFractalType of [
        FractalType.None,
        FractalType.DomainWarpIndependent,
        FractalType.DomainWarpProgressive,
    ]) {
        const noise = new FastNoiseLite();
        const warp = new FastNoiseLite();
        warp.SetFrequency(0.05);
        warp.SetDomainWarpType(warpType);
        warp.SetFractalType(warpFractalType);
        warp.SetRotationType3D(RotationType3D.ImproveXYPlanes);
        warp.SetDomainWarpAmp(50);
        testCases.push({ noise, warp });
    }
}

const SCALE = 2;
const ROWS = 8;
const TEST_CASE_SIZE = 512;

const png = new PNG({
    width: TEST_CASE_SIZE * ROWS,
    height: (1 + Math.floor((2 * testCases.length) / ROWS)) * TEST_CASE_SIZE,
    filterType: -1
});

console.time("testCases");

let tcIndex = 0
const Z = 42;
const pos2 = { x: 0, y: 0 };
const pos3 = { x: 0, y: 0, z: Z };
for (const testCase of testCases) {
    for (let dims = 2; dims <= 3; dims++) {
        const yBase = Math.floor(tcIndex / ROWS) * TEST_CASE_SIZE;
        const xBase = (tcIndex % ROWS) * TEST_CASE_SIZE;
        for (let y = 0; y < TEST_CASE_SIZE; y++) {
            for (let x = 0; x < TEST_CASE_SIZE; x++) {
                const idx = (png.width * (yBase + y) + (xBase + x)) << 2;

                let v;
                if (dims === 2) {
                    pos2.x = x * SCALE;
                    pos2.y = y * SCALE;
                    if (testCase.warp) testCase.warp.DomainWrap(pos2);
                    v = testCase.noise.GetNoise(pos2.x, pos2.y);
                } else {
                    pos3.x = x * SCALE;
                    pos3.y = y * SCALE;
                    pos3.z = Z;
                    if (testCase.warp) testCase.warp.DomainWrap(pos3);
                    v = testCase.noise.GetNoise(pos3.x, pos3.y, (pos3 as Vector3).z);
                }

                let color = (v + 1) * 128;
                color = Math.max(0, Math.min(color, 255));
                png.data[idx] = color; // red
                png.data[idx + 1] = color; // green
                png.data[idx + 2] = color; // blue
                png.data[idx + 3] = 255; // alpha (0 is transparent)
            }
        }
        tcIndex += 1;
    }
}

console.timeEnd("testCases")

png.pack().pipe(createWriteStream('test-output.png'));

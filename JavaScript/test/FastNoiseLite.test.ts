import FastNoiseLite, {
    CellularDistanceFunction,
    CellularReturnType,
    DomainWarpType,
    FractalType,
    NoiseType,
    RotationType3D,
    type Vector3,
} from "../src/FastNoiseLite.ts";
import test from "node:test";
import { PNG } from "pngjs";
import { createWriteStream } from "node:fs";

function assertApproximatelyEqual(t: test.TestContext, a: number, b: number, epsilon: number = 0.00001) {
    t.assert.ok(Math.abs(a - b) <= epsilon, `Expected ${a} to be approximately equal to ${b}`);
}

test("Simple example code from readme", (t) => {
    // Create and configure FastNoiseLite object
    let noise = new FastNoiseLite();
    noise.SetNoiseType(FastNoiseLite.NoiseType.OpenSimplex2);

    const S = 4;

    // Gather noise data
    let noiseData: number[][] = [];

    for (let x = 0; x < S; x++) {
        noiseData[x] = [];

        for (let y = 0; y < S; y++) {
            noiseData[x]![y] = noise.GetNoise(x, y);
        }
    }

    const expected = [
        [0.00000, 0.04946, 0.09869, 0.14744],
        [0.03795, 0.08734, 0.13644, 0.18500],
        [0.07572, 0.12497, 0.17386, 0.22215],
        [0.11313, 0.16215, 0.21075, 0.25870],
    ]
    for (let x = 0; x < S; x++) {
        for (let y = 0; y < S; y++) {
            assertApproximatelyEqual(t, noiseData[x][y], expected[x][y]);
        }
    }
});

test("Domain warp example code from readme", (t) => {
    let noise = new FastNoiseLite();
    noise.SetDomainWarpType(FastNoiseLite.DomainWarpType.OpenSimplex2);
    noise.SetDomainWarpAmp(1.5);

    let vec = { x: 1, y: 2 };
    noise.DomainWarp(vec);

    assertApproximatelyEqual(t, vec.x, 0.98362);
    assertApproximatelyEqual(t, vec.y, 1.95825);
});

test("Create an image", () => {
    const testCases: { noise: FastNoiseLite; warp?: FastNoiseLite }[] = [];

    // basic noise types and fractal types
    for (const noiseType of [
        NoiseType.OpenSimplex2,
        NoiseType.OpenSimplex2S,
        NoiseType.Perlin,
        NoiseType.Value,
        NoiseType.ValueCubic,
    ]) {
        for (const fractalType of [
            FractalType.None,
            FractalType.FBm,
            FractalType.PingPong,
            FractalType.Ridged,
        ]) {
            const noise = new FastNoiseLite();
            noise.SetDomainWarpAmp(0);
            noise.SetNoiseType(noiseType);
            noise.SetFractalType(fractalType);
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
        filterType: -1,
    });

    console.time("benchmark");

    let tcIndex = 0;
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
                        if (testCase.warp) testCase.warp.DomainWarp(pos2);
                        v = testCase.noise.GetNoise(pos2.x, pos2.y);
                    } else {
                        pos3.x = x * SCALE;
                        pos3.y = y * SCALE;
                        pos3.z = Z;
                        if (testCase.warp) testCase.warp.DomainWarp(pos3);
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

    console.timeEnd("benchmark");

    png.pack().pipe(createWriteStream("test-output.png"));
});

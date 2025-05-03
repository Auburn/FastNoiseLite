export declare enum NoiseType {
    OpenSimplex2 = 1,
    OpenSimplex2S = 2,
    Cellular = 3,
    Perlin = 4,
    ValueCubic = 5,
    Value = 6
}
export declare enum RotationType3D {
    None = 0,
    ImproveXYPlanes = 1,
    ImproveXZPlanes = 2
}
export declare enum FractalType {
    None = 0,
    FBm = 1,
    Ridged = 2,
    PingPong = 3,
    DomainWarpProgressive = 4,
    DomainWarpIndependent = 5
}
export declare enum CellularDistanceFunction {
    Euclidean = 1,
    EuclideanSq = 2,
    Manhattan = 3,
    Hybrid = 4
}
export declare enum CellularReturnType {
    CellValue = 1,
    Distance = 2,
    Distance2 = 3,
    Distance2Add = 4,
    Distance2Sub = 5,
    Distance2Mul = 6,
    Distance2Div = 7
}
export declare enum DomainWarpType {
    OpenSimplex2 = 1,
    OpenSimplex2Reduced = 2,
    BasicGrid = 3
}
export declare enum TransformType3D {
    None = 0,
    ImproveXYPlanes = 1,
    ImproveXZPlanes = 2,
    DefaultOpenSimplex2 = 3
}
export interface Vector2 {
    x: number;
    y: number;
    z: never;
}
export interface Vector3 {
    x: number;
    y: number;
    z: number;
}
/**
 * @description FastNoise Lite is an extremely portable open source noise generation library with a large selection of noise algorithms
 * @author Jordan Peck, snowfoxsh
 * @version 1.1.1
 * @copyright Copyright(c) 2023 Jordan Peck, Contributors
 * @license MIT
 * @git https://github.com/Auburn/FastNoiseLite
 * @npm https://www.npmjs.com/package/fastnoise-lite
 * @example
// Import from npm (if you used npm)

import FastNoiseLite from "fastnoise-lite";

// Create and configure FastNoiseLite object

let noise = new FastNoiseLite();
noise.SetNoiseType(FastNoiseLite.NoiseType.OpenSimplex2);

// Gather noise data
let noiseData = [];

for (let x = 0; x < 128; x++) {
    noiseData[x] = [];

    for (let y = 0; y < 128; y++) {
        noiseData[x][y] = noise.GetNoise(x,y);
    }
}

// Do something with this data...
 */
export default class FastNoiseLite {
    static NoiseType: typeof NoiseType;
    static RotationType3D: typeof RotationType3D;
    static FractalType: typeof FractalType;
    static CellularDistanceFunction: typeof CellularDistanceFunction;
    static CellularReturnType: typeof CellularReturnType;
    static DomainWarpType: typeof DomainWarpType;
    static TransformType3D: typeof TransformType3D;
    private _Seed;
    private _Frequency;
    private _NoiseType;
    private _RotationType3D;
    private _TransformType3D;
    private _DomainWarpAmp;
    private _FractalType;
    private _Octaves;
    private _Lacunarity;
    private _Gain;
    private _WeightedStrength;
    private _PingPongStrength;
    private _FractalBounding;
    private _CellularDistanceFunction;
    private _CellularReturnType;
    private _CellularJitterModifier;
    private _DomainWarpType;
    private _WarpTransformType3D;
    /**
     * @description Create new FastNoiseLite object with optional seed
     */
    constructor(seed?: number);
    /**
     * @description Sets seed used for all noise types
     * @remarks Default: 1337
     */
    SetSeed(seed: number): void;
    /**
     * @description Sets frequency for all noise types
     * @remarks Default: 0.01
     */
    SetFrequency(frequency: number): void;
    /**
     * @description Sets noise algorithm used for GetNoise(...)
     * @remarks Default: OpenSimplex2
     */
    SetNoiseType(noiseType: NoiseType): void;
    /**
     * @description Sets domain rotation type for 3D Noise and 3D DomainWarp.
     * @description Can aid in reducing directional artifacts when sampling a 2D plane in 3D
     * @remarks Default: None
     */
    SetRotationType3D(rotationType3D: RotationType3D): void;
    /**
     * @description Sets method for combining octaves in all fractal noise types
     * @remarks Default: None
     */
    SetFractalType(fractalType: FractalType): void;
    /**
     * @description Sets octave count for all fractal noise types
     * @remarks Default: 3
     */
    SetFractalOctaves(octaves: number): void;
    /**
     * @description Sets octave lacunarity for all fractal noise types
     * @remarks Default: 2.0
     */
    SetFractalLacunarity(lacunarity: number): void;
    /**
     * @description Sets octave gain for all fractal noise types
     * @remarks Default: 0.5
     */
    SetFractalGain(gain: number): void;
    /**
     * @description Sets octave weighting for all none DomainWarp fratal types
     * @remarks Default: 0.0 | Keep between 0...1 to maintain -1...1 output bounding
     */
    SetFractalWeightedStrength(weightedStrength: number): void;
    /**
     * @description Sets strength of the fractal ping pong effect
     * @remarks Default: 2.0
     */
    SetFractalPingPongStrength(pingPongStrength: number): void;
    /**
     * @description Sets distance function used in cellular noise calculations
     * @remarks Default: EuclideanSq
     */
    SetCellularDistanceFunction(cellularDistanceFunction: CellularDistanceFunction): void;
    /**
     * @description Sets return type from cellular noise calculations
     * @remarks Default: Distance
     */
    SetCellularReturnType(cellularReturnType: CellularReturnType): void;
    /**
     * @description Sets the maximum distance a cellular point can move from it's grid position
     * @remarks Default: 1.0
     */
    SetCellularJitter(cellularJitter: number): void;
    /**
     * @description Sets the warp algorithm when using DomainWarp(...)
     * @remarks Default: OpenSimplex2
     */
    SetDomainWarpType(domainWarpType: DomainWarpType): void;
    /**
     * @description Sets the maximum warp distance from original position when using DomainWarp(...)
     * @remarks Default: 1.0
     */
    SetDomainWarpAmp(domainWarpAmp: number): void;
    /**
     * @description 2D/3D noise at given position using current settings
     * @return Noise output bounded between -1...1
     */
    GetNoise(x: number, y: number, z?: number): number;
    /**
     * @description 2D/3D warps the input position using current domain warp settings
     */
    DomainWrap(coord: Vector2 | Vector3): void;
    private _Gradients2D;
    private _RandVecs2D;
    private _Gradients3D;
    private _RandVecs3D;
    private _PrimeX;
    private _PrimeY;
    private _PrimeZ;
    private static _Lerp;
    private static _InterpHermite;
    private static _InterpQuintic;
    private static _CubicLerp;
    private static _PingPong;
    private _CalculateFractalBounding;
    private _HashR2;
    private _HashR3;
    private _ValCoordR2;
    private _ValCoordR3;
    private _GradCoordR2;
    private _GradCoordR3;
    private _GenNoiseSingleR2;
    private _GenNoiseSingleR3;
    private _UpdateTransformType3D;
    private _UpdateWarpTransformType3D;
    private _GenFractalFBmR2;
    private _GenFractalFBmR3;
    private _GenFractalRidgedR2;
    private _GenFractalRidgedR3;
    private _GenFractalPingPongR2;
    private _GenFractalPingPongR3;
    private _SingleOpenSimplex2R2;
    private _SingleOpenSimplex2R3;
    private _SingleOpenSimplex2SR2;
    private _SingleOpenSimplex2SR3;
    private _SingleCellularR2;
    private _SingleCellularR3;
    private _SinglePerlinR2;
    private _SinglePerlinR3;
    private _SingleValueCubicR2;
    private _SingleValueCubicR3;
    private _SingleValueR2;
    private _SingleValueR3;
    private _DoSingleDomainWarp;
    private _DomainWarpSingle;
    private _DomainWarpFractalProgressive;
    private _DomainWarpFractalIndependent;
    private _SingleDomainWarpBasicGrid;
    private _SingleDomainWarpOpenSimplex2Gradient;
}

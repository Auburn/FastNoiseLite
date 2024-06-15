// MIT License
//
// Copyright(c) 2023 Jordan Peck (jordan.me2@gmail.com)
// Copyright(c) 2023 Contributors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// .'',;:cldxkO00KKXXNNWWWNNXKOkxdollcc::::::;:::ccllloooolllllllllooollc:,'...        ...........',;cldxkO000Okxdlc::;;;,,;;;::cclllllll
// ..',;:ldxO0KXXNNNNNNNNXXK0kxdolcc::::::;;;,,,,,,;;;;;;;;;;:::cclllllc:;'....       ...........',;:ldxO0KXXXK0Okxdolc::;;;;::cllodddddo
// ...',:loxO0KXNNNNNXXKK0Okxdolc::;::::::::;;;,,'''''.....''',;:clllllc:;,'............''''''''',;:loxO0KXNNNNNXK0Okxdollccccllodxxxxxxd
// ....';:ldkO0KXXXKK00Okxdolcc:;;;;;::cclllcc:;;,''..... ....',;clooddolcc:;;;;,,;;;;;::::;;;;;;:cloxk0KXNWWWWWWNXKK0Okxddoooddxxkkkkkxx
// .....';:ldxkOOOOOkxxdolcc:;;;,,,;;:cllooooolcc:;'...      ..,:codxkkkxddooollloooooooollcc:::::clodkO0KXNWWWWWWNNXK00Okxxxxxxxxkkkkxxx
// . ....';:cloddddo___________,,,,;;:clooddddoolc:,...      ..,:ldx__00OOOkkk___kkkkkkxxdollc::::cclodkO0KXXNNNNNNXXK0OOkxxxxxxxxxxxxddd
// .......',;:cccc:|           |,,,;;:cclooddddoll:;'..     ..';cox|  \KKK000|   |KK00OOkxdocc___;::clldxxkO0KKKKK00Okkxdddddddddddddddoo
// .......'',,,,,''|   ________|',,;;::cclloooooolc:;'......___:ldk|   \KK000|   |XKKK0Okxolc|   |;;::cclodxxkkkkxxdoolllcclllooodddooooo
// ''......''''....|   |  ....'',,,,;;;::cclloooollc:;,''.'|   |oxk|    \OOO0|   |KKK00Oxdoll|___|;;;;;::ccllllllcc::;;,,;;;:cclloooooooo
// ;;,''.......... |   |_____',,;;;____:___cllo________.___|   |___|     \xkk|   |KK_______ool___:::;________;;;_______...'',;;:ccclllloo
// c:;,''......... |         |:::/     '   |lo/        |           |      \dx|   |0/       \d|   |cc/        |'/       \......',,;;:ccllo
// ol:;,'..........|    _____|ll/    __    |o/   ______|____    ___|   |   \o|   |/   ___   \|   |o/   ______|/   ___   \ .......'',;:clo
// dlc;,...........|   |::clooo|    /  |   |x\___   \KXKKK0|   |dol|   |\   \|   |   |   |   |   |d\___   \..|   |  /   /       ....',:cl
// xoc;'...  .....'|   |llodddd|    \__|   |_____\   \KKK0O|   |lc:|   |'\       |   |___|   |   |_____\   \.|   |_/___/...      ...',;:c
// dlc;'... ....',;|   |oddddddo\          |          |Okkx|   |::;|   |..\      |\         /|   |          | \         |...    ....',;:c
// ol:,'.......',:c|___|xxxddollc\_____,___|_________/ddoll|___|,,,|___|...\_____|:\ ______/l|___|_________/...\________|'........',;::cc
// c:;'.......';:codxxkkkkxxolc::;::clodxkOO0OOkkxdollc::;;,,''''',,,,''''''''''',,'''''',;:loxkkOOkxol:;,'''',,;:ccllcc:;,'''''',;::ccll
// ;,'.......',:codxkOO0OOkxdlc:;,,;;:cldxxkkxxdolc:;;,,''.....'',;;:::;;,,,'''''........,;cldkO0KK0Okdoc::;;::cloodddoolc:;;;;;::ccllooo
// .........',;:lodxOO0000Okdoc:,,',,;:clloddoolc:;,''.......'',;:clooollc:;;,,''.......',:ldkOKXNNXX0Oxdolllloddxxxxxxdolccccccllooodddd
// .    .....';:cldxkO0000Okxol:;,''',,;::cccc:;,,'.......'',;:cldxxkkxxdolc:;;,'.......';coxOKXNWWWNXKOkxddddxxkkkkkkxdoollllooddxxxxkkk
//       ....',;:codxkO000OOxdoc:;,''',,,;;;;,''.......',,;:clodkO00000Okxolc::;,,''..',;:ldxOKXNWWWNNK0OkkkkkkkkkkkxxddooooodxxkOOOOO000
//       ....',;;clodxkkOOOkkdolc:;,,,,,,,,'..........,;:clodxkO0KKXKK0Okxdolcc::;;,,,;;:codkO0XXNNNNXKK0OOOOOkkkkxxdoollloodxkO0KKKXXXXX
//
// VERSION: 1.1.1
// https://github.com/Auburn/FastNoiseLite

//! Developed and tested using Zig 0.12.0

const std = @import("std");
const expect = std.testing.expect;

/// A constant prime-number used in x-axis calculations.
const prime_x: i32 = 501125321;
/// A constant prime-number used in y-axis calculations.
const prime_y: i32 = 1136930381;
/// A constant prime-number used in z-axis calculations.
const prime_z: i32 = 1720413743;
/// The value of `prime_z` left-shifted by 1 bit.
const prime_x_shl1 = std.math.shl(i32, prime_x, 1);
/// The value of `prime_y` left-shifted by 1 bit. Overflow bits are truncated.
const prime_y_shl1 = std.math.shl(i32, prime_y, 1);
/// The value of `prime_z` left-shifted by 1 bit. Overflow bits are truncated.
const prime_z_shl1 = std.math.shl(i32, prime_z, 1);

/// Describes a noise-generating algorithm.
pub const NoiseType = enum {
    /// Simplex is the successor of and comparable to Perlin noise, but with fewer
    /// directional artifacts in higher dimensions, and a lower computational overhead.
    simplex,
    /// A variation of simplex (i.e. "SuperSimplex") which has a smoother output.
    simplex_smooth,
    /// Worley/Voronoi noise algorithm.
    cellular,
    /// A classic general-purpose gradient noise.
    perlin,
    /// A more complex variation of value noise that employs a cubic function.
    value_cubic,
    /// Consists of the creation of a lattice of points which are assigned random values.
    value,
};

pub const RotationType = enum {
    none,
    improve_xy_planes,
    improve_xz_planes,
};

pub const FractalType = enum {
    none,
    fbm,
    ridged,
    ping_pong,
    progressive,
    independent,
};

pub const CellularDistanceFunc = enum {
    euclidean,
    euclidean_sq,
    manhattan,
    hybrid,
};

pub const CellularReturnType = enum {
    cell_value,
    distance,
    distance2,
    distance2_add,
    distance2_sub,
    distance2_mul,
    distance2_div,
};

pub const DomainWarpType = enum {
    simplex,
    simplex_reduced,
    basic_grid,
};

/// Structure containing the noise-generater state.
///
/// The generator behaves as a state-machine, and all of its functions are "pure" in
/// the regard that that they do not modify the internal state of the generator.
/// Configuration of the generator is done via the struct's fields, which are intended
/// to be modified directly as-needed.
pub fn Noise(comptime Float: type) type {
    // Compile-error if a non-float is specified
    switch (@typeInfo(Float)) {
        .Float => |f| switch (f.bits) {
            32, 64 => {},
            else => @compileError("only 32 and 64 bit types supported"),
        },
        else => @compileError(@typeName(Float) ++ " is not a floating-point type."),
    }

    // Equivalent to -ffast-math in GCC
    @setFloatMode(.optimized);

    const Vec4 = @Vector(4, Float);
    const sqrt3: Float = comptime @sqrt(3.0);
    const f2: Float = comptime 0.5 * (sqrt3 - 1.0);
    const g2: Float = comptime (3.0 - sqrt3) / 6.0;
    const r3: Float = comptime 2.0 / 3.0;

    return struct {
        /// Alias name for the current type.
        const State = @This();
        /// Seed used for all noise types.
        ///
        /// Default: `1337`
        seed: i32 = 1337,
        /// The frequency for all noise types.
        ///
        /// Default: `0.01`
        frequency: Float = 0.01,
        /// The noise algorithm to be used.
        ///
        /// Default: `.simplex`
        noise_type: NoiseType = .simplex,
        /// Sets rotation type for 3D noise.
        ///
        /// Default: `.none`
        rotation_type: RotationType = .none,
        /// The method used for combining octaves for all fractal noise types.
        ///
        /// Default: `.none`
        fractal_type: FractalType = .none,
        /// The octave count for all fractal noise types.
        ///
        /// Default: `3`
        octaves: u32 = 3,
        /// The octave lacunarity for all fractal noise types.
        ///
        /// Default: `2.0`
        lacunarity: Float = 2.0,
        /// The octave gain for all fractal noise types.
        ///
        /// Default: `0.5`
        gain: Float = 0.5,
        /// The octave weighting for all non domain warp fractal types.
        ///
        /// Default: `0.0`
        weighted_strength: Float = 0.0,
        /// The strength of the fractal ping pong effect.
        ///
        /// Default: `2.0`
        ping_pong_strength: Float = 2.0,
        /// The distance function used in cellular noise calculations.
        ///
        /// Default: `euclidean_sq`
        cellular_distance: CellularDistanceFunc = .euclidean_sq,
        /// The cellular return type from cellular noise calculations.
        ///
        /// Default: `.distance`
        cellular_return: CellularReturnType = .distance,
        /// The maximum distance a cellular point can move from it's grid position.
        /// Setting this higher than `1.0` will cause artifacts.
        ///
        /// Default: `1.0`
        cellular_jitter_mod: Float = 1.0,
        /// The warp algorithm when using domain warp.
        ///
        /// Default: `.simplex`
        domain_warp_type: DomainWarpType = .simplex,
        /// The maximum warp distance from original position when using domain warp.
        ///
        /// Default: `1.0`
        domain_warp_amp: Float = 1.0,

        /// Generates 2D noise at given position using the currently configured state.
        /// The return value is normalized to the the range of `-1.0` to `1.0`.
        pub fn genNoise2D(self: *const State, x: Float, y: Float) Float {
            var ox = x;
            var oy = y;
            self.transformNoiseCoordinate2D(&ox, &oy);

            return switch (self.fractal_type) {
                .fbm => self.genFractalFBm2D(ox, oy),
                .ridged => self.genFractalRidged2D(ox, oy),
                .ping_pong => self.genFractalPingPong2D(ox, oy),
                else => self.genNoiseSingle2D(self.seed, ox, oy),
            };
        }

        /// Generates 2D noise at given position using the currently configured state.
        /// The return value is mapped to the given range and numeric type.
        pub fn genNoise2DRange(self: *const State, x: Float, y: Float, comptime T: type, min: T, max: T) T {
            std.debug.assert(min < max);
            // Normalize to range of 0..1
            const n = 0.5 * (1.0 + @max(-1.0, @min(1.0, self.genNoise2D(x, y))));
            return switch (@typeInfo(T)) {
                .Int => min + @as(T, @intFromFloat(n * @as(Float, @floatFromInt(max - min)))),
                .Float => min + @as(T, @floatCast(n * @as(Float, @floatCast(max - min)))),
                else => @compileError(@typeName(T) ++ " is not a numeric type"),
            };
        }

        /// Generates 2D noise at given position using the currently configured state.
        /// The return value is mapped to the range of the specified numeric type.
        pub fn genNoise2DAsType(self: *const State, x: Float, y: Float, comptime T: type) T {
            return switch (@typeInfo(T)) {
                .Int => {
                    const min = comptime std.math.minInt(T);
                    const max = comptime std.math.maxInt(T);
                    return genNoise2DRange(self, x, y, T, min, max);
                },
                .Float => @floatCast(genNoise2D(self, x, y)),
                else => @compileError(@typeName(T) ++ " is not a numeric type"),
            };
        }

        /// Warps the specified 2D coordinates.
        ///
        /// This is typically done using two separate noise states, where one is used for
        /// modiifying the coordinates, which are then used by another for generation.
        pub inline fn domainWarp2D(self: *const State, x: *Float, y: *Float) void {
            switch (self.fractal_type) {
                .progressive => self.domainWarpFractalProgressive2D(x, y),
                .independent => self.domainWarpFractalIndependent2D(x, y),
                else => self.domainWarpSingle2D(x, y),
            }
        }

        /// Generates 3D noise at given position using the currently configured state.
        /// The return value is normalized to the the range of `-1.0` to `1.0`.
        pub fn genNoise3D(self: *const State, x: Float, y: Float, z: Float) Float {
            var ox = x;
            var oy = y;
            var oz = z;
            self.transformNoiseCoordinate3D(&ox, &oy, &oz);

            return switch (self.fractal_type) {
                .fbm => self.genFractalFBm3D(ox, oy, oz),
                .ridged => self.genFractalRidged3D(ox, oy, oz),
                .ping_pong => self.genFractalPingPong3D(ox, oy, oz),
                else => self.genNoiseSingle3D(self.seed, ox, oy, oz),
            };
        }

        /// Generates 3D noise at given position using the currently configured state.
        /// The return value is mapped to the given range and numeric type.
        pub fn genNoise3DRange(self: *const State, x: Float, y: Float, z: Float, comptime T: type, min: T, max: T) T {
            std.debug.assert(min < max);
            // Normalize to range of 0..1
            const n = 0.5 * (1.0 + @max(-1.0, @min(1.0, self.genNoise3D(x, y, z))));
            return switch (@typeInfo(T)) {
                .Int => min + @as(T, @intFromFloat(n * @as(Float, @floatFromInt(max - min)))),
                .Float => min + @as(T, @floatCast(n * @as(Float, @floatCast(max - min)))),
                else => @compileError(@typeName(T) ++ " is not a numeric type"),
            };
        }

        /// Generates 3D noise at given position using the currently configured state.
        /// The return value is mapped to the range of the specified numeric type.
        pub fn genNoise3DAsType(self: *const State, x: Float, y: Float, z: Float, comptime T: type) T {
            return switch (@typeInfo(T)) {
                .Int => {
                    const min = comptime std.math.minInt(T);
                    const max = comptime std.math.maxInt(T);
                    return genNoise3DRange(self, x, y, z, T, min, max);
                },
                .Float => @floatCast(genNoise3D(self, x, y, z)),
                else => @compileError(@typeName(T) ++ " is not a numeric type"),
            };
        }

        /// Warps the specified 3D coordinates.
        ///
        /// This is typically done using two separate noise states, where one is used for
        /// modiifying the coordinates, which are then used by another for generation.
        pub inline fn domainWarp3D(self: *const State, x: *Float, y: *Float, z: *Float) void {
            switch (self.fractal_type) {
                .progressive => self.domainWarpFractalProgressive3D(x, y, z),
                .independent => self.domainWarpFractalIndependent3D(x, y, z),
                else => self.domainWarpSingle3D(x, y, z),
            }
        }

        // End of public API

        inline fn genNoiseSingle2D(state: *const State, seed: i32, x: Float, y: Float) Float {
            return switch (state.noise_type) {
                .simplex => singleSimplex2D(seed, x, y),
                .simplex_smooth => singleSimplexS2D(seed, x, y),
                .cellular => singleCellular2D(state, seed, x, y),
                .perlin => singlePerlin2D(seed, x, y),
                .value_cubic => singleValueCubic2D(seed, x, y),
                .value => singleValue2D(seed, x, y),
            };
        }

        inline fn genNoiseSingle3D(state: *const State, seed: i32, x: Float, y: Float, z: Float) Float {
            return switch (state.noise_type) {
                .simplex => singleSimplex3D(seed, x, y, z),
                .simplex_smooth => singleSimplexS3D(seed, x, y, z),
                .cellular => singleCellular3D(state, seed, x, y, z),
                .perlin => singlePerlin3D(seed, x, y, z),
                .value_cubic => singleValueCubic3D(seed, x, y, z),
                .value => singleValue3D(seed, x, y, z),
            };
        }

        inline fn doSingleDomainWarp2D(self: *const State, seed: i32, amp: Float, freq: Float, x: Float, y: Float, xp: *Float, yp: *Float) void {
            switch (self.domain_warp_type) {
                .simplex => singleDomainWarpSimplexGradient(seed, amp * 38.283687591552734375, freq, x, y, xp, yp, false),
                .simplex_reduced => singleDomainWarpSimplexGradient(seed, amp * 16.0, freq, x, y, xp, yp, true),
                .basic_grid => singleDomainWarpBasicGrid2D(seed, amp, freq, x, y, xp, yp),
            }
        }

        inline fn doSingleDomainWarp3D(self: *const State, seed: u32, amp: Float, freq: Float, x: Float, y: Float, z: Float, xp: *Float, yp: *Float, zp: *Float) void {
            switch (self.domain_warp_type) {
                .simplex => singleDomainWarpOpenSimplex2Gradient(seed, amp * 32.69428253173828125, freq, x, y, z, xp, yp, zp, false),
                .simplex_reduced => singleDomainWarpOpenSimplex2Gradient(seed, amp * 7.71604938271605, freq, x, y, z, xp, yp, zp, true),
                .basic_grid => singleDomainWarpBasicGrid3D(seed, amp, freq, x, y, z, xp, yp, zp),
            }
        }

        // Utilities

        inline fn fastFloor(f: Float) i32 {
            return @intFromFloat(if (f >= 0) f else f - 1);
        }

        inline fn fastRound(f: Float) i32 {
            return @intFromFloat(if (f >= 0) f + 0.5 else f - 0.5);
        }

        inline fn lerp(a: Float, b: Float, t: Float) Float {
            return a + t * (b - a);
        }

        inline fn interpHermite(t: Float) Float {
            return t * t * (3 - 2 * t);
        }

        inline fn interpQuintic(t: Float) Float {
            return t * t * t * (t * (t * 6 - 15) + 10);
        }

        inline fn cubicLerp(a: Float, b: Float, c: Float, d: Float, t: Float) Float {
            const p = (d - c) - (a - b);
            return t * t * t * p + t * t * ((a - b) - p) + t * (c - a) + b;
        }

        inline fn pingPong(t: Float) Float {
            const f = t - (@floor(t * 0.5) * 2);
            return if (f < 1) f else 2 - f;
        }

        fn calculateFractalBounding(self: *const State) Float {
            const gain: Float = @abs(self.gain);
            var amp = gain;
            var amp_fractal: Float = 1.0;
            for (0..self.octaves) |_| {
                amp_fractal += amp;
                amp *= gain;
            }
            return 1.0 / amp_fractal;
        }

        // Hashing

        inline fn hash2D(seed: i32, x_primed: i32, y_primed: i32) i32 {
            const hash: i32 = seed ^ x_primed ^ y_primed;
            return hash *% 0x27D4EB2D;
        }

        inline fn hash3D(seed: i32, x_primed: i32, y_primed: i32, z_primed: i32) i32 {
            const hash: i32 = seed ^ x_primed ^ y_primed ^ z_primed;
            return hash *% 0x27D4EB2D;
        }

        inline fn valCoord2D(seed: i32, x_primed: i32, y_primed: i32) Float {
            var hash = hash2D(seed, x_primed, y_primed);
            hash *%= hash;
            hash ^= std.math.shl(i32, hash, 19);
            return @as(Float, @floatFromInt(hash)) * (1.0 / 2147483648.0);
        }

        inline fn valCoord3D(seed: i32, x_primed: i32, y_primed: i32, z_primed: i32) Float {
            var hash = hash3D(seed, x_primed, y_primed, z_primed);
            hash *%= hash;
            hash ^= std.math.shl(i32, hash, 19);
            return @as(Float, @floatFromInt(hash)) * (1.0 / 2147483648.0);
        }

        inline fn gradCoord2D(seed: i32, x_primed: i32, y_primed: i32, xd: Float, yd: Float) Float {
            var hash = hash2D(seed, x_primed, y_primed);
            hash ^= (hash >> 15);
            const index: usize = @intCast(hash & (127 << 1));
            return xd * gradients_2d[index] + yd * gradients_2d[index | 1];
        }

        inline fn gradCoord3D(seed: i32, x_primed: i32, y_primed: i32, z_primed: i32, xd: Float, yd: Float, zd: Float) Float {
            var hash = hash3D(seed, x_primed, y_primed, z_primed);
            hash ^= (hash >> 15);
            const index: usize = @intCast(hash & (63 << 2));
            return xd * gradients_3d[index] + yd * gradients_3d[index | 1] + zd * gradients_3d[index | 2];
        }

        inline fn gradCoordOut2D(seed: i32, x_primed: i32, y_primed: i32, xo: *Float, yo: *Float) void {
            const hash: usize = @intCast(hash2D(seed, x_primed, y_primed) & (255 << 1));
            xo.* = rand_2d[hash];
            yo.* = rand_2d[hash | 1];
        }

        inline fn gradCoordOut3D(seed: i32, x_primed: i32, y_primed: i32, z_primed: i32, xo: *Float, yo: *Float, zo: *Float) void {
            const hash: usize = @intCast(hash3D(seed, x_primed, y_primed, z_primed) & (255 << 2));
            xo.* = rand_3d[hash];
            yo.* = rand_3d[hash | 1];
            zo.* = rand_3d[hash | 2];
        }

        inline fn gradCoordDual2D(seed: i32, x_primed: i32, y_primed: i32, xd: Float, yd: Float, xo: *Float, yo: *Float) void {
            const hash = hash2D(seed, x_primed, y_primed);
            const index1: usize = @intCast(hash & (127 << 1));
            const index2: usize = @intCast((hash >> 7) & (255 << 1));

            const xg: Float = gradients_2d[index1];
            const yg: Float = gradients_2d[index1 | 1];
            const value = xd * xg + yd * yg;

            const xgo: Float = rand_2d[index2];
            const ygo: Float = rand_2d[index2 | 1];

            xo.* = value * xgo;
            yo.* = value * ygo;
        }

        inline fn gradCoordDual3D(seed: u32, x_primed: i32, y_primed: i32, z_primed: i32, xd: Float, yd: Float, zd: Float, xo: *Float, yo: *Float, zo: *Float) void {
            const hash = hash3D(seed, x_primed, y_primed, z_primed);
            const index1: usize = @intCast(hash & (63 << 2));
            const index2: usize = @intCast((hash >> 6) & (255 << 2));

            const xg: Float = gradients_3d[index1];
            const yg: Float = gradients_3d[index1 | 1];
            const zg: Float = gradients_3d[index1 | 2];
            const value = xd * xg + yd * yg + zd * zg;

            const xgo: Float = rand_3d[index2];
            const ygo: Float = rand_3d[index2 | 1];
            const zgo: Float = rand_3d[index2 | 2];

            xo.* = value * xgo;
            yo.* = value * ygo;
            zo.* = value * zgo;
        }

        // Noise Coordinate Transforms (frequency, and possible skew or rotation)

        fn transformNoiseCoordinate2D(self: *const State, x: *Float, y: *Float) void {
            x.* *= self.frequency;
            y.* *= self.frequency;

            switch (self.noise_type) {
                .simplex, .simplex_smooth => {
                    const t: Float = (x.* + y.*) * f2;
                    x.* += t;
                    y.* += t;
                },
                else => {},
            }
        }

        fn transformNoiseCoordinate3D(self: *const State, x: *Float, y: *Float, z: *Float) void {
            x.* *= self.frequency;
            y.* *= self.frequency;
            z.* *= self.frequency;

            switch (self.rotation_type) {
                .improve_xy_planes => {
                    const xy: Float = x.* + y.*;
                    const s2: Float = xy * -0.211324865405187;
                    z.* *= 0.577350269189626;
                    x.* += s2 - z.*;
                    y.* = y.* + s2 - z.*;
                    z.* += xy * 0.577350269189626;
                },
                .improve_xz_planes => {
                    const xz: Float = x.* + z.*;
                    const s2: Float = xz * -0.211324865405187;
                    y.* *= 0.577350269189626;
                    x.* += s2 - y.*;
                    z.* += s2 - y.*;
                    y.* += xz * 0.577350269189626;
                },
                else => switch (self.noise_type) {
                    .simplex, .simplex_smooth => {
                        const r: Float = (x.* + y.* + z.*) * r3; // Rotation, not skew
                        x.* = r - x.*;
                        y.* = r - y.*;
                        z.* = r - z.*;
                    },
                    else => {},
                },
            }
        }

        // Domain Warp Coordinate Transforms

        fn transformDomainWarpCoordinate2D(state: *const State, x: *Float, y: *Float) void {
            switch (state.domain_warp_type) {
                .simplex, .simplex_reduced => {
                    const t: Float = (x.* + y.*) * f2;
                    x.* += t;
                    y.* += t;
                },
                else => {},
            }
        }

        fn transformDomainWarpCoordinate3D(state: *const State, x: *Float, y: *Float, z: *Float) void {
            switch (state.rotation_type) {
                .improve_xy_planes => {
                    const xy: Float = *x + *y;
                    const s2: Float = xy * -0.211324865405187;
                    z.* *= 0.577350269189626;
                    x.* += s2 - *z;
                    y.* = *y + s2 - *z;
                    z.* += xy * 0.577350269189626;
                },
                .improve_xz_planes => {
                    const xz: Float = x.* + z.*;
                    const s2: Float = xz * -0.211324865405187;
                    y.* *= 0.577350269189626;
                    x.* += s2 - y.*;
                    z.* += s2 - y.*;
                    y.* += xz * 0.577350269189626;
                },
                else => switch (state.domain_warp_type) {
                    .simplex, .simplex_reduced => {
                        const r: Float = (x.* + y.* + z.*) * r3; // Rotation, not skew
                        x.* = r - x.*;
                        y.* = r - y.*;
                        z.* = r - z.*;
                    },
                    else => {},
                },
            }
        }

        // Fractal FBm

        fn genFractalFBm2D(state: *const State, x: Float, y: Float) Float {
            var sum: Float = 0;
            var vec = Vec4{ x, y, state.calculateFractalBounding(), 0 };
            const mul = Vec4{ state.lacunarity, state.lacunarity, state.gain, 0 };

            for (0..state.octaves) |i| {
                const noise = state.genNoiseSingle2D(state.seed + @as(i32, @intCast(i)), vec[0], vec[1]);
                sum += noise * vec[2];
                vec[2] *= lerp(1.0, @min(noise + 1.0, 2.0) * 0.5, state.weighted_strength);
                vec *= mul;
            }

            return sum;
        }

        fn genFractalFBm3D(state: *const State, x: Float, y: Float, z: Float) Float {
            var sum: Float = 0;
            var vec = Vec4{ x, y, z, state.calculateFractalBounding() };
            const mul = Vec4{ state.lacunarity, state.lacunarity, state.lacunarity, state.gain };

            for (0..state.octaves) |i| {
                const noise = state.genNoiseSingle3D(state.seed + @as(i32, @intCast(i)), vec[0], vec[1], vec[2]);
                sum += noise * vec[3];
                vec[3] *= lerp(1.0, (noise + 1.0) * 0.5, state.weighted_strength);
                vec *= mul;
            }

            return sum;
        }

        // Fractal Ridged

        fn genFractalRidged2D(state: *const State, x: Float, y: Float) Float {
            var sum: Float = 0;
            var vec = Vec4{ x, y, state.calculateFractalBounding(), 0 };
            const mul = Vec4{ state.lacunarity, state.lacunarity, state.gain, 0 };

            for (0..state.octaves) |i| {
                const noise = @abs(state.genNoiseSingle2D(state.seed + @as(i32, @intCast(i)), vec[0], vec[1]));
                sum += (noise * -2.0 + 1.0) * vec[2];
                vec[2] *= lerp(1.0, 1.0 - noise, state.weighted_strength);
                vec *= mul;
            }

            return sum;
        }

        fn genFractalRidged3D(state: *const State, x: Float, y: Float, z: Float) Float {
            var sum: Float = 0;
            var vec = Vec4{ x, y, z, state.calculateFractalBounding() };
            const mul = Vec4{ state.lacunarity, state.lacunarity, state.lacunarity, state.gain };

            for (0..state.octaves) |i| {
                const noise = @abs(state.genNoiseSingle3D(state.seed + @as(i32, @intCast(i)), vec[0], vec[1], vec[2]));
                sum += (noise * -2 + 1) * vec[3];
                vec[3] *= lerp(1.0, 1.0 - noise, state.weighted_strength);
                vec *= mul;
            }

            return sum;
        }

        // Fractal PingPong

        fn genFractalPingPong2D(state: *const State, x: Float, y: Float) Float {
            var sum: Float = 0;
            var vec = Vec4{ x, y, state.calculateFractalBounding(), 0 };
            const mul = Vec4{ state.lacunarity, state.lacunarity, state.gain, 0 };

            for (0..state.octaves) |i| {
                const noise = pingPong((state.genNoiseSingle2D(state.seed + @as(i32, @intCast(i)), vec[0], vec[1]) + 1) * state.ping_pong_strength);
                sum += (noise - 0.5) * 2.0 * vec[2];
                vec[2] *= lerp(1.0, noise, state.weighted_strength);
                vec *= mul;
            }

            return sum;
        }

        fn genFractalPingPong3D(state: *const State, x: Float, y: Float, z: Float) Float {
            var sum: Float = 0;
            var vec = Vec4{ x, y, z, state.calculateFractalBounding() };
            const mul = Vec4{ state.lacunarity, state.lacunarity, state.lacunarity, state.gain };

            for (0..state.octaves) |i| {
                const noise = pingPong((state.genNoiseSingle3D(state.seed + @as(i32, @intCast(i)), vec[0], vec[1], vec[2]) + 1.0) * state.ping_pong_strength);
                sum += (noise - 0.5) * 2.0 * vec[3];
                vec[3] *= lerp(1.0, noise, state.weighted_strength);
                vec *= mul;
            }

            return sum;
        }

        // Domain Warp Single Wrapper

        fn domainWarpSingle2D(state: *const State, x: *Float, y: *Float) void {
            const amp = state.domain_warp_amp * state.calculateFractalBounding();
            var xs: Float = x.*;
            var ys: Float = y.*;
            state.transformDomainWarpCoordinate2D(&xs, &ys);
            state.doSingleDomainWarp2D(state.seed, amp, state.frequency, xs, ys, x, y);
        }

        fn domainWarpSingle3D(state: *const State, x: *Float, y: *Float, z: *Float) void {
            const amp = state.domain_warp_amp * state.calculateFractalBounding();
            var xs: Float = x.*;
            var ys: Float = y.*;
            var zs: Float = z.*;
            state.transformDomainWarpCoordinate3D(&xs, &ys, &zs);
            state.doSingleDomainWarp3D(state.seed, amp, state.frequency, xs, ys, zs, x, y, z);
        }

        // Domain Warp Fractal Progressive

        fn domainWarpFractalProgressive2D(state: *const State, x: *Float, y: *Float) void {
            var amp = state.domain_warp_amp * state.calculateFractalBounding();
            var freq = state.frequency;
            for (0..state.octaves) |i| {
                var xs: Float = x.*;
                var ys: Float = y.*;
                state.transformDomainWarpCoordinate2D(&xs, &ys);
                state.doSingleDomainWarp2D(state.seed + @as(i32, @intCast(i)), amp, freq, xs, ys, x, y);
                amp *= state.gain;
                freq *= state.lacunarity;
            }
        }

        fn domainWarpFractalProgressive3D(state: *const State, x: *Float, y: *Float, z: *Float) void {
            var amp = state.domain_warp_amp * state.calculateFractalBounding();
            var freq = state.frequency;
            for (0..state.octaves) |i| {
                var xs: Float = x.*;
                var ys: Float = y.*;
                var zs: Float = z.*;
                state.transformDomainWarpCoordinate3D(&xs, &ys, &zs);
                state.doSingleDomainWarp3D(state.seed + @as(i32, @intCast(i)), amp, freq, xs, ys, zs, x, y, z);
                amp *= state.gain;
                freq *= state.lacunarity;
            }
        }

        // Domain Warp Fractal Independent

        fn domainWarpFractalIndependent2D(state: *const State, x: *Float, y: *Float) void {
            var xs: Float = x.*;
            var ys: Float = y.*;
            state.transformDomainWarpCoordinate2D(&xs, &ys);
            const amp = state.domain_warp_amp * state.calculateFractalBounding();
            const freq = state.frequency;
            for (0..state.octaves) |i| {
                state.doSingleDomainWarp2D(state.seed + @as(i32, @intCast(i)), amp, freq, xs, ys, x, y);
                amp *= state.gain;
                freq *= state.lacunarity;
            }
        }

        fn domainWarpFractalIndependent3D(state: *const State, x: *Float, y: *Float, z: *Float) void {
            var xs: Float = x.*;
            var ys: Float = y.*;
            var zs: Float = z.*;
            state.transformDomainWarpCoordinate3D(&xs, &ys, &zs);
            const amp = state.domain_warp_amp * state.calculateFractalBounding();
            const freq = state.frequency;
            for (0..state.octaves) |i| {
                state.doSingleDomainWarp3D(state.seed + @as(i32, @intCast(i)), amp, freq, xs, ys, zs, x, y, z);
                amp *= state.gain;
                freq *= state.lacunarity;
            }
        }

        // Domain Warp Basic Grid

        fn singleDomainWarpBasicGrid2D(seed: i32, warp_amp: Float, frequency: Float, x: Float, y: Float, xp: *Float, yp: *Float) void {
            const xf = x * frequency;
            const yf = y * frequency;

            var x0 = fastFloor(xf);
            var y0 = fastFloor(yf);

            const xs = interpHermite(xf - @as(Float, @floatFromInt(x0)));
            const ys = interpHermite(yf - @as(Float, @floatFromInt(y0)));

            x0 *%= prime_x;
            y0 *%= prime_y;
            const x1 = x0 +% prime_x;
            const y1 = y0 +% prime_y;

            var idx0: usize = @intCast(hash2D(seed, x0, y0) & (255 << 1));
            var idx1: usize = @intCast(hash2D(seed, x1, y0) & (255 << 1));

            const lx0x = lerp(rand_2d[idx0], rand_2d[idx1], xs);
            const ly0x = lerp(rand_2d[idx0 | 1], rand_2d[idx1 | 1], xs);

            idx0 = @intCast(hash2D(seed, x0, y1) & (255 << 1));
            idx1 = @intCast(hash2D(seed, x1, y1) & (255 << 1));

            const lx1x = lerp(rand_2d[idx0], rand_2d[idx1], xs);
            const ly1x = lerp(rand_2d[idx0 | 1], rand_2d[idx1 | 1], xs);

            xp.* += lerp(lx0x, lx1x, ys) * warp_amp;
            yp.* += lerp(ly0x, ly1x, ys) * warp_amp;
        }

        fn singleDomainWarpBasicGrid3D(seed: i32, warp_amp: Float, frequency: Float, x: Float, y: Float, z: Float, xp: *Float, yp: *Float, zp: *Float) void {
            const xf = x * frequency;
            const yf = y * frequency;
            const zf = z * frequency;

            var x0 = fastFloor(xf);
            var y0 = fastFloor(yf);
            var z0 = fastFloor(zf);

            const xs = interpHermite(xf - @as(Float, @floatFromInt(x0)));
            const ys = interpHermite(yf - @as(Float, @floatFromInt(y0)));
            const zs = interpHermite(zf - @as(Float, @floatFromInt(z0)));

            x0 *%= prime_x;
            y0 *%= prime_y;
            z0 *%= prime_z;
            const x1 = x0 +% prime_x;
            const y1 = y0 +% prime_y;
            const z1 = z0 +% prime_z;

            var idx0: usize = @intCast(hash3D(seed, x0, y0, z0) & (255 << 2));
            var idx1: usize = @intCast(hash3D(seed, x1, y0, z0) & (255 << 2));

            const lx0x = lerp(rand_3d[idx0], rand_3d[idx1], xs);
            const ly0x = lerp(rand_3d[idx0 | 1], rand_3d[idx1 | 1], xs);
            const lz0x = lerp(rand_3d[idx0 | 2], rand_3d[idx1 | 2], xs);

            idx0 = @intCast(hash3D(seed, x0, y1, z0) & (255 << 2));
            idx1 = @intCast(hash3D(seed, x1, y1, z0) & (255 << 2));

            var lx1x = lerp(rand_3d[idx0], rand_3d[idx1], xs);
            var ly1x = lerp(rand_3d[idx0 | 1], rand_3d[idx1 | 1], xs);
            var lz1x = lerp(rand_3d[idx0 | 2], rand_3d[idx1 | 2], xs);

            const lx0y = lerp(lx0x, lx1x, ys);
            const ly0y = lerp(ly0x, ly1x, ys);
            const lz0y = lerp(lz0x, lz1x, ys);

            idx0 = hash3D(seed, x0, y0, z1) & (255 << 2);
            idx1 = hash3D(seed, x1, y0, z1) & (255 << 2);

            lx0x = lerp(rand_3d[idx0], rand_3d[idx1], xs);
            ly0x = lerp(rand_3d[idx0 | 1], rand_3d[idx1 | 1], xs);
            lz0x = lerp(rand_3d[idx0 | 2], rand_3d[idx1 | 2], xs);

            idx0 = hash3D(seed, x0, y1, z1) & (255 << 2);
            idx1 = hash3D(seed, x1, y1, z1) & (255 << 2);

            lx1x = lerp(rand_3d[idx0], rand_3d[idx1], xs);
            ly1x = lerp(rand_3d[idx0 | 1], rand_3d[idx1 | 1], xs);
            lz1x = lerp(rand_3d[idx0 | 2], rand_3d[idx1 | 2], xs);

            xp.* += lerp(lx0y, lerp(lx0x, lx1x, ys), zs) * warp_amp;
            yp.* += lerp(ly0y, lerp(ly0x, ly1x, ys), zs) * warp_amp;
            zp.* += lerp(lz0y, lerp(lz0x, lz1x, ys), zs) * warp_amp;
        }

        // Domain Warp Simplex/OpenSimplex2

        fn singleDomainWarpSimplexGradient(seed: i32, warp_amp: Float, frequency: Float, x: Float, y: Float, xr: *Float, yr: *Float, out_grad: bool) void {
            const xx = x * frequency;
            const yy = y * frequency;

            var i = fastFloor(xx);
            var j = fastFloor(yy);
            const xi = xx - @as(Float, @floatFromInt(i));
            const yi = yy - @as(Float, @floatFromInt(j));

            const t = (xi + yi) * g2;
            const x0: Float = xi - t;
            const y0: Float = yi - t;

            i *%= prime_x;
            j *%= prime_y;

            var vx: Float = 0;
            var vy: Float = 0;
            var xo: Float = undefined;
            var yo: Float = undefined;

            const a = 0.5 - x0 * x0 - y0 * y0;
            if (a > 0) {
                const aaaa = (a * a) * (a * a);
                if (out_grad) {
                    gradCoordOut2D(seed, i, j, &xo, &yo);
                } else {
                    gradCoordDual2D(seed, i, j, x0, y0, &xo, &yo);
                }
                vx += aaaa * xo;
                vy += aaaa * yo;
            }

            const c = (2.0 * (1.0 - 2.0 * g2) * (1.0 / g2 - 2.0)) * t + ((-2.0 * (1.0 - 2.0 * g2) * (1.0 - 2.0 * g2)) + a);
            if (c > 0) {
                const x2 = x0 + (2 * g2 - 1.0);
                const y2 = y0 + (2 * g2 - 1.0);
                const cccc = (c * c) * (c * c);
                if (out_grad) {
                    gradCoordOut2D(seed, i +% prime_x, j +% prime_y, &xo, &yo);
                } else {
                    gradCoordDual2D(seed, i +% prime_x, j +% prime_y, x2, y2, &xo, &yo);
                }
                vx += cccc * xo;
                vy += cccc * yo;
            }

            if (y0 > x0) {
                const x1 = x0 + g2;
                const y1 = y0 + (g2 - 1.0);
                const b = 0.5 - x1 * x1 - y1 * y1;
                if (b > 0) {
                    const bbbb = (b * b) * (b * b);
                    if (out_grad) {
                        gradCoordOut2D(seed, i, j +% prime_y, &xo, &yo);
                    } else {
                        gradCoordDual2D(seed, i, j +% prime_y, x1, y1, &xo, &yo);
                    }
                    vx += bbbb * xo;
                    vy += bbbb * yo;
                }
            } else {
                const x1 = x0 + (g2 - 1.0);
                const y1 = y0 + g2;
                const b = 0.5 - x1 * x1 - y1 * y1;
                if (b > 0) {
                    const bbbb = (b * b) * (b * b);
                    if (out_grad) {
                        gradCoordOut2D(seed, i +% prime_x, j, &xo, &yo);
                    } else {
                        gradCoordDual2D(seed, i +% prime_x, j, x1, y1, &xo, &yo);
                    }
                    vx += bbbb * xo;
                    vy += bbbb * yo;
                }
            }

            xr.* += vx * warp_amp;
            yr.* += vy * warp_amp;
        }

        fn singleDomainWarpOpenSimplex2Gradient(seed: i32, warp_amp: Float, frequency: Float, x: Float, y: Float, z: Float, xr: *Float, yr: *Float, zr: *Float, out_grad: bool) void {
            const xx = x * frequency;
            const yy = y * frequency;
            const zz = z * frequency;

            var i = fastRound(xx);
            var j = fastRound(yy);
            var k = fastRound(zz);
            const x0 = xx - @as(Float, @floatFromInt(i));
            const y0 = yy - @as(Float, @floatFromInt(j));
            const z0 = zz - @as(Float, @floatFromInt(k));

            var xNSign = @as(i32, @intFromFloat(-x0 - 1.0)) | 1;
            var yNSign = @as(i32, @intFromFloat(-y0 - 1.0)) | 1;
            var zNSign = @as(i32, @intFromFloat(-z0 - 1.0)) | 1;

            const ax0 = @as(Float, @floatFromInt(xNSign)) * -x0;
            const ay0 = @as(Float, @floatFromInt(yNSign)) * -y0;
            const az0 = @as(Float, @floatFromInt(zNSign)) * -z0;

            i *%= prime_x;
            j *%= prime_y;
            k *%= prime_z;

            var vx: Float = 0;
            var vy: Float = 0;
            var vz: Float = 0;
            var xo: Float = undefined;
            var yo: Float = undefined;
            var zo: Float = undefined;

            var seed_value = seed;
            const a = (0.6 - x0 * x0) - (y0 * y0 + z0 * z0);
            var l: usize = 0;
            while (l < 2) : (l += 1) {
                const xNSignf: Float = @floatFromInt(xNSign);
                const yNSignf: Float = @floatFromInt(yNSign);
                const zNSignf: Float = @floatFromInt(zNSign);

                if (a > 0) {
                    const aaaa = (a * a) * (a * a);
                    if (out_grad) {
                        gradCoordOut3D(seed_value, i, j, k, &xo, &yo, &zo);
                    } else {
                        gradCoordDual3D(seed_value, i, j, k, x0, y0, z0, &xo, &yo, &zo);
                    }
                    vx += aaaa * xo;
                    vy += aaaa * yo;
                    vz += aaaa * zo;
                }

                var b = a + 1.0;
                var ii = i;
                var jj = j;
                var kk = k;
                var x1 = x0;
                var y1 = y0;
                var z1 = z0;
                if (ax0 >= ay0 and ax0 >= az0) {
                    x1 += xNSignf;
                    b -= xNSignf * 2.0 * x1;
                    ii -= xNSign *% prime_x;
                } else if (ay0 > ax0 and ay0 >= az0) {
                    y1 += yNSignf;
                    b -= yNSignf * 2.0 * y1;
                    jj -= yNSign *% prime_y;
                } else {
                    z1 += zNSignf;
                    b -= zNSignf * 2.0 * z1;
                    kk -= zNSign *% prime_z;
                }

                if (b > 0) {
                    const bbbb = (b * b) * (b * b);
                    if (out_grad) {
                        gradCoordOut3D(seed_value, ii, jj, kk, &xo, &yo, &zo);
                    } else {
                        gradCoordDual3D(seed_value, ii, jj, kk, x1, y1, z1, &xo, &yo, &zo);
                    }
                    vx += bbbb * xo;
                    vy += bbbb * yo;
                    vz += bbbb * zo;
                }

                if (l == 1) break;

                ax0 = 0.5 - ax0;
                ay0 = 0.5 - ay0;
                az0 = 0.5 - az0;

                x0 = xNSign * ax0;
                y0 = yNSign * ay0;
                z0 = zNSign * az0;

                a += (0.75 - ax0) - (ay0 + az0);

                i += (xNSign >> 1) & prime_x;
                j += (yNSign >> 1) & prime_y;
                k += (zNSign >> 1) & prime_z;

                xNSign = -xNSign;
                yNSign = -yNSign;
                zNSign = -zNSign;

                seed_value += 1293373;
            }

            xr.* += vx * warp_amp;
            yr.* += vy * warp_amp;
            zr.* += vz * warp_amp;
        }

        // Perlin Noise

        pub fn singlePerlin2D(seed: i32, x: Float, y: Float) Float {
            var x0 = fastFloor(x);
            var y0 = fastFloor(y);

            const xd0: Float = x - @as(Float, @floatFromInt(x0));
            const yd0: Float = y - @as(Float, @floatFromInt(y0));
            const xd1: Float = xd0 - 1;
            const yd1: Float = yd0 - 1;

            const xs: Float = interpQuintic(xd0);
            const ys: Float = interpQuintic(yd0);

            x0 *%= prime_x;
            y0 *%= prime_y;
            const x1 = x0 +% prime_x;
            const y1 = y0 +% prime_y;

            const xf0: Float = lerp(gradCoord2D(seed, x0, y0, xd0, yd0), gradCoord2D(seed, x1, y0, xd1, yd0), xs);
            const xf1: Float = lerp(gradCoord2D(seed, x0, y1, xd0, yd1), gradCoord2D(seed, x1, y1, xd1, yd1), xs);

            return lerp(xf0, xf1, ys) * 1.4247691104677813;
        }

        fn singlePerlin3D(seed: i32, x: Float, y: Float, z: Float) Float {
            var x0 = fastFloor(x);
            var y0 = fastFloor(y);
            var z0 = fastFloor(z);

            const xd0: Float = x - @as(Float, @floatFromInt(x0));
            const yd0: Float = y - @as(Float, @floatFromInt(y0));
            const zd0: Float = z - @as(Float, @floatFromInt(z0));
            const xd1: Float = xd0 - 1;
            const yd1: Float = yd0 - 1;
            const zd1: Float = zd0 - 1;

            const xs: Float = interpQuintic(xd0);
            const ys: Float = interpQuintic(yd0);
            const zs: Float = interpQuintic(zd0);

            x0 *%= prime_x;
            y0 *%= prime_y;
            z0 *%= prime_z;
            const x1 = x0 +% prime_x;
            const y1 = y0 +% prime_y;
            const z1 = z0 +% prime_z;

            const xf00: Float = lerp(gradCoord3D(seed, x0, y0, z0, xd0, yd0, zd0), gradCoord3D(seed, x1, y0, z0, xd1, yd0, zd0), xs);
            const xf10: Float = lerp(gradCoord3D(seed, x0, y1, z0, xd0, yd1, zd0), gradCoord3D(seed, x1, y1, z0, xd1, yd1, zd0), xs);
            const xf01: Float = lerp(gradCoord3D(seed, x0, y0, z1, xd0, yd0, zd1), gradCoord3D(seed, x1, y0, z1, xd1, yd0, zd1), xs);
            const xf11: Float = lerp(gradCoord3D(seed, x0, y1, z1, xd0, yd1, zd1), gradCoord3D(seed, x1, y1, z1, xd1, yd1, zd1), xs);

            const yf0 = lerp(xf00, xf10, ys);
            const yf1 = lerp(xf01, xf11, ys);
            return lerp(yf0, yf1, zs) * 0.964921414852142333984375;
        }

        // Simplex/OpenSimplex2 Noise

        fn singleSimplex2D(seed: i32, x: Float, y: Float) Float {
            // 2D OpenSimplex2 case uses the same algorithm as ordinary Simplex.
            var i: i32 = fastFloor(x);
            var j: i32 = fastFloor(y);
            const xi: Float = x - @as(Float, @floatFromInt(i));
            const yi: Float = y - @as(Float, @floatFromInt(j));

            const t: Float = (xi + yi) * g2;
            const x0: Float = xi - t;
            const y0: Float = yi - t;

            i *%= prime_x;
            j *%= prime_y;
            var n0: Float = 0;
            var n1: Float = 0;
            var n2: Float = 0;

            const a: Float = 0.5 - x0 * x0 - y0 * y0;
            if (a > 0) n0 = (a * a) * (a * a) * gradCoord2D(seed, i, j, x0, y0);

            const c: Float = (2.0 * (1.0 - 2.0 * g2) * (1.0 / g2 - 2.0)) * t + ((-2.0 * (1.0 - 2.0 * g2) * (1.0 - 2.0 * g2)) + a);
            if (c > 0) {
                const x2: Float = x0 + (2 * g2 - 1);
                const y2: Float = y0 + (2 * g2 - 1);
                n2 = (c * c) * (c * c) * gradCoord2D(seed, i +% prime_x, j +% prime_y, x2, y2);
            }

            if (y0 > x0) {
                const x1: Float = x0 + g2;
                const y1: Float = y0 + (g2 - 1);
                const b: Float = 0.5 - x1 * x1 - y1 * y1;
                if (b > 0) n1 = (b * b) * (b * b) * gradCoord2D(seed, i, j +% prime_y, x1, y1);
            } else {
                const x1: Float = x0 + (g2 - 1);
                const y1: Float = y0 + g2;
                const b: Float = 0.5 - x1 * x1 - y1 * y1;
                if (b > 0) n1 = (b * b) * (b * b) * gradCoord2D(seed, i +% prime_x, j, x1, y1);
            }

            return (n0 + n1 + n2) * 99.83685446303647;
        }

        fn singleSimplex3D(seed: i32, x: Float, y: Float, z: Float) Float {
            var i: i32 = fastRound(x);
            var j: i32 = fastRound(y);
            var k: i32 = fastRound(z);
            var x0: Float = x - @as(Float, @floatFromInt(i));
            var y0: Float = y - @as(Float, @floatFromInt(j));
            var z0: Float = z - @as(Float, @floatFromInt(k));

            var xNSign: i32 = @as(i32, @intFromFloat(-1.0 - x0)) | 1;
            var yNSign: i32 = @as(i32, @intFromFloat(-1.0 - y0)) | 1;
            var zNSign: i32 = @as(i32, @intFromFloat(-1.0 - z0)) | 1;

            var ax0: Float = @as(Float, @floatFromInt(xNSign)) * -x0;
            var ay0: Float = @as(Float, @floatFromInt(yNSign)) * -y0;
            var az0: Float = @as(Float, @floatFromInt(zNSign)) * -z0;

            i *%= prime_x;
            j *%= prime_y;
            k *%= prime_z;

            var l: usize = 0;
            var value: Float = 0;
            var a: Float = (0.6 - x0 * x0) - (y0 * y0 + z0 * z0);
            var seed_value = seed;

            while (true) : (l += 1) {
                const xNSignf = @as(Float, @floatFromInt(xNSign));
                const yNSignf = @as(Float, @floatFromInt(yNSign));
                const zNSignf = @as(Float, @floatFromInt(zNSign));

                if (a > 0) value += (a * a) * (a * a) * gradCoord3D(seed_value, i, j, k, x0, y0, z0);

                var b: Float = a + 1;
                var ii = i;
                var jj = j;
                var kk = k;
                var x1: Float = x0;
                var y1: Float = y0;
                var z1: Float = z0;
                if (ax0 >= ay0 and ax0 >= az0) {
                    x1 += xNSignf;
                    b -= xNSignf * 2 * x1;
                    ii -%= xNSign * prime_x;
                } else if (ay0 > ax0 and ay0 >= az0) {
                    y1 += yNSignf;
                    b -= yNSignf * 2 * y1;
                    jj -%= yNSign * prime_y;
                } else {
                    z1 += zNSignf;
                    b -= zNSignf * 2 * z1;
                    kk -%= zNSign * prime_z;
                }

                if (b > 0) value += (b * b) * (b * b) * gradCoord3D(seed_value, ii, jj, kk, x1, y1, z1);

                if (l == 1) break;

                ax0 = 0.5 - ax0;
                ay0 = 0.5 - ay0;
                az0 = 0.5 - az0;

                x0 = xNSignf * ax0;
                y0 = yNSignf * ay0;
                z0 = zNSignf * az0;

                a += (0.75 - ax0) - (ay0 + az0);

                i +%= (xNSign >> 1) & prime_x;
                j +%= (yNSign >> 1) & prime_y;
                k +%= (zNSign >> 1) & prime_z;

                xNSign = -xNSign;
                yNSign = -yNSign;
                zNSign = -zNSign;

                seed_value = ~seed_value;
            }

            return value * 32.69428253173828125;
        }

        // Value noise

        fn singleValue2D(seed: i32, x: Float, y: Float) Float {
            var x0: i32 = fastFloor(x);
            var y0: i32 = fastFloor(y);

            const xs: Float = interpHermite(x - @as(Float, @floatFromInt(x0)));
            const ys: Float = interpHermite(y - @as(Float, @floatFromInt(y0)));

            x0 *%= prime_x;
            y0 *%= prime_y;
            const x1 = x0 +% prime_x;
            const y1 = y0 +% prime_y;

            const xf0: Float = lerp(valCoord2D(seed, x0, y0), valCoord2D(seed, x1, y0), xs);
            const xf1: Float = lerp(valCoord2D(seed, x0, y1), valCoord2D(seed, x1, y1), xs);

            return lerp(xf0, xf1, ys);
        }

        fn singleValue3D(seed: i32, x: Float, y: Float, z: Float) Float {
            var x0: i32 = fastFloor(x);
            var y0: i32 = fastFloor(y);
            var z0: i32 = fastFloor(z);

            const xs: Float = interpHermite(x - @as(Float, @floatFromInt(x0)));
            const ys: Float = interpHermite(y - @as(Float, @floatFromInt(y0)));
            const zs: Float = interpHermite(z - @as(Float, @floatFromInt(z0)));

            x0 *%= prime_x;
            y0 *%= prime_y;
            z0 *%= prime_z;
            const x1 = x0 +% prime_x;
            const y1 = y0 +% prime_y;
            const z1 = z0 +% prime_z;

            const xf00: Float = lerp(valCoord3D(seed, x0, y0, z0), valCoord3D(seed, x1, y0, z0), xs);
            const xf10: Float = lerp(valCoord3D(seed, x0, y1, z0), valCoord3D(seed, x1, y1, z0), xs);
            const xf01: Float = lerp(valCoord3D(seed, x0, y0, z1), valCoord3D(seed, x1, y0, z1), xs);
            const xf11: Float = lerp(valCoord3D(seed, x0, y1, z1), valCoord3D(seed, x1, y1, z1), xs);

            const yf0: Float = lerp(xf00, xf10, ys);
            const yf1: Float = lerp(xf01, xf11, ys);

            return lerp(yf0, yf1, zs);
        }

        // Value Cubic

        fn singleValueCubic2D(seed: i32, x: Float, y: Float) Float {
            var x1: i32 = fastFloor(x);
            var y1: i32 = fastFloor(y);

            const xs: Float = x - @as(Float, @floatFromInt(x1));
            const ys: Float = y - @as(Float, @floatFromInt(y1));
            x1 *%= prime_x;
            y1 *%= prime_y;

            const x0 = x1 -% prime_x;
            const y0 = y1 -% prime_y;
            const x2 = x1 +% prime_x;
            const y2 = y1 +% prime_y;
            const x3 = x1 +% prime_x_shl1;
            const y3 = y1 +% prime_y_shl1;

            return cubicLerp(cubicLerp(valCoord2D(seed, x0, y0), valCoord2D(seed, x1, y0), valCoord2D(seed, x2, y0), valCoord2D(seed, x3, y0), xs), cubicLerp(valCoord2D(seed, x0, y1), valCoord2D(seed, x1, y1), valCoord2D(seed, x2, y1), valCoord2D(seed, x3, y1), xs), cubicLerp(valCoord2D(seed, x0, y2), valCoord2D(seed, x1, y2), valCoord2D(seed, x2, y2), valCoord2D(seed, x3, y2), xs), cubicLerp(valCoord2D(seed, x0, y3), valCoord2D(seed, x1, y3), valCoord2D(seed, x2, y3), valCoord2D(seed, x3, y3), xs), ys) * (1.0 / (1.5 * 1.5));
        }

        fn singleValueCubic3D(seed: i32, x: Float, y: Float, z: Float) Float {
            var x1: i32 = fastFloor(x);
            var y1: i32 = fastFloor(y);
            var z1: i32 = fastFloor(z);

            const xs: Float = x - @as(Float, @floatFromInt(x1));
            const ys: Float = y - @as(Float, @floatFromInt(y1));
            const zs: Float = z - @as(Float, @floatFromInt(z1));
            x1 *%= prime_x;
            y1 *%= prime_y;
            z1 *%= prime_z;

            const x0 = x1 -% prime_x;
            const y0 = y1 -% prime_y;
            const z0 = z1 -% prime_z;

            const x2 = x1 +% prime_x;
            const y2 = y1 +% prime_y;
            const z2 = z1 +% prime_z;

            const x3 = x1 +% prime_x_shl1;
            const y3 = y1 +% prime_y_shl1;
            const z3 = z1 +% prime_z_shl1;

            return cubicLerp(cubicLerp(cubicLerp(valCoord3D(seed, x0, y0, z0), valCoord3D(seed, x1, y0, z0), valCoord3D(seed, x2, y0, z0), valCoord3D(seed, x3, y0, z0), xs), cubicLerp(valCoord3D(seed, x0, y1, z0), valCoord3D(seed, x1, y1, z0), valCoord3D(seed, x2, y1, z0), valCoord3D(seed, x3, y1, z0), xs), cubicLerp(valCoord3D(seed, x0, y2, z0), valCoord3D(seed, x1, y2, z0), valCoord3D(seed, x2, y2, z0), valCoord3D(seed, x3, y2, z0), xs), cubicLerp(valCoord3D(seed, x0, y3, z0), valCoord3D(seed, x1, y3, z0), valCoord3D(seed, x2, y3, z0), valCoord3D(seed, x3, y3, z0), xs), ys), cubicLerp(cubicLerp(valCoord3D(seed, x0, y0, z1), valCoord3D(seed, x1, y0, z1), valCoord3D(seed, x2, y0, z1), valCoord3D(seed, x3, y0, z1), xs), cubicLerp(valCoord3D(seed, x0, y1, z1), valCoord3D(seed, x1, y1, z1), valCoord3D(seed, x2, y1, z1), valCoord3D(seed, x3, y1, z1), xs), cubicLerp(valCoord3D(seed, x0, y2, z1), valCoord3D(seed, x1, y2, z1), valCoord3D(seed, x2, y2, z1), valCoord3D(seed, x3, y2, z1), xs), cubicLerp(valCoord3D(seed, x0, y3, z1), valCoord3D(seed, x1, y3, z1), valCoord3D(seed, x2, y3, z1), valCoord3D(seed, x3, y3, z1), xs), ys), cubicLerp(cubicLerp(valCoord3D(seed, x0, y0, z2), valCoord3D(seed, x1, y0, z2), valCoord3D(seed, x2, y0, z2), valCoord3D(seed, x3, y0, z2), xs), cubicLerp(valCoord3D(seed, x0, y1, z2), valCoord3D(seed, x1, y1, z2), valCoord3D(seed, x2, y1, z2), valCoord3D(seed, x3, y1, z2), xs), cubicLerp(valCoord3D(seed, x0, y2, z2), valCoord3D(seed, x1, y2, z2), valCoord3D(seed, x2, y2, z2), valCoord3D(seed, x3, y2, z2), xs), cubicLerp(valCoord3D(seed, x0, y3, z2), valCoord3D(seed, x1, y3, z2), valCoord3D(seed, x2, y3, z2), valCoord3D(seed, x3, y3, z2), xs), ys), cubicLerp(cubicLerp(valCoord3D(seed, x0, y0, z3), valCoord3D(seed, x1, y0, z3), valCoord3D(seed, x2, y0, z3), valCoord3D(seed, x3, y0, z3), xs), cubicLerp(valCoord3D(seed, x0, y1, z3), valCoord3D(seed, x1, y1, z3), valCoord3D(seed, x2, y1, z3), valCoord3D(seed, x3, y1, z3), xs), cubicLerp(valCoord3D(seed, x0, y2, z3), valCoord3D(seed, x1, y2, z3), valCoord3D(seed, x2, y2, z3), valCoord3D(seed, x3, y2, z3), xs), cubicLerp(valCoord3D(seed, x0, y3, z3), valCoord3D(seed, x1, y3, z3), valCoord3D(seed, x2, y3, z3), valCoord3D(seed, x3, y3, z3), xs), ys), zs) * (1.0 / (1.5 * 1.5 * 1.5));
        }

        // OpenSimplex2S Noise

        fn singleSimplexS2D(seed: i32, x: Float, y: Float) Float {
            // 2D OpenSimplex2S case is a modified 2D simplex noise.

            var i: i32 = fastFloor(x);
            var j: i32 = fastFloor(y);
            const xi: Float = x - @as(Float, @floatFromInt(i));
            const yi: Float = y - @as(Float, @floatFromInt(j));

            i *%= prime_x;
            j *%= prime_y;
            const ii = i +% prime_x;
            const jj = j +% prime_y;

            const t: Float = (xi + yi) * g2;
            const x0: Float = xi - t;
            const y0: Float = yi - t;

            const a0: Float = (2.0 / 3.0) - x0 * x0 - y0 * y0;
            var value: Float = (a0 * a0) * (a0 * a0) * gradCoord2D(seed, i, j, x0, y0);

            const a1: Float = (2.0 * (1.0 - 2.0 * g2) * (1.0 / g2 - 2.0)) * t + ((-2.0 * (1.0 - 2.0 * g2) * (1.0 - 2.0 * g2)) + a0);
            const x1: Float = x0 - (1.0 - 2.0 * g2);
            const y1: Float = y0 - (1.0 - 2.0 * g2);
            value += (a1 * a1) * (a1 * a1) * gradCoord2D(seed, ii, jj, x1, y1);

            // Nested conditionals were faster than compact bit logic/arithmetic.
            const xmyi: Float = xi - yi;
            if (t > g2) {
                if (xi + xmyi > 1) {
                    const x2: Float = x0 + (3.0 * g2 - 2.0);
                    const y2: Float = y0 + (3.0 * g2 - 1.0);
                    const a2: Float = (2.0 / 3.0) - x2 * x2 - y2 * y2;
                    if (a2 > 0) {
                        value += (a2 * a2) * (a2 * a2) * gradCoord2D(seed, i +% prime_x_shl1, j +% prime_y, x2, y2);
                    }
                } else {
                    const x2: Float = x0 + g2;
                    const y2: Float = y0 + (g2 - 1.0);
                    const a2: Float = (2.0 / 3.0) - x2 * x2 - y2 * y2;
                    if (a2 > 0) {
                        value += (a2 * a2) * (a2 * a2) * gradCoord2D(seed, i, j +% prime_y, x2, y2);
                    }
                }

                if (yi - xmyi > 1) {
                    const x3: Float = x0 + (3.0 * g2 - 1.0);
                    const y3: Float = y0 + (3.0 * g2 - 2.0);
                    const a3: Float = (2.0 / 3.0) - x3 * x3 - y3 * y3;
                    if (a3 > 0) {
                        value += (a3 * a3) * (a3 * a3) * gradCoord2D(seed, i +% prime_x, j +% prime_y_shl1, x3, y3);
                    }
                } else {
                    const x3: Float = x0 + (g2 - 1.0);
                    const y3: Float = y0 + g2;
                    const a3: Float = (2.0 / 3.0) - x3 * x3 - y3 * y3;
                    if (a3 > 0) {
                        value += (a3 * a3) * (a3 * a3) * gradCoord2D(seed, i +% prime_x, j, x3, y3);
                    }
                }
            } else {
                if (xi + xmyi < 0) {
                    const x2: Float = x0 + (1.0 - g2);
                    const y2: Float = y0 - g2;
                    const a2: Float = (2.0 / 3.0) - x2 * x2 - y2 * y2;
                    if (a2 > 0) {
                        value += (a2 * a2) * (a2 * a2) * gradCoord2D(seed, i -% prime_x, j, x2, y2);
                    }
                } else {
                    const x2: Float = x0 + (g2 - 1.0);
                    const y2: Float = y0 + g2;
                    const a2: Float = (2.0 / 3.0) - x2 * x2 - y2 * y2;
                    if (a2 > 0) {
                        value += (a2 * a2) * (a2 * a2) * gradCoord2D(seed, i +% prime_x, j, x2, y2);
                    }
                }

                if (yi < xmyi) {
                    const x2: Float = x0 - g2;
                    const y2: Float = y0 - (g2 - 1.0);
                    const a2: Float = (2.0 / 3.0) - x2 * x2 - y2 * y2;
                    if (a2 > 0) {
                        value += (a2 * a2) * (a2 * a2) * gradCoord2D(seed, i, j -% prime_y, x2, y2);
                    }
                } else {
                    const x2: Float = x0 + g2;
                    const y2: Float = y0 + (g2 - 1.0);
                    const a2: Float = (2.0 / 3.0) - x2 * x2 - y2 * y2;
                    if (a2 > 0) {
                        value += (a2 * a2) * (a2 * a2) * gradCoord2D(seed, i, j +% prime_y, x2, y2);
                    }
                }
            }

            return value * 18.24196194486065;
        }

        fn singleSimplexS3D(seed: i32, x: Float, y: Float, z: Float) Float {
            // 3D OpenSimplex2S case uses two offset rotated cube grids.

            var i: i32 = fastFloor(x);
            var j: i32 = fastFloor(y);
            var k: i32 = fastFloor(z);
            const xi: Float = x - @as(Float, @floatFromInt(i));
            const yi: Float = y - @as(Float, @floatFromInt(j));
            const zi: Float = z - @as(Float, @floatFromInt(k));
            i *%= prime_x;
            j *%= prime_y;
            k *%= prime_z;

            const seed2 = seed +% 1293373;
            const xNMask: i32 = @intFromFloat(-0.5 - xi);
            const yNMask: i32 = @intFromFloat(-0.5 - yi);
            const zNMask: i32 = @intFromFloat(-0.5 - zi);

            const x0: Float = xi + @as(Float, @floatFromInt(xNMask));
            const y0: Float = yi + @as(Float, @floatFromInt(yNMask));
            const z0: Float = zi + @as(Float, @floatFromInt(zNMask));
            const a0: Float = 0.75 - x0 * x0 - y0 * y0 - z0 * z0;
            var value: Float = (a0 * a0) * (a0 * a0) * gradCoord3D(seed, i +% (xNMask & prime_x), j +% (yNMask & prime_y), k +% (zNMask & prime_z), x0, y0, z0);

            const x1 = xi - 0.5;
            const y1 = yi - 0.5;
            const z1 = zi - 0.5;
            const a1 = 0.75 - x1 * x1 - y1 * y1 - z1 * z1;
            value += (a1 * a1) * (a1 * a1) * gradCoord3D(seed2, i +% prime_x, j +% prime_y, k +% prime_z, x1, y1, z1);

            const xAFlipMask0: Float = @as(Float, @floatFromInt((xNMask | 1) << 1)) * x1;
            const yAFlipMask0: Float = @as(Float, @floatFromInt((yNMask | 1) << 1)) * y1;
            const zAFlipMask0: Float = @as(Float, @floatFromInt((zNMask | 1) << 1)) * z1;
            const xAFlipMask1: Float = @as(Float, @floatFromInt(-2 - (xNMask << 2))) * x1 - 1.0;
            const yAFlipMask1: Float = @as(Float, @floatFromInt(-2 - (yNMask << 2))) * y1 - 1.0;
            const zAFlipMask1: Float = @as(Float, @floatFromInt(-2 - (zNMask << 2))) * z1 - 1.0;

            var skip_5 = false;
            const a2: Float = xAFlipMask0 + a0;
            if (a2 > 0) {
                const x2: Float = x0 - @as(Float, @floatFromInt(xNMask | 1));
                const y2: Float = y0;
                const z2: Float = z0;
                value += (a2 * a2) * (a2 * a2) * gradCoord3D(seed, i +% (~xNMask & prime_x), j +% (yNMask & prime_y), k +% (zNMask & prime_z), x2, y2, z2);
            } else {
                const a3: Float = yAFlipMask0 + zAFlipMask0 + a0;
                if (a3 > 0) {
                    const x3: Float = x0;
                    const y3: Float = y0 - @as(Float, @floatFromInt(yNMask | 1));
                    const z3: Float = z0 - @as(Float, @floatFromInt(zNMask | 1));
                    value += (a3 * a3) * (a3 * a3) * gradCoord3D(seed, i +% (xNMask & prime_x), j +% (~yNMask & prime_y), k +% (~zNMask & prime_z), x3, y3, z3);
                }

                const a4: Float = xAFlipMask1 + a1;
                if (a4 > 0) {
                    const x4: Float = @as(Float, @floatFromInt(xNMask | 1)) + x1;
                    const y4: Float = y1;
                    const z4: Float = z1;
                    value += (a4 * a4) * (a4 * a4) * gradCoord3D(seed2, i +% (xNMask & (prime_x *% 2)), j +% prime_y, k +% prime_z, x4, y4, z4);
                    skip_5 = true;
                }
            }

            var skip_9 = false;
            const a6: Float = yAFlipMask0 + a0;
            if (a6 > 0) {
                const x6: Float = x0;
                const y6: Float = y0 - @as(Float, @floatFromInt(yNMask | 1));
                const z6: Float = z0;
                value += (a6 * a6) * (a6 * a6) * gradCoord3D(seed, i +% (xNMask & prime_x), j +% (~yNMask & prime_y), k +% (zNMask & prime_z), x6, y6, z6);
            } else {
                const a7: Float = xAFlipMask0 + zAFlipMask0 + a0;
                if (a7 > 0) {
                    const x7: Float = x0 - @as(Float, @floatFromInt(xNMask | 1));
                    const y7: Float = y0;
                    const z7: Float = z0 - @as(Float, @floatFromInt(zNMask | 1));
                    value += (a7 * a7) * (a7 * a7) * gradCoord3D(seed, i +% (~xNMask & prime_x), j +% (yNMask & prime_y), k +% (~zNMask & prime_z), x7, y7, z7);
                }

                const a8: Float = yAFlipMask1 + a1;
                if (a8 > 0) {
                    const x8: Float = x1;
                    const y8: Float = @as(Float, @floatFromInt(yNMask | 1)) + y1;
                    const z8: Float = z1;
                    value += (a8 * a8) * (a8 * a8) * gradCoord3D(seed2, i +% prime_x, j +% (yNMask & prime_y_shl1), k +% prime_z, x8, y8, z8);
                    skip_9 = true;
                }
            }

            var skip_d = false;
            const aA: Float = zAFlipMask0 + a0;
            if (aA > 0) {
                const xA: Float = x0;
                const yA: Float = y0;
                const zA: Float = z0 - @as(Float, @floatFromInt(zNMask | 1));
                value += (aA * aA) * (aA * aA) * gradCoord3D(seed, i +% (xNMask & prime_x), j +% (yNMask & prime_y), k +% (~zNMask & prime_z), xA, yA, zA);
            } else {
                const aB: Float = xAFlipMask0 + yAFlipMask0 + a0;
                if (aB > 0) {
                    const xB: Float = x0 - @as(Float, @floatFromInt(xNMask | 1));
                    const yB: Float = y0 - @as(Float, @floatFromInt(yNMask | 1));
                    const zB: Float = z0;
                    value += (aB * aB) * (aB * aB) * gradCoord3D(seed, i +% (~xNMask & prime_x), j +% (~yNMask & prime_y), k +% (zNMask & prime_z), xB, yB, zB);
                }

                const aC: Float = zAFlipMask1 + a1;
                if (aC > 0) {
                    const xC: Float = x1;
                    const yC: Float = y1;
                    const zC: Float = @as(Float, @floatFromInt(zNMask | 1)) + z1;
                    value += (aC * aC) * (aC * aC) * gradCoord3D(seed2, i +% prime_x, j +% prime_y, k +% (zNMask & (prime_z_shl1)), xC, yC, zC);
                    skip_d = true;
                }
            }

            if (!skip_5) {
                const a5: Float = yAFlipMask1 + zAFlipMask1 + a1;
                if (a5 > 0) {
                    const x5: Float = x1;
                    const y5: Float = @as(Float, @floatFromInt(yNMask | 1)) + y1;
                    const z5: Float = @as(Float, @floatFromInt(zNMask | 1)) + z1;
                    value += (a5 * a5) * (a5 * a5) * gradCoord3D(seed2, i +% prime_x, j +% (yNMask & (prime_y_shl1)), k +% (zNMask & (prime_z_shl1)), x5, y5, z5);
                }
            }

            if (!skip_9) {
                const a9: Float = xAFlipMask1 + zAFlipMask1 + a1;
                if (a9 > 0) {
                    const x9: Float = @as(Float, @floatFromInt(xNMask | 1)) + x1;
                    const y9: Float = y1;
                    const z9: Float = @as(Float, @floatFromInt(zNMask | 1)) + z1;
                    value += (a9 * a9) * (a9 * a9) * gradCoord3D(seed2, i +% (xNMask & (prime_x *% 2)), j +% prime_y, k +% (zNMask & (prime_z_shl1)), x9, y9, z9);
                }
            }

            if (!skip_d) {
                const aD: Float = xAFlipMask1 + yAFlipMask1 + a1;
                if (aD > 0) {
                    const xD: Float = @as(Float, @floatFromInt(xNMask | 1)) + x1;
                    const yD: Float = @as(Float, @floatFromInt(yNMask | 1)) + y1;
                    const zD: Float = z1;
                    value += (aD * aD) * (aD * aD) * gradCoord3D(seed2, i +% (xNMask & (prime_x_shl1)), j +% (yNMask & (prime_y_shl1)), k +% prime_z, xD, yD, zD);
                }
            }

            return value * 9.046026385208288;
        }

        inline fn fastSqrt(f: f32) f32 {
            const xhalf = 0.5 * f;
            var a: f32 = @bitCast(0x5F3759DF - (@as(u32, @bitCast(f)) >> 1));
            a = a * (1.5 - xhalf * a * a);

            return f * a;
        }

        // Cellular Noise

        fn singleCellular2D(self: *const State, seed: i32, x: Float, y: Float) Float {
            const xr: i32 = fastRound(x);
            const yr: i32 = fastRound(y);

            var dist0 = std.math.floatMax(Float);
            var dist1 = std.math.floatMax(Float);
            var closest_hash: i32 = 0;

            const jitter = 0.43701595 * self.cellular_jitter_mod;

            var x_primed: i32 = (xr - 1) *% prime_x;
            const y_primed_base = (yr - 1) *% prime_y;
            var xi = xr - 1;

            switch (self.cellular_distance) {
                .euclidean, .euclidean_sq => while (xi <= xr + 1) : (xi += 1) {
                    var y_primed = y_primed_base;
                    var yi = yr - 1;
                    while (yi <= yr + 1) : (yi += 1) {
                        const hash = hash2D(seed, x_primed, y_primed);
                        const idx: usize = @intCast(hash & (255 << 1));
                        const vx: Float = (@as(Float, @floatFromInt(xi)) - x) + rand_2d[idx] * jitter;
                        const vy: Float = (@as(Float, @floatFromInt(yi)) - y) + rand_2d[idx | 1] * jitter;
                        const new_dist: Float = (vx * vx) + (vy * vy);

                        dist1 = @max(@min(dist1, new_dist), dist0);
                        if (new_dist < dist0) {
                            dist0 = new_dist;
                            closest_hash = hash;
                        }
                        y_primed +%= prime_y;
                    }
                    x_primed +%= prime_x;
                },
                .manhattan => while (xi <= xr + 1) : (xi += 1) {
                    var y_primed = y_primed_base;
                    var yi = yr - 1;
                    while (yi <= yr + 1) : (yi += 1) {
                        const hash = hash2D(seed, x_primed, y_primed);
                        const idx: usize = @intCast(hash & (255 << 1));
                        const vx: Float = (@as(Float, @floatFromInt(xi)) - x) + (rand_2d[idx] * jitter);
                        const vy: Float = (@as(Float, @floatFromInt(yi)) - y) + (rand_2d[idx | 1] * jitter);
                        const new_dist: Float = @abs(vx) + @abs(vy);

                        dist1 = @max(@min(dist1, new_dist), dist0);
                        if (new_dist < dist0) {
                            dist0 = new_dist;
                            closest_hash = hash;
                        }
                        y_primed +%= prime_y;
                    }
                    x_primed +%= prime_x;
                },
                .hybrid => while (xi <= xr + 1) : (xi += 1) {
                    var y_primed = y_primed_base;
                    var yi = yr - 1;
                    while (yi <= yr + 1) : (yi += 1) {
                        const hash = hash2D(seed, x_primed, y_primed);
                        const idx: usize = @intCast(hash & (255 << 1));
                        const vx: Float = (@as(Float, @floatFromInt(xi)) - x) + (rand_2d[idx] * jitter);
                        const vy: Float = (@as(Float, @floatFromInt(yi)) - y) + (rand_2d[idx | 1] * jitter);
                        const new_dist: Float = (@abs(vx) + @abs(vy)) + ((vx * vx) + (vy * vy));

                        dist1 = @max(@min(dist1, new_dist), dist0);
                        if (new_dist < dist0) {
                            dist0 = new_dist;
                            closest_hash = hash;
                        }
                        y_primed +%= prime_y;
                    }
                    x_primed +%= prime_x;
                },
            }

            std.debug.assert(dist0 >= 0 and dist0 < dist1);
            if (self.cellular_distance == .euclidean and self.cellular_return != .cell_value) {
                dist0 = @sqrt(dist0);
                if (self.cellular_return != .distance) dist1 = @sqrt(dist1);
            }

            const result = switch (self.cellular_return) {
                .cell_value => @as(Float, @floatFromInt(closest_hash)) * (1.0 / 2147483648.0),
                .distance => dist0 - 1.0,
                .distance2 => dist1 - 1.0,
                .distance2_add => ((dist1 + dist0) * 0.5) - 1.0,
                .distance2_sub => (dist1 - dist0) - 1.0,
                .distance2_mul => ((dist1 * dist0) * 0.5) - 1.0,
                .distance2_div => (dist0 / dist1) - 1.0,
            };

            // "hybrid" can result in out of range values
            // std.debug.assert(temp >= -1.0 and temp <= 1.0);
            return @max(-1.0, @min(1.0, result));
        }

        fn singleCellular3D(state: *const State, seed: i32, x: Float, y: Float, z: Float) Float {
            const xr: i32 = fastRound(x);
            const yr: i32 = fastRound(y);
            const zr: i32 = fastRound(z);

            var dist0 = std.math.floatMax(Float);
            var dist1 = std.math.floatMax(Float);
            var closest_hash: i32 = 0;

            const jitter = 0.39614353 * state.cellular_jitter_mod;
            var x_primed: i32 = (xr - 1) *% prime_x;
            const y_primed_base = (yr - 1) *% prime_y;
            const z_primed_base = (zr - 1) *% prime_z;
            var xi = xr - 1;

            switch (state.cellular_distance) {
                .euclidean, .euclidean_sq => while (xi <= xr + 1) : (xi += 1) {
                    var y_primed = y_primed_base;
                    var yi = yr - 1;
                    while (yi <= yr + 1) : (yi += 1) {
                        var z_primed = z_primed_base;
                        var zi = zr - 1;
                        while (zi <= zr + 1) : (zi += 1) {
                            const hash = hash3D(seed, x_primed, y_primed, z_primed);
                            const idx: usize = @intCast(hash & (255 << 2));
                            const vx: Float = (@as(Float, @floatFromInt(xi)) - x) + rand_3d[idx] * jitter;
                            const vy: Float = (@as(Float, @floatFromInt(yi)) - y) + rand_3d[idx | 1] * jitter;
                            const vz: Float = (@as(Float, @floatFromInt(zi)) - z) + rand_3d[idx | 2] * jitter;

                            const new_dist = vx * vx + vy * vy + vz * vz;
                            dist1 = @max(@min(dist1, new_dist), dist0);
                            if (new_dist < dist0) {
                                dist0 = new_dist;
                                closest_hash = hash;
                            }
                            z_primed +%= prime_z;
                        }
                        y_primed +%= prime_y;
                    }
                    x_primed +%= prime_x;
                },
                .manhattan => while (xi <= xr + 1) : (xi += 1) {
                    var y_primed = y_primed_base;
                    var yi = yr - 1;
                    while (yi <= yr + 1) : (yi += 1) {
                        var z_primed = z_primed_base;
                        var zi = zr - 1;
                        while (zi <= zr + 1) : (zi += 1) {
                            const hash = hash3D(seed, x_primed, y_primed, z_primed);
                            const idx: usize = @intCast(hash & (255 << 2));
                            const vx: Float = (@as(Float, @floatFromInt(xi)) - x) + rand_3d[idx] * jitter;
                            const vy: Float = (@as(Float, @floatFromInt(yi)) - y) + rand_3d[idx | 1] * jitter;
                            const vz: Float = (@as(Float, @floatFromInt(zi)) - z) + rand_3d[idx | 2] * jitter;
                            const new_dist = @abs(vx) + @abs(vy) + @abs(vz);

                            dist1 = @max(@min(dist1, new_dist), dist0);
                            if (new_dist < dist0) {
                                dist0 = new_dist;
                                closest_hash = hash;
                            }
                            z_primed +%= prime_z;
                        }
                        y_primed +%= prime_y;
                    }
                    x_primed +%= prime_x;
                },
                .hybrid => while (xi <= xr + 1) : (xi += 1) {
                    var y_primed = y_primed_base;
                    var yi = yr - 1;
                    while (yi <= yr + 1) : (yi += 1) {
                        var z_primed = z_primed_base;
                        var zi = zr - 1;
                        while (zi <= zr + 1) : (zi += 1) {
                            const hash = hash3D(seed, x_primed, y_primed, z_primed);
                            const idx: usize = @intCast(hash & (255 << 2));
                            const vx: Float = (@as(Float, @floatFromInt(xi)) - x) + rand_3d[idx] * jitter;
                            const vy: Float = (@as(Float, @floatFromInt(yi)) - y) + rand_3d[idx | 1] * jitter;
                            const vz: Float = (@as(Float, @floatFromInt(zi)) - z) + rand_3d[idx | 2] * jitter;

                            const new_dist = (@abs(vx) + @abs(vy) + @abs(vz)) + (vx * vx + vy * vy + vz * vz);
                            dist1 = @max(@min(dist1, new_dist), dist0);
                            if (new_dist < dist0) {
                                dist0 = new_dist;
                                closest_hash = hash;
                            }
                            z_primed +%= prime_z;
                        }
                        y_primed +%= prime_y;
                    }
                    x_primed +%= prime_x;
                },
            }

            if (state.cellular_distance == .euclidean and state.cellular_return != .cell_value) {
                dist0 = @sqrt(dist0);
                if (state.cellular_return != .distance) dist1 = @sqrt(dist1);
            }

            const value = switch (state.cellular_return) {
                .cell_value => @as(Float, @floatFromInt(closest_hash)) * (1.0 / 2147483648.0),
                .distance => dist0 - 1,
                .distance2 => dist1 - 1,
                .distance2_add => ((dist1 + dist0) * 0.5) - 1,
                .distance2_sub => dist1 - dist0 - 1,
                .distance2_mul => (dist1 * dist0 * 0.5) - 1,
                .distance2_div => (dist0 / dist1) - 1,
            };
            return @max(-1.0, @min(1.0, value));
        }

        const gradients_2d = [256]Float{
            0.130526192220052,  0.99144486137381,   0.38268343236509,   0.923879532511287,  0.608761429008721,  0.793353340291235,  0.793353340291235,  0.608761429008721,
            0.923879532511287,  0.38268343236509,   0.99144486137381,   0.130526192220051,  0.99144486137381,   -0.130526192220051, 0.923879532511287,  -0.38268343236509,
            0.793353340291235,  -0.60876142900872,  0.608761429008721,  -0.793353340291235, 0.38268343236509,   -0.923879532511287, 0.130526192220052,  -0.99144486137381,
            -0.130526192220052, -0.99144486137381,  -0.38268343236509,  -0.923879532511287, -0.608761429008721, -0.793353340291235, -0.793353340291235, -0.608761429008721,
            -0.923879532511287, -0.38268343236509,  -0.99144486137381,  -0.130526192220052, -0.99144486137381,  0.130526192220051,  -0.923879532511287, 0.38268343236509,
            -0.793353340291235, 0.608761429008721,  -0.608761429008721, 0.793353340291235,  -0.38268343236509,  0.923879532511287,  -0.130526192220052, 0.99144486137381,
            0.130526192220052,  0.99144486137381,   0.38268343236509,   0.923879532511287,  0.608761429008721,  0.793353340291235,  0.793353340291235,  0.608761429008721,
            0.923879532511287,  0.38268343236509,   0.99144486137381,   0.130526192220051,  0.99144486137381,   -0.130526192220051, 0.923879532511287,  -0.38268343236509,
            0.793353340291235,  -0.60876142900872,  0.608761429008721,  -0.793353340291235, 0.38268343236509,   -0.923879532511287, 0.130526192220052,  -0.99144486137381,
            -0.130526192220052, -0.99144486137381,  -0.38268343236509,  -0.923879532511287, -0.608761429008721, -0.793353340291235, -0.793353340291235, -0.608761429008721,
            -0.923879532511287, -0.38268343236509,  -0.99144486137381,  -0.130526192220052, -0.99144486137381,  0.130526192220051,  -0.923879532511287, 0.38268343236509,
            -0.793353340291235, 0.608761429008721,  -0.608761429008721, 0.793353340291235,  -0.38268343236509,  0.923879532511287,  -0.130526192220052, 0.99144486137381,
            0.130526192220052,  0.99144486137381,   0.38268343236509,   0.923879532511287,  0.608761429008721,  0.793353340291235,  0.793353340291235,  0.608761429008721,
            0.923879532511287,  0.38268343236509,   0.99144486137381,   0.130526192220051,  0.99144486137381,   -0.130526192220051, 0.923879532511287,  -0.38268343236509,
            0.793353340291235,  -0.60876142900872,  0.608761429008721,  -0.793353340291235, 0.38268343236509,   -0.923879532511287, 0.130526192220052,  -0.99144486137381,
            -0.130526192220052, -0.99144486137381,  -0.38268343236509,  -0.923879532511287, -0.608761429008721, -0.793353340291235, -0.793353340291235, -0.608761429008721,
            -0.923879532511287, -0.38268343236509,  -0.99144486137381,  -0.130526192220052, -0.99144486137381,  0.130526192220051,  -0.923879532511287, 0.38268343236509,
            -0.793353340291235, 0.608761429008721,  -0.608761429008721, 0.793353340291235,  -0.38268343236509,  0.923879532511287,  -0.130526192220052, 0.99144486137381,
            0.130526192220052,  0.99144486137381,   0.38268343236509,   0.923879532511287,  0.608761429008721,  0.793353340291235,  0.793353340291235,  0.608761429008721,
            0.923879532511287,  0.38268343236509,   0.99144486137381,   0.130526192220051,  0.99144486137381,   -0.130526192220051, 0.923879532511287,  -0.38268343236509,
            0.793353340291235,  -0.60876142900872,  0.608761429008721,  -0.793353340291235, 0.38268343236509,   -0.923879532511287, 0.130526192220052,  -0.99144486137381,
            -0.130526192220052, -0.99144486137381,  -0.38268343236509,  -0.923879532511287, -0.608761429008721, -0.793353340291235, -0.793353340291235, -0.608761429008721,
            -0.923879532511287, -0.38268343236509,  -0.99144486137381,  -0.130526192220052, -0.99144486137381,  0.130526192220051,  -0.923879532511287, 0.38268343236509,
            -0.793353340291235, 0.608761429008721,  -0.608761429008721, 0.793353340291235,  -0.38268343236509,  0.923879532511287,  -0.130526192220052, 0.99144486137381,
            0.130526192220052,  0.99144486137381,   0.38268343236509,   0.923879532511287,  0.608761429008721,  0.793353340291235,  0.793353340291235,  0.608761429008721,
            0.923879532511287,  0.38268343236509,   0.99144486137381,   0.130526192220051,  0.99144486137381,   -0.130526192220051, 0.923879532511287,  -0.38268343236509,
            0.793353340291235,  -0.60876142900872,  0.608761429008721,  -0.793353340291235, 0.38268343236509,   -0.923879532511287, 0.130526192220052,  -0.99144486137381,
            -0.130526192220052, -0.99144486137381,  -0.38268343236509,  -0.923879532511287, -0.608761429008721, -0.793353340291235, -0.793353340291235, -0.608761429008721,
            -0.923879532511287, -0.38268343236509,  -0.99144486137381,  -0.130526192220052, -0.99144486137381,  0.130526192220051,  -0.923879532511287, 0.38268343236509,
            -0.793353340291235, 0.608761429008721,  -0.608761429008721, 0.793353340291235,  -0.38268343236509,  0.923879532511287,  -0.130526192220052, 0.99144486137381,
            0.38268343236509,   0.923879532511287,  0.923879532511287,  0.38268343236509,   0.923879532511287,  -0.38268343236509,  0.38268343236509,   -0.923879532511287,
            -0.38268343236509,  -0.923879532511287, -0.923879532511287, -0.38268343236509,  -0.923879532511287, 0.38268343236509,   -0.38268343236509,  0.923879532511287,
        };

        const rand_2d = [512]Float{
            -0.2700222198,   -0.9628540911, 0.3863092627,  -0.9223693152, 0.04444859006,  -0.999011673,  -0.5992523158, -0.8005602176, -0.7819280288,  0.6233687174,   0.9464672271,   0.3227999196,   -0.6514146797,  -0.7587218957,  0.9378472289,  0.347048376,
            -0.8497875957,   -0.5271252623, -0.879042592,  0.4767432447,  -0.892300288,   -0.4514423508, -0.379844434,  -0.9250503802, -0.9951650832,  0.0982163789,   0.7724397808,   -0.6350880136,  0.7573283322,   -0.6530343002,  -0.9928004525, -0.119780055,
            -0.0532665713,   0.9985803285,  0.9754253726,  -0.2203300762, -0.7665018163,  0.6422421394,  0.991636706,   0.1290606184,  -0.994696838,   0.1028503788,   -0.5379205513,  -0.84299554,    0.5022815471,   -0.8647041387,  0.4559821461,  -0.8899889226,
            -0.8659131224,   -0.5001944266, 0.0879458407,  -0.9961252577, -0.5051684983,  0.8630207346,  0.7753185226,  -0.6315704146, -0.6921944612,  0.7217110418,   -0.5191659449,  -0.8546734591,  0.8978622882,   -0.4402764035,  -0.1706774107, 0.9853269617,
            -0.9353430106,   -0.3537420705, -0.9992404798, 0.03896746794, -0.2882064021,  -0.9575683108, -0.9663811329, 0.2571137995,  -0.8759714238,  -0.4823630009,  -0.8303123018,  -0.5572983775,  0.05110133755,  -0.9986934731,  -0.8558373281, -0.5172450752,
            0.09887025282,   0.9951003332,  0.9189016087,  0.3944867976,  -0.2439375892,  -0.9697909324, -0.8121409387, -0.5834613061, -0.9910431363,  0.1335421355,   0.8492423985,   -0.5280031709,  -0.9717838994,  -0.2358729591,  0.9949457207,  0.1004142068,
            0.6241065508,    -0.7813392434, 0.662910307,   0.7486988212,  -0.7197418176,  0.6942418282,  -0.8143370775, -0.5803922158, 0.104521054,    -0.9945226741,  -0.1065926113,  -0.9943027784,  0.445799684,    -0.8951327509,  0.105547406,   0.9944142724,
            -0.992790267,    0.1198644477,  -0.8334366408, 0.552615025,   0.9115561563,   -0.4111755999, 0.8285544909,  -0.5599084351, 0.7217097654,   -0.6921957921,  0.4940492677,   -0.8694339084,  -0.3652321272,  -0.9309164803,  -0.9696606758, 0.2444548501,
            0.08925509731,   -0.996008799,  0.5354071276,  -0.8445941083, -0.1053576186,  0.9944343981,  -0.9890284586, 0.1477251101,  0.004856104961, 0.9999882091,   0.9885598478,   0.1508291331,   0.9286129562,   -0.3710498316,  -0.5832393863, -0.8123003252,
            0.3015207509,    0.9534596146,  -0.9575110528, 0.2883965738,  0.9715802154,   -0.2367105511, 0.229981792,   0.9731949318,  0.955763816,    -0.2941352207,  0.740956116,    0.6715534485,   -0.9971513787,  -0.07542630764, 0.6905710663,  -0.7232645452,
            -0.290713703,    -0.9568100872, 0.5912777791,  -0.8064679708, -0.9454592212,  -0.325740481,  0.6664455681,  0.74555369,    0.6236134912,   0.7817328275,   0.9126993851,   -0.4086316587,  -0.8191762011,  0.5735419353,   -0.8812745759, -0.4726046147,
            0.9953313627,    0.09651672651, 0.9855650846,  -0.1692969699, -0.8495980887,  0.5274306472,  0.6174853946,  -0.7865823463, 0.8508156371,   0.52546432,     0.9985032451,   -0.05469249926, 0.1971371563,   -0.9803759185,  0.6607855748,  -0.7505747292,
            -0.03097494063,  0.9995201614,  -0.6731660801, 0.739491331,   -0.7195018362,  -0.6944905383, 0.9727511689,  0.2318515979,  0.9997059088,   -0.0242506907,  0.4421787429,   -0.8969269532,  0.9981350961,   -0.061043673,   -0.9173660799, -0.3980445648,
            -0.8150056635,   -0.5794529907, -0.8789331304, 0.4769450202,  0.0158605829,   0.999874213,   -0.8095464474, 0.5870558317,  -0.9165898907,  -0.3998286786,  -0.8023542565,  0.5968480938,   -0.5176737917,  0.8555780767,   -0.8154407307, -0.5788405779,
            0.4022010347,    -0.9155513791, -0.9052556868, -0.4248672045, 0.7317445619,   0.6815789728,  -0.5647632201, -0.8252529947, -0.8403276335,  -0.5420788397,  -0.9314281527,  0.363925262,    0.5238198472,   0.8518290719,   0.7432803869,  -0.6689800195,
            -0.985371561,    -0.1704197369, 0.4601468731,  0.88784281,    0.825855404,    0.5638819483,  0.6182366099,  0.7859920446,  0.8331502863,   -0.553046653,   0.1500307506,   0.9886813308,   -0.662330369,   -0.7492119075,  -0.668598664,  0.743623444,
            0.7025606278,    0.7116238924,  -0.5419389763, -0.8404178401, -0.3388616456,  0.9408362159,  0.8331530315,  0.5530425174,  -0.2989720662,  -0.9542618632,  0.2638522993,   0.9645630949,   0.124108739,    -0.9922686234,  -0.7282649308, -0.6852956957,
            0.6962500149,    0.7177993569,  -0.9183535368, 0.3957610156,  -0.6326102274,  -0.7744703352, -0.9331891859, -0.359385508,  -0.1153779357,  -0.9933216659,  0.9514974788,   -0.3076565421,  -0.08987977445, -0.9959526224,  0.6678496916,  0.7442961705,
            0.7952400393,    -0.6062947138, -0.6462007402, -0.7631674805, -0.2733598753,  0.9619118351,  0.9669590226,  -0.254931851,  -0.9792894595,  0.2024651934,   -0.5369502995,  -0.8436138784,  -0.270036471,   -0.9628500944,  -0.6400277131, 0.7683518247,
            -0.7854537493,   -0.6189203566, 0.06005905383, -0.9981948257, -0.02455770378, 0.9996984141,  -0.65983623,   0.751409442,   -0.6253894466,  -0.7803127835,  -0.6210408851,  -0.7837781695,  0.8348888491,   0.5504185768,   -0.1592275245, 0.9872419133,
            0.8367622488,    0.5475663786,  -0.8675753916, -0.4973056806, -0.2022662628,  -0.9793305667, 0.9399189937,  0.3413975472,  0.9877404807,   -0.1561049093,  -0.9034455656,  0.4287028224,   0.1269804218,   -0.9919052235,  -0.3819600854, 0.924178821,
            0.9754625894,    0.2201652486,  -0.3204015856, -0.9472818081, -0.9874760884,  0.1577687387,  0.02535348474, -0.9996785487, 0.4835130794,   -0.8753371362,  -0.2850799925,  -0.9585037287,  -0.06805516006, -0.99768156,    -0.7885244045, -0.6150034663,
            0.3185392127,    -0.9479096845, 0.8880043089,  0.4598351306,  0.6476921488,   -0.7619021462, 0.9820241299,  0.1887554194,  0.9357275128,   -0.3527237187,  -0.8894895414,  0.4569555293,   0.7922791302,   0.6101588153,   0.7483818261,  0.6632681526,
            -0.7288929755,   -0.6846276581, 0.8729032783,  -0.4878932944, 0.8288345784,   0.5594937369,  0.08074567077, 0.9967347374,  0.9799148216,   -0.1994165048,  -0.580730673,   -0.8140957471,  -0.4700049791,  -0.8826637636,  0.2409492979,  0.9705377045,
            0.9437816757,    -0.3305694308, -0.8927998638, -0.4504535528, -0.8069622304,  0.5906030467,  0.06258973166, 0.9980393407,  -0.9312597469,  0.3643559849,   0.5777449785,   0.8162173362,   -0.3360095855,  -0.941858566,   0.697932075,   -0.7161639607,
            -0.002008157227, -0.9999979837, -0.1827294312, -0.9831632392, -0.6523911722,  0.7578824173,  -0.4302626911, -0.9027037258, -0.9985126289,  -0.05452091251, -0.01028102172, -0.9999471489,  -0.4946071129,  0.8691166802,   -0.2999350194, 0.9539596344,
            0.8165471961,    0.5772786819,  0.2697460475,  0.962931498,   -0.7306287391,  -0.6827749597, -0.7590952064, -0.6509796216, -0.907053853,   0.4210146171,   -0.5104861064,  -0.8598860013,  0.8613350597,   0.5080373165,   0.5007881595,  -0.8655698812,
            -0.654158152,    0.7563577938,  -0.8382755311, -0.545246856,  0.6940070834,   0.7199681717,  0.06950936031, 0.9975812994,  0.1702942185,   -0.9853932612,  0.2695973274,   0.9629731466,   0.5519612192,   -0.8338697815,  0.225657487,   -0.9742067022,
            0.4215262855,    -0.9068161835, 0.4881873305,  -0.8727388672, -0.3683854996,  -0.9296731273, -0.9825390578, 0.1860564427,  0.81256471,     0.5828709909,   0.3196460933,   -0.9475370046,  0.9570913859,   0.2897862643,   -0.6876655497, -0.7260276109,
            -0.9988770922,   -0.047376731,  -0.1250179027, 0.992154486,   -0.8280133617,  0.560708367,   0.9324863769,  -0.3612051451, 0.6394653183,   0.7688199442,   -0.01623847064, -0.9998681473,  -0.9955014666,  -0.09474613458, -0.81453315,   0.580117012,
            0.4037327978,    -0.9148769469, 0.9944263371,  0.1054336766,  -0.1624711654,  0.9867132919,  -0.9949487814, -0.100383875,  -0.6995302564,  0.7146029809,   0.5263414922,   -0.85027327,    -0.5395221479,  0.841971408,    0.6579370318,  0.7530729462,
            0.01426758847,   -0.9998982128, -0.6734383991, 0.7392433447,  0.639412098,    -0.7688642071, 0.9211571421,  0.3891908523,  -0.146637214,   -0.9891903394,  -0.782318098,   0.6228791163,   -0.5039610839,  -0.8637263605,  -0.7743120191, -0.6328039957,
        };

        const gradients_3d = [256]Float{
            0, 1, 1, 0, 0,  -1, 1, 0, 0,  1,  -1, 0, 0,  -1, -1, 0,
            1, 0, 1, 0, -1, 0,  1, 0, 1,  0,  -1, 0, -1, 0,  -1, 0,
            1, 1, 0, 0, -1, 1,  0, 0, 1,  -1, 0,  0, -1, -1, 0,  0,
            0, 1, 1, 0, 0,  -1, 1, 0, 0,  1,  -1, 0, 0,  -1, -1, 0,
            1, 0, 1, 0, -1, 0,  1, 0, 1,  0,  -1, 0, -1, 0,  -1, 0,
            1, 1, 0, 0, -1, 1,  0, 0, 1,  -1, 0,  0, -1, -1, 0,  0,
            0, 1, 1, 0, 0,  -1, 1, 0, 0,  1,  -1, 0, 0,  -1, -1, 0,
            1, 0, 1, 0, -1, 0,  1, 0, 1,  0,  -1, 0, -1, 0,  -1, 0,
            1, 1, 0, 0, -1, 1,  0, 0, 1,  -1, 0,  0, -1, -1, 0,  0,
            0, 1, 1, 0, 0,  -1, 1, 0, 0,  1,  -1, 0, 0,  -1, -1, 0,
            1, 0, 1, 0, -1, 0,  1, 0, 1,  0,  -1, 0, -1, 0,  -1, 0,
            1, 1, 0, 0, -1, 1,  0, 0, 1,  -1, 0,  0, -1, -1, 0,  0,
            0, 1, 1, 0, 0,  -1, 1, 0, 0,  1,  -1, 0, 0,  -1, -1, 0,
            1, 0, 1, 0, -1, 0,  1, 0, 1,  0,  -1, 0, -1, 0,  -1, 0,
            1, 1, 0, 0, -1, 1,  0, 0, 1,  -1, 0,  0, -1, -1, 0,  0,
            1, 1, 0, 0, 0,  -1, 1, 0, -1, 1,  0,  0, 0,  -1, -1, 0,
        };

        const rand_3d = [1024]Float{
            -0.7292736885,  -0.6618439697,  0.1735581948,   0, 0.790292081,   -0.5480887466,  -0.2739291014,  0, 0.7217578935,   0.6226212466,  -0.3023380997,   0, 0.565683137,    -0.8208298145, -0.0790000257, 0, 0.760049034,   -0.5555979497, -0.3370999617,  0, 0.3713945616,   0.5011264475,   0.7816254623,   0, -0.1277062463,  -0.4254438999,    -0.8959289049, 0, -0.2881560924, -0.5815838982,  0.7607405838,   0,
            0.5849561111,   -0.662820239,   -0.4674352136,  0, 0.3307171178,  0.0391653737,   0.94291689,     0, 0.8712121778,   -0.4113374369, -0.2679381538,   0, 0.580981015,    0.7021915846,  0.4115677815,  0, 0.503756873,   0.6330056931,  -0.5878203852,  0, 0.4493712205,   0.601390195,    0.6606022552,   0, -0.6878403724,  0.09018890807,    -0.7202371714, 0, -0.5958956522, -0.6469350577,  0.475797649,    0,
            -0.5127052122,  0.1946921978,   -0.8361987284,  0, -0.9911507142, -0.05410276466, -0.1212153153,  0, -0.2149721042,  0.9720882117,  -0.09397607749,  0, -0.7518650936,  -0.5428057603, 0.3742469607,  0, 0.5237068895,  0.8516377189,  -0.02107817834, 0, 0.6333504779,   0.1926167129,   -0.7495104896,  0, -0.06788241606, 0.3998305789,     0.9140719259,  0, -0.5538628599, -0.4729896695,  -0.6852128902,  0,
            -0.7261455366,  -0.5911990757,  0.3509933228,   0, -0.9229274737, -0.1782808786,  0.3412049336,   0, -0.6968815002,  0.6511274338,  0.3006480328,    0, 0.9608044783,   -0.2098363234, -0.1811724921, 0, 0.06817146062, -0.9743405129, 0.2145069156,   0, -0.3577285196,  -0.6697087264,  -0.6507845481,  0, -0.1868621131,  0.7648617052,     -0.6164974636, 0, -0.6541697588, 0.3967914832,   0.6439087246,   0,
            0.6993340405,   -0.6164538506,  0.3618239211,   0, -0.1546665739, 0.6291283928,   0.7617583057,   0, -0.6841612949,  -0.2580482182, -0.6821542638,   0, 0.5383980957,   0.4258654885,  0.7271630328,  0, -0.5026987823, -0.7939832935, -0.3418836993,  0, 0.3202971715,   0.2834415347,   0.9039195862,   0, 0.8683227101,   -0.0003762656404, -0.4959995258, 0, 0.791120031,   -0.08511045745, 0.6057105799,   0,
            -0.04011016052, -0.4397248749,  0.8972364289,   0, 0.9145119872,  0.3579346169,   -0.1885487608,  0, -0.9612039066,  -0.2756484276, 0.01024666929,   0, 0.6510361721,   -0.2877799159, -0.7023778346, 0, -0.2041786351, 0.7365237271,  0.644859585,    0, -0.7718263711,  0.3790626912,   0.5104855816,   0, -0.3060082741,  -0.7692987727,    0.5608371729,  0, 0.454007341,   -0.5024843065,  0.7357899537,   0,
            0.4816795475,   0.6021208291,   -0.6367380315,  0, 0.6961980369,  -0.3222197429,  0.641469197,    0, -0.6532160499,  -0.6781148932, 0.3368515753,    0, 0.5089301236,   -0.6154662304, -0.6018234363, 0, -0.1635919754, -0.9133604627, -0.372840892,   0, 0.52408019,     -0.8437664109,  0.1157505864,   0, 0.5902587356,   0.4983817807,     -0.6349883666, 0, 0.5863227872,  0.494764745,    0.6414307729,   0,
            0.6779335087,   0.2341345225,   0.6968408593,   0, 0.7177054546,  -0.6858979348,  0.120178631,    0, -0.5328819713,  -0.5205125012, 0.6671608058,    0, -0.8654874251,  -0.0700727088, -0.4960053754, 0, -0.2861810166, 0.7952089234,  0.5345495242,   0, -0.04849529634, 0.9810836427,   -0.1874115585,  0, -0.6358521667,  0.6058348682,     0.4781800233,  0, 0.6254794696,  -0.2861619734,  0.7258696564,   0,
            -0.2585259868,  0.5061949264,   -0.8227581726,  0, 0.02136306781, 0.5064016808,   -0.8620330371,  0, 0.200111773,    0.8599263484,  0.4695550591,    0, 0.4743561372,   0.6014985084,  -0.6427953014, 0, 0.6622993731,  -0.5202474575, -0.5391679918,  0, 0.08084972818,  -0.6532720452,  0.7527940996,   0, -0.6893687501,  0.0592860349,     0.7219805347,  0, -0.1121887082, -0.9673185067,  0.2273952515,   0,
            0.7344116094,   0.5979668656,   -0.3210532909,  0, 0.5789393465,  -0.2488849713,  0.7764570201,   0, 0.6988182827,   0.3557169806,  -0.6205791146,   0, -0.8636845529,  -0.2748771249, -0.4224826141, 0, -0.4247027957, -0.4640880967, 0.777335046,    0, 0.5257722489,   -0.8427017621,  0.1158329937,   0, 0.9343830603,   0.316302472,      -0.1639543925, 0, -0.1016836419, -0.8057303073,  -0.5834887393,  0,
            -0.6529238969,  0.50602126,     -0.5635892736,  0, -0.2465286165, -0.9668205684,  -0.06694497494, 0, -0.9776897119,  -0.2099250524, -0.007368825344, 0, 0.7736893337,   0.5734244712,  0.2694238123,  0, -0.6095087895, 0.4995678998,  0.6155736747,   0, 0.5794535482,   0.7434546771,   0.3339292269,   0, -0.8226211154,  0.08142581855,    0.5627293636,  0, -0.510385483,  0.4703667658,   0.7199039967,   0,
            -0.5764971849,  -0.07231656274, -0.8138926898,  0, 0.7250628871,  0.3949971505,   -0.5641463116,  0, -0.1525424005,  0.4860840828,  -0.8604958341,   0, -0.5550976208,  -0.4957820792, 0.667882296,   0, -0.1883614327, 0.9145869398,  0.357841725,    0, 0.7625556724,   -0.5414408243,  -0.3540489801,  0, -0.5870231946,  -0.3226498013,    -0.7424963803, 0, 0.3051124198,  0.2262544068,   -0.9250488391,  0,
            0.6379576059,   0.577242424,    -0.5097070502,  0, -0.5966775796, 0.1454852398,   -0.7891830656,  0, -0.658330573,   0.6555487542,  -0.3699414651,   0, 0.7434892426,   0.2351084581,  0.6260573129,  0, 0.5562114096,  0.8264360377,  -0.0873632843,  0, -0.3028940016,  -0.8251527185,  0.4768419182,   0, 0.1129343818,   -0.985888439,     -0.1235710781, 0, 0.5937652891,  -0.5896813806,  0.5474656618,   0,
            0.6757964092,   -0.5835758614,  -0.4502648413,  0, 0.7242302609,  -0.1152719764,  0.6798550586,   0, -0.9511914166,  0.0753623979,  -0.2992580792,   0, 0.2539470961,   -0.1886339355, 0.9486454084,  0, 0.571433621,   -0.1679450851, -0.8032795685,  0, -0.06778234979, 0.3978269256,   0.9149531629,   0, 0.6074972649,   0.733060024,      -0.3058922593, 0, -0.5435478392, 0.1675822484,   0.8224791405,   0,
            -0.5876678086,  -0.3380045064,  -0.7351186982,  0, -0.7967562402, 0.04097822706,  -0.6029098428,  0, -0.1996350917,  0.8706294745,  0.4496111079,    0, -0.02787660336, -0.9106232682, -0.4122962022, 0, -0.7797625996, -0.6257634692, 0.01975775581,  0, -0.5211232846,  0.7401644346,   -0.4249554471,  0, 0.8575424857,   0.4053272873,     -0.3167501783, 0, 0.1045223322,  0.8390195772,   -0.5339674439,  0,
            0.3501822831,   0.9242524096,   -0.1520850155,  0, 0.1987849858,  0.07647613266,  0.9770547224,   0, 0.7845996363,   0.6066256811,  -0.1280964233,   0, 0.09006737436,  -0.9750989929, -0.2026569073, 0, -0.8274343547, -0.542299559,  0.1458203587,   0, -0.3485797732,  -0.415802277,   0.840000362,    0, -0.2471778936,  -0.7304819962,    -0.6366310879, 0, -0.3700154943, 0.8577948156,   0.3567584454,   0,
            0.5913394901,   -0.548311967,   -0.5913303597,  0, 0.1204873514,  -0.7626472379,  -0.6354935001,  0, 0.616959265,    0.03079647928, 0.7863922953,    0, 0.1258156836,   -0.6640829889, -0.7369967419, 0, -0.6477565124, -0.1740147258, -0.7417077429,  0, 0.6217889313,   -0.7804430448,  -0.06547655076, 0, 0.6589943422,   -0.6096987708,    0.4404473475,  0, -0.2689837504, -0.6732403169,  -0.6887635427,  0,
            -0.3849775103,  0.5676542638,   0.7277093879,   0, 0.5754444408,  0.8110471154,   -0.1051963504,  0, 0.9141593684,   0.3832947817,  0.131900567,     0, -0.107925319,   0.9245493968,  0.3654593525,  0, 0.377977089,   0.3043148782,  0.8743716458,   0, -0.2142885215,  -0.8259286236,  0.5214617324,   0, 0.5802544474,   0.4148098596,     -0.7008834116, 0, -0.1982660881, 0.8567161266,   -0.4761596756,  0,
            -0.03381553704, 0.3773180787,   -0.9254661404,  0, -0.6867922841, -0.6656597827,  0.2919133642,   0, 0.7731742607,   -0.2875793547, -0.5652430251,   0, -0.09655941928, 0.9193708367,  -0.3813575004, 0, 0.2715702457,  -0.9577909544, -0.09426605581, 0, 0.2451015704,   -0.6917998565,  -0.6792188003,  0, 0.977700782,    -0.1753855374,    0.1155036542,  0, -0.5224739938, 0.8521606816,   0.02903615945,  0,
            -0.7734880599,  -0.5261292347,  0.3534179531,   0, -0.7134492443, -0.269547243,   0.6467878011,   0, 0.1644037271,   0.5105846203,  -0.8439637196,   0, 0.6494635788,   0.05585611296, 0.7583384168,  0, -0.4711970882, 0.5017280509,  -0.7254255765,  0, -0.6335764307,  -0.2381686273,  -0.7361091029,  0, -0.9021533097,  -0.270947803,     -0.3357181763, 0, -0.3793711033, 0.872258117,    0.3086152025,   0,
            -0.6855598966,  -0.3250143309,  0.6514394162,   0, 0.2900942212,  -0.7799057743,  -0.5546100667,  0, -0.2098319339,  0.85037073,    0.4825351604,    0, -0.4592603758,  0.6598504336,  -0.5947077538, 0, 0.8715945488,  0.09616365406, -0.4807031248,  0, -0.6776666319,  0.7118504878,   -0.1844907016,  0, 0.7044377633,   0.312427597,      0.637304036,   0, -0.7052318886, -0.2401093292,  -0.6670798253,  0,
            0.081921007,    -0.7207336136,  -0.6883545647,  0, -0.6993680906, -0.5875763221,  -0.4069869034,  0, -0.1281454481,  0.6419895885,  0.7559286424,    0, -0.6337388239,  -0.6785471501, -0.3714146849, 0, 0.5565051903,  -0.2168887573, -0.8020356851,  0, -0.5791554484,  0.7244372011,   -0.3738578718,  0, 0.1175779076,   -0.7096451073,    0.6946792478,  0, -0.6134619607, 0.1323631078,   0.7785527795,   0,
            0.6984635305,   -0.02980516237, -0.715024719,   0, 0.8318082963,  -0.3930171956,  0.3919597455,   0, 0.1469576422,   0.05541651717, -0.9875892167,   0, 0.708868575,    -0.2690503865, 0.6520101478,  0, 0.2726053183,  0.67369766,    -0.68688995,    0, -0.6591295371,  0.3035458599,   -0.6880466294,  0, 0.4815131379,   -0.7528270071,    0.4487723203,  0, 0.9430009463,  0.1675647412,   -0.2875261255,  0,
            0.434802957,    0.7695304522,   -0.4677277752,  0, 0.3931996188,  0.594473625,    0.7014236729,   0, 0.7254336655,   -0.603925654,  0.3301814672,    0, 0.7590235227,   -0.6506083235, 0.02433313207, 0, -0.8552768592, -0.3430042733, 0.3883935666,   0, -0.6139746835,  0.6981725247,   0.3682257648,   0, -0.7465905486,  -0.5752009504,    0.3342849376,  0, 0.5730065677,  0.810555537,    -0.1210916791,  0,
            -0.9225877367,  -0.3475211012,  -0.167514036,   0, -0.7105816789, -0.4719692027,  -0.5218416899,  0, -0.08564609717, 0.3583001386,  0.929669703,     0, -0.8279697606,  -0.2043157126, 0.5222271202,  0, 0.427944023,   0.278165994,   0.8599346446,   0, 0.5399079671,   -0.7857120652,  -0.3019204161,  0, 0.5678404253,   -0.5495413974,    -0.6128307303, 0, -0.9896071041, 0.1365639107,   -0.04503418428, 0,
            -0.6154342638,  -0.6440875597,  0.4543037336,   0, 0.1074204368,  -0.7946340692,  0.5975094525,   0, -0.3595449969,  -0.8885529948, 0.28495784,      0, -0.2180405296,  0.1529888965,  0.9638738118,  0, -0.7277432317, -0.6164050508, -0.3007234646,  0, 0.7249729114,   -0.00669719484, 0.6887448187,   0, -0.5553659455,  -0.5336586252,    0.6377908264,  0, 0.5137558015,  0.7976208196,   -0.3160000073,  0,
            -0.3794024848,  0.9245608561,   -0.03522751494, 0, 0.8229248658,  0.2745365933,   -0.4974176556,  0, -0.5404114394,  0.6091141441,  0.5804613989,    0, 0.8036581901,   -0.2703029469, 0.5301601931,  0, 0.6044318879,  0.6832968393,  0.4095943388,   0, 0.06389988817,  0.9658208605,   -0.2512108074,  0, 0.1087113286,   0.7402471173,     -0.6634877936, 0, -0.713427712,  -0.6926784018,  0.1059128479,   0,
            0.6458897819,   -0.5724548511,  -0.5050958653,  0, -0.6553931414, 0.7381471625,   0.159995615,    0, 0.3910961323,   0.9188871375,  -0.05186755998,  0, -0.4879022471,  -0.5904376907, 0.6429111375,  0, 0.6014790094,  0.7707441366,  -0.2101820095,  0, -0.5677173047,  0.7511360995,   0.3368851762,   0, 0.7858573506,   0.226674665,      0.5753666838,  0, -0.4520345543, -0.604222686,   -0.6561857263,  0,
            0.002272116345, 0.4132844051,   -0.9105991643,  0, -0.5815751419, -0.5162925989,  0.6286591339,   0, -0.03703704785, 0.8273785755,  0.5604221175,    0, -0.5119692504,  0.7953543429,  -0.3244980058, 0, -0.2682417366, -0.9572290247, -0.1084387619,  0, -0.2322482736,  -0.9679131102,  -0.09594243324, 0, 0.3554328906,   -0.8881505545,    0.2913006227,  0, 0.7346520519,  -0.4371373164,  0.5188422971,   0,
            0.9985120116,   0.04659011161,  -0.02833944577, 0, -0.3727687496, -0.9082481361,  0.1900757285,   0, 0.91737377,     -0.3483642108, 0.1925298489,    0, 0.2714911074,   0.4147529736,  -0.8684886582, 0, 0.5131763485,  -0.7116334161, 0.4798207128,   0, -0.8737353606,  0.18886992,     -0.4482350644,  0, 0.8460043821,   -0.3725217914,    0.3814499973,  0, 0.8978727456,  -0.1780209141,  -0.4026575304,  0,
            0.2178065647,   -0.9698322841,  -0.1094789531,  0, -0.1518031304, -0.7788918132,  -0.6085091231,  0, -0.2600384876,  -0.4755398075, -0.8403819825,   0, 0.572313509,    -0.7474340931, -0.3373418503, 0, -0.7174141009, 0.1699017182,  -0.6756111411,  0, -0.684180784,   0.02145707593,  -0.7289967412,  0, -0.2007447902,  0.06555605789,    -0.9774476623, 0, -0.1148803697, -0.8044887315,  0.5827524187,   0,
            -0.7870349638,  0.03447489231,  0.6159443543,   0, -0.2015596421, 0.6859872284,   0.6991389226,   0, -0.08581082512, -0.10920836,   -0.9903080513,   0, 0.5532693395,   0.7325250401,  -0.396610771,  0, -0.1842489331, -0.9777375055, -0.1004076743,  0, 0.0775473789,   -0.9111505856,  0.4047110257,   0, 0.1399838409,   0.7601631212,     -0.6344734459, 0, 0.4484419361,  -0.845289248,   0.2904925424,   0,
        };
    };
}

test "reference all" {
    std.testing.refAllDecls(Noise(f32));
}

test "range of all 2D noise/fractal combinations" {
    const size = 768;
    var noise = Noise(f32){};

    inline for (@typeInfo(FractalType).Enum.fields) |fractal| {
        noise.fractal_type = comptime std.meta.stringToEnum(FractalType, fractal.name).?;

        inline for (@typeInfo(NoiseType).Enum.fields) |noise_type| {
            noise.noise_type = comptime std.meta.stringToEnum(NoiseType, noise_type.name).?;
            for (0..size * size) |i| {
                const value = noise.genNoise2D(@floatFromInt(i % size), @floatFromInt(i / size));
                try expect(value >= -1.0 and value <= 1.0);
            }
        }
    }
}

test "range of all 3D noise/fractal combinations" {
    const size = 64;
    var noise = Noise(f32){};

    inline for (@typeInfo(FractalType).Enum.fields) |fractal| {
        noise.fractal_type = comptime std.meta.stringToEnum(FractalType, fractal.name).?;

        inline for (@typeInfo(NoiseType).Enum.fields) |noise_type| {
            noise.noise_type = comptime std.meta.stringToEnum(NoiseType, noise_type.name).?;
            for (0..size) |z| for (0..size) |y| for (0..size) |x| {
                const value = noise.genNoise3D(@floatFromInt(x), @floatFromInt(y), @floatFromInt(z));
                try expect(value >= -1.0 and value <= 1.0);
            };
        }
    }
}

test "range of all 2D cellular return/distance combinations (f64)" {
    const size = 768;
    var noise = Noise(f64){
        .noise_type = .cellular,
    };

    inline for (@typeInfo(CellularDistanceFunc).Enum.fields) |dist| {
        noise.cellular_distance = comptime std.meta.stringToEnum(CellularDistanceFunc, dist.name).?;
        inline for (@typeInfo(CellularReturnType).Enum.fields) |ret| {
            noise.cellular_return = std.meta.stringToEnum(CellularReturnType, ret.name).?;
            for (0..size * size) |i| {
                const value = noise.genNoise2D(@floatFromInt(i % size), @floatFromInt(i / size));
                try expect(value >= -1.0 and value <= 1.0);
            }
        }
    }
}

test "range of all 3D cellular return/distance combinations" {
    const size = 96;
    var noise = Noise(f32){
        .noise_type = .cellular,
    };

    inline for (@typeInfo(CellularDistanceFunc).Enum.fields) |dist| {
        noise.cellular_distance = comptime std.meta.stringToEnum(CellularDistanceFunc, dist.name).?;
        inline for (@typeInfo(CellularReturnType).Enum.fields) |ret| {
            noise.cellular_return = std.meta.stringToEnum(CellularReturnType, ret.name).?;
            for (0..size) |z| for (0..size) |y| for (0..size) |x| {
                const value = noise.genNoise3D(@floatFromInt(x), @floatFromInt(y), @floatFromInt(z));
                try expect(value >= -1.0 and value <= 1.0);
            };
        }
    }
}

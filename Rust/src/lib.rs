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
// https://crates.io/crates/fastnoise-lite
//
// Ported to Rust by Keavon Chambers:
// Discord: Keavon (preferred) | Email: see <https://keavon.com> for the address | GitHub: Keavon (https://github.com/Keavon)

#![doc = include_str!("../README.md")]

#![cfg_attr(not(feature = "std"), no_std)]
#![allow(clippy::excessive_precision)]

// ===================================================================================
// "f64" feature flag:
// For choosing the desired floating point precision of input position X/Y coordinates
// ===================================================================================

// Switch between using floats or doubles for input X/Y coordinate positions
#[cfg(not(feature = "f64"))]
type Float = f32;
#[cfg(feature = "f64")]
type Float = f64;

// ===========================================================================================================================================================
// "std" and "libm" feature flags:
// Ensures access to `sqrt()`, `trunc()`, and `abs()` floating point functions from either the Rust standard library, or the `libm` crate for `no_std` support
// ===========================================================================================================================================================

// Use the `num-traits` crate's `Float` trait if the user has enabled the `libm` feature flag
#[cfg(feature = "libm")]
use num_traits::float::Float as FloatOps;

// Use the standard library's `f32` type for floating point math operators if we're in an `std` (not `no_std`) context and the user hasn't enabled the `libm` feature flag
#[cfg(all(feature = "std", not(feature = "libm")))]
use f32 as FloatOps;

// Ensures a user-facing compile error if this crate is misused by having neither the "std" or "libm" feature flags
#[cfg(all(not(feature = "std"), not(feature = "libm")))]
compile_error!("`fastnoise-lite` crate: either the "std" or "libm" feature must be enabled");
#[cfg(all(not(feature = "std"), not(feature = "libm")))]
use Float as FloatOps;
#[cfg(all(not(feature = "std"), not(feature = "libm")))]
impl DummyFloatExt for Float {}
#[cfg(all(not(feature = "std"), not(feature = "libm")))]
trait DummyFloatExt: Sized {
    // Dummy trait to allow compilation without std or libm

    fn sqrt(self) -> Self {
        unimplemented!()
    }
    fn trunc(self) -> Self {
        unimplemented!()
    }
    fn abs(self) -> Self {
        unimplemented!()
    }
}

// ========================================
// Option enums for FastNoise Lite settings
// ========================================

#[derive(Copy, Clone, Debug, PartialEq)]
pub enum NoiseType {
    OpenSimplex2,
    OpenSimplex2S,
    Cellular,
    Perlin,
    ValueCubic,
    Value,
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub enum RotationType3D {
    None,
    ImproveXYPlanes,
    ImproveXZPlanes,
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub enum FractalType {
    None,
    FBm,
    Ridged,
    PingPong,
    DomainWarpProgressive,
    DomainWarpIndependent,
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub enum CellularDistanceFunction {
    Euclidean,
    EuclideanSq,
    Manhattan,
    Hybrid,
}

#[derive(Copy, Clone, Debug, PartialEq, PartialOrd)]
pub enum CellularReturnType {
    CellValue = 0,
    Distance = 1,
    Distance2 = 2,
    Distance2Add = 3,
    Distance2Sub = 4,
    Distance2Mul = 5,
    Distance2Div = 6,
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub enum DomainWarpType {
    OpenSimplex2,
    OpenSimplex2Reduced,
    BasicGrid,
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub enum TransformType3D {
    None,
    ImproveXYPlanes,
    ImproveXZPlanes,
    DefaultOpenSimplex2,
}

// ========================================
// FastNoise Lite settings/generator object
// ========================================

/// The object you construct, configure, and then use to sample the noise.
///
/// 1. Construct this with [`FastNoiseLite::new`] or [`FastNoiseLite::with_seed`].
/// 2. Configure it from its defaults with the various `set_*` functions.
/// 3. Optionally, use [`FastNoiseLite::domain_warp_2d`] or [`FastNoiseLite::domain_warp_3d`] to translate the input coordinates to their warped positions.
/// 4. Use [`FastNoiseLite::get_noise_2d`] or [`FastNoiseLite::get_noise_3d`] to sample the noise at the (ordinary or warped) input coordinates.
///
/// # Example
///
/// ```rs
/// use fastnoise_lite::*;
/// 
/// // Create and configure the FastNoise object
/// let mut noise = FastNoiseLite::new();
/// noise.set_noise_type(Some(NoiseType::OpenSimplex2));
/// 
/// const WIDTH: usize = 128;
/// const HEIGHT: usize = 128;
/// let mut noise_data = [[0.; HEIGHT]; WIDTH];
/// 
/// // Sample noise pixels
/// for x in 0..WIDTH {
///     for y in 0..HEIGHT {
///         // Domain warp can optionally be employed to transform the coordinates before sampling:
///         // let (x, y) = noise.domain_warp_2d(x as f32, y as f32);
///         
///         let negative_1_to_1 = noise.get_noise_2d(x as f32, y as f32);
///         // You may want to remap the -1..1 range data to the 0..1 range:
///         noise_data[x][y] = (neg_1_to_1 + 1.) / 2.;
///         
///         // (Uses of `as f32` above should become `as f64` if you're using FNL with the "f64" feature flag)
///     }
/// }
///
/// // Do something with this data...
/// ```
pub struct FastNoiseLite {
    pub seed: i32,
    pub frequency: f32,
    pub noise_type: NoiseType,
    pub rotation_type_3d: RotationType3D,
    transform_type_3d: TransformType3D,

    pub fractal_type: FractalType,
    pub octaves: i32,
    pub lacunarity: f32,
    pub gain: f32,
    pub weighted_strength: f32,
    pub ping_pong_strength: f32,

    fractal_bounding: f32,

    pub cellular_distance_function: CellularDistanceFunction,
    pub cellular_return_type: CellularReturnType,
    pub cellular_jitter_modifier: f32,

    pub domain_warp_type: DomainWarpType,
    warp_transform_type_3d: TransformType3D,
    pub domain_warp_amp: f32,
}

impl Default for FastNoiseLite {
    fn default() -> Self {
        Self {
            seed: 1337,
            frequency: 0.01,
            noise_type: NoiseType::OpenSimplex2,
            rotation_type_3d: RotationType3D::None,
            /* private */ transform_type_3d: TransformType3D::DefaultOpenSimplex2,

            fractal_type: FractalType::None,
            octaves: 3,
            lacunarity: 2.,
            gain: 0.5,
            weighted_strength: 0.,
            ping_pong_strength: 2.,

            /* private */ fractal_bounding: 1. / 1.75,

            cellular_distance_function: CellularDistanceFunction::EuclideanSq,
            cellular_return_type: CellularReturnType::Distance,
            cellular_jitter_modifier: 1.,

            domain_warp_type: DomainWarpType::OpenSimplex2,
            /* private */ warp_transform_type_3d: TransformType3D::DefaultOpenSimplex2,
            domain_warp_amp: 1.,
        }
    }
}

impl FastNoiseLite {
    // =====================
    // Constructor functions
    // =====================

    /// # Constructor
    ///
    /// Create new FastNoise object with the default seed of `1337`.
    pub fn new() -> Self {
        Self::default()
    }

    /// Create new FastNoise object with a specific seed.
    pub fn with_seed(seed: i32) -> Self {
        let mut fnl = Self::default();
        fnl.set_seed(Some(seed));
        fnl
    }

    // ============================
    // Setter convenience functions
    // ============================

    /// Sets seed used for all noise types.
    ///
    /// If set to [`None`], it is reset to its default: `1337`.
    pub fn set_seed(&mut self, seed: Option<i32>) {
        self.seed = seed.unwrap_or(Self::default().seed);
    }

    /// Sets frequency used for all noise types.
    ///
    /// If set to [`None`], it is reset to its default: `0.01`.
    pub fn set_frequency(&mut self, frequency: Option<f32>) {
        self.frequency = frequency.unwrap_or(Self::default().frequency);
    }

    /// Sets noise algorithm used for [`get_noise_2d`](Self::get_noise_2d)/[`get_noise_3d`](Self::get_noise_3d).
    ///
    /// If set to [`None`], it is reset to its default: [`NoiseType::OpenSimplex2`].
    pub fn set_noise_type(&mut self, noise_type: Option<NoiseType>) {
        self.noise_type = noise_type.unwrap_or(Self::default().noise_type);
        self.update_transform_type_3d();
    }

    /// Sets domain rotation type for 3D Noise and 3D DomainWarp.
    /// Can aid in reducing directional artifacts when sampling a 2D plane in 3D.
    ///
    /// If set to [`None`], it is reset to its default: [`RotationType3D::None`].
    pub fn set_rotation_type_3d(&mut self, rotation_type_3d: Option<RotationType3D>) {
        self.rotation_type_3d = rotation_type_3d.unwrap_or(Self::default().rotation_type_3d);
        self.update_transform_type_3d();
        self.update_warp_transform_type_3d();
    }

    /// Sets method for combining octaves in all fractal noise types.
    ///
    /// If set to [`None`], it is reset to its default: [`FractalType::None`].
    ///
    /// Note: [`FractalType::DomainWarpProgressive`]/[`FractalType::DomainWarpIndependent`] only affects [`domain_warp_2d`](Self::domain_warp_2d).
    pub fn set_fractal_type(&mut self, fractal_type: Option<FractalType>) {
        self.fractal_type = fractal_type.unwrap_or(Self::default().fractal_type);
    }

    /// Sets octave count for all fractal noise types.
    ///
    /// If set to [`None`], it is reset to its default: `3`.
    pub fn set_fractal_octaves(&mut self, octaves: Option<i32>) {
        self.octaves = octaves.unwrap_or(Self::default().octaves);
        self.calculate_fractal_bounding();
    }

    /// Sets octave lacunarity for all fractal noise types.
    ///
    /// If set to [`None`], it is reset to its default: `2.0`.
    pub fn set_fractal_lacunarity(&mut self, lacunarity: Option<f32>) {
        self.lacunarity = lacunarity.unwrap_or(Self::default().lacunarity);
    }

    /// Sets octave gain for all fractal noise types.
    ///
    /// If set to [`None`], it is reset to its default: `0.5`.
    pub fn set_fractal_gain(&mut self, gain: Option<f32>) {
        self.gain = gain.unwrap_or(Self::default().gain);
        self.calculate_fractal_bounding();
    }

    /// Sets octave weighting for all none DomainWarp fractal types.
    ///
    /// If set to [`None`], it is reset to its default: `0.0`.
    ///
    /// Note: Keep between 0..1 to maintain -1..1 output bounding.
    pub fn set_fractal_weighted_strength(&mut self, weighted_strength: Option<f32>) {
        self.weighted_strength = weighted_strength.unwrap_or(Self::default().weighted_strength);
    }

    /// Sets strength of the fractal ping pong effect.
    ///
    /// If set to [`None`], it is reset to its default: `2.0`.
    pub fn set_fractal_ping_pong_strength(&mut self, ping_pong_strength: Option<f32>) {
        self.ping_pong_strength = ping_pong_strength.unwrap_or(Self::default().ping_pong_strength);
    }

    /// Sets distance function used in cellular noise calculations.
    ///
    /// If set to [`None`], it is reset to its default: [`CellularDistanceFunction::EuclideanSq`].
    pub fn set_cellular_distance_function(
        &mut self,
        cellular_distance_function: Option<CellularDistanceFunction>,
    ) {
        self.cellular_distance_function =
            cellular_distance_function.unwrap_or(Self::default().cellular_distance_function);
    }

    /// Sets return type from cellular noise calculations.
    ///
    /// If set to [`None`], it is reset to its default: [`CellularReturnType::Distance`].
    pub fn set_cellular_return_type(&mut self, cellular_return_type: Option<CellularReturnType>) {
        self.cellular_return_type =
            cellular_return_type.unwrap_or(Self::default().cellular_return_type);
    }

    /// Sets the maximum distance a cellular point can move from its grid position.
    ///
    /// If set to [`None`], it is reset to its default: `1.0`.
    ///
    /// Note: Setting this higher than 1 will cause artifacts.
    pub fn set_cellular_jitter(&mut self, cellular_jitter: Option<f32>) {
        self.cellular_jitter_modifier =
            cellular_jitter.unwrap_or(Self::default().cellular_jitter_modifier);
    }

    /// Sets the warp algorithm when using [`domain_warp_2d`](Self::domain_warp_2d).
    ///
    /// If set to [`None`], it is reset to its default: [`DomainWarpType::OpenSimplex2`].
    pub fn set_domain_warp_type(&mut self, domain_warp_type: Option<DomainWarpType>) {
        self.domain_warp_type = domain_warp_type.unwrap_or(Self::default().domain_warp_type);
        self.update_warp_transform_type_3d();
    }

    /// Sets the maximum warp distance from original position when using [`domain_warp_2d`](Self::domain_warp_2d).
    ///
    /// If set to [`None`], it is reset to its default: `1.0`.
    pub fn set_domain_warp_amp(&mut self, domain_warp_amp: Option<f32>) {
        self.domain_warp_amp = domain_warp_amp.unwrap_or(Self::default().domain_warp_amp);
    }

    // =========================
    // Noise generator functions
    // =========================

    /// 2D noise at given position using current settings.
    ///
    /// Noise output bounded between -1..1.
    ///
    /// Example usage:
    /// ```rs
    /// let noise = get_noise_2d(x, y); // Value in the -1..1 range
    /// let noise = (noise + 1.) / 2.; // Consider remapping it to the 0..1 range
    /// ```
    pub fn get_noise_2d(&self, x: Float, y: Float) -> f32 {
        let (x, y) = self.transform_noise_coordinate_2d(x, y);

        match self.fractal_type {
            FractalType::FBm => self.gen_fractal_fbm_2d(x, y),
            FractalType::Ridged => self.gen_fractal_ridged_2d(x, y),
            FractalType::PingPong => self.gen_fractal_ping_pong_2d(x, y),
            _ => self.gen_noise_single_2d(self.seed, x, y),
        }
    }

    /// 3D noise at given position using current settings.
    ///
    /// Noise output is bounded between -1..1.
    ///
    /// Example usage:
    /// ```rs
    /// let noise = get_noise_3d(x, y, z); // Value in the -1..1 range
    /// let noise = (noise + 1.) / 2.; // Consider remapping it to the 0..1 range
    /// ```
    pub fn get_noise_3d(&self, x: Float, y: Float, z: Float) -> f32 {
        let (x, y, z) = self.transform_noise_coordinate_3d(x, y, z);

        match self.fractal_type {
            FractalType::FBm => self.gen_fractal_fbm_3d(x, y, z),
            FractalType::Ridged => self.gen_fractal_ridged_3d(x, y, z),
            FractalType::PingPong => self.gen_fractal_ping_pong_3d(x, y, z),
            _ => self.gen_noise_single_3d(self.seed, x, y, z),
        }
    }

    // ===============================================
    // Domain warp coordinate transformation functions
    // ===============================================

    /// 2D warps the input position using current domain warp settings.
    ///
    /// Example usage:
    /// ```rs
    /// let (x, y) = domain_warp_2d(x, y);
    /// let noise = get_noise_2d(x, y); // Value in the -1..1 range
    /// let noise = (noise + 1.) / 2.; // Consider remapping it to the 0..1 range
    /// ```
    pub fn domain_warp_2d(&self, x: Float, y: Float) -> (Float, Float) {
        match self.fractal_type {
            FractalType::DomainWarpProgressive => self.domain_warp_fractal_progressive_2d(x, y),
            FractalType::DomainWarpIndependent => self.domain_warp_fractal_independent_2d(x, y),
            _ => self.domain_warp_single_2d(x, y),
        }
    }

    /// 3D warps the input position using current domain warp settings.
    ///
    /// Example usage:
    /// ```rs
    /// let (x, y, z) = domain_warp_3d(x, y, z);
    /// let noise = get_noise_3d(x, y, z); // Value in the -1..1 range
    /// let noise = (noise + 1.) / 2.; // Consider remapping it to the 0..1 range
    /// ```
    pub fn domain_warp_3d(&self, x: Float, y: Float, z: Float) -> (Float, Float, Float) {
        match self.fractal_type {
            FractalType::DomainWarpProgressive => self.domain_warp_fractal_progressive_3d(x, y, z),
            FractalType::DomainWarpIndependent => self.domain_warp_fractal_independent_3d(x, y, z),
            _ => self.domain_warp_single_3d(x, y, z),
        }
    }

    // =================
    // Lookup table data
    // =================
    
    #[rustfmt::skip]
    const GRADIENTS_2D: [f32; 256] = [
         0.130526192220052,  0.99144486137381,   0.38268343236509,   0.923879532511287,  0.608761429008721,  0.793353340291235,  0.793353340291235,  0.608761429008721,
         0.923879532511287,  0.38268343236509,   0.99144486137381,   0.130526192220051,  0.99144486137381,  -0.130526192220051,  0.923879532511287, -0.38268343236509,
         0.793353340291235, -0.60876142900872,   0.608761429008721, -0.793353340291235,  0.38268343236509,  -0.923879532511287,  0.130526192220052, -0.99144486137381,
        -0.130526192220052, -0.99144486137381,  -0.38268343236509,  -0.923879532511287, -0.608761429008721, -0.793353340291235, -0.793353340291235, -0.608761429008721,
        -0.923879532511287, -0.38268343236509,  -0.99144486137381,  -0.130526192220052, -0.99144486137381,   0.130526192220051, -0.923879532511287,  0.38268343236509,
        -0.793353340291235,  0.608761429008721, -0.608761429008721,  0.793353340291235, -0.38268343236509,   0.923879532511287, -0.130526192220052,  0.99144486137381,
         0.130526192220052,  0.99144486137381,   0.38268343236509,   0.923879532511287,  0.608761429008721,  0.793353340291235,  0.793353340291235,  0.608761429008721,
         0.923879532511287,  0.38268343236509,   0.99144486137381,   0.130526192220051,  0.99144486137381,  -0.130526192220051,  0.923879532511287, -0.38268343236509,
         0.793353340291235, -0.60876142900872,   0.608761429008721, -0.793353340291235,  0.38268343236509,  -0.923879532511287,  0.130526192220052, -0.99144486137381,
        -0.130526192220052, -0.99144486137381,  -0.38268343236509,  -0.923879532511287, -0.608761429008721, -0.793353340291235, -0.793353340291235, -0.608761429008721,
        -0.923879532511287, -0.38268343236509,  -0.99144486137381,  -0.130526192220052, -0.99144486137381,   0.130526192220051, -0.923879532511287,  0.38268343236509,
        -0.793353340291235,  0.608761429008721, -0.608761429008721,  0.793353340291235, -0.38268343236509,   0.923879532511287, -0.130526192220052,  0.99144486137381,
         0.130526192220052,  0.99144486137381,   0.38268343236509,   0.923879532511287,  0.608761429008721,  0.793353340291235,  0.793353340291235,  0.608761429008721,
         0.923879532511287,  0.38268343236509,   0.99144486137381,   0.130526192220051,  0.99144486137381,  -0.130526192220051,  0.923879532511287, -0.38268343236509,
         0.793353340291235, -0.60876142900872,   0.608761429008721, -0.793353340291235,  0.38268343236509,  -0.923879532511287,  0.130526192220052, -0.99144486137381,
        -0.130526192220052, -0.99144486137381,  -0.38268343236509,  -0.923879532511287, -0.608761429008721, -0.793353340291235, -0.793353340291235, -0.608761429008721,
        -0.923879532511287, -0.38268343236509,  -0.99144486137381,  -0.130526192220052, -0.99144486137381,   0.130526192220051, -0.923879532511287,  0.38268343236509,
        -0.793353340291235,  0.608761429008721, -0.608761429008721,  0.793353340291235, -0.38268343236509,   0.923879532511287, -0.130526192220052,  0.99144486137381,
         0.130526192220052,  0.99144486137381,   0.38268343236509,   0.923879532511287,  0.608761429008721,  0.793353340291235,  0.793353340291235,  0.608761429008721,
         0.923879532511287,  0.38268343236509,   0.99144486137381,   0.130526192220051,  0.99144486137381,  -0.130526192220051,  0.923879532511287, -0.38268343236509,
         0.793353340291235, -0.60876142900872,   0.608761429008721, -0.793353340291235,  0.38268343236509,  -0.923879532511287,  0.130526192220052, -0.99144486137381,
        -0.130526192220052, -0.99144486137381,  -0.38268343236509,  -0.923879532511287, -0.608761429008721, -0.793353340291235, -0.793353340291235, -0.608761429008721,
        -0.923879532511287, -0.38268343236509,  -0.99144486137381,  -0.130526192220052, -0.99144486137381,   0.130526192220051, -0.923879532511287,  0.38268343236509,
        -0.793353340291235,  0.608761429008721, -0.608761429008721,  0.793353340291235, -0.38268343236509,   0.923879532511287, -0.130526192220052,  0.99144486137381,
         0.130526192220052,  0.99144486137381,   0.38268343236509,   0.923879532511287,  0.608761429008721,  0.793353340291235,  0.793353340291235,  0.608761429008721,
         0.923879532511287,  0.38268343236509,   0.99144486137381,   0.130526192220051,  0.99144486137381,  -0.130526192220051,  0.923879532511287, -0.38268343236509,
         0.793353340291235, -0.60876142900872,   0.608761429008721, -0.793353340291235,  0.38268343236509,  -0.923879532511287,  0.130526192220052, -0.99144486137381,
        -0.130526192220052, -0.99144486137381,  -0.38268343236509,  -0.923879532511287, -0.608761429008721, -0.793353340291235, -0.793353340291235, -0.608761429008721,
        -0.923879532511287, -0.38268343236509,  -0.99144486137381,  -0.130526192220052, -0.99144486137381,   0.130526192220051, -0.923879532511287,  0.38268343236509,
        -0.793353340291235,  0.608761429008721, -0.608761429008721,  0.793353340291235, -0.38268343236509,   0.923879532511287, -0.130526192220052,  0.99144486137381,
         0.38268343236509,   0.923879532511287,  0.923879532511287,  0.38268343236509,   0.923879532511287, -0.38268343236509,   0.38268343236509,  -0.923879532511287,
        -0.38268343236509,  -0.923879532511287, -0.923879532511287, -0.38268343236509,  -0.923879532511287,  0.38268343236509,  -0.38268343236509,   0.923879532511287,
    ];

    #[rustfmt::skip]
    const RAND_VECS_2D: [f32; 512] = [
        -0.2700222198, -0.9628540911, 0.3863092627, -0.9223693152, 0.04444859006, -0.999011673, -0.5992523158, -0.8005602176, -0.7819280288, 0.6233687174, 0.9464672271, 0.3227999196, -0.6514146797, -0.7587218957, 0.9378472289, 0.347048376,
        -0.8497875957, -0.5271252623, -0.879042592, 0.4767432447, -0.892300288, -0.4514423508, -0.379844434, -0.9250503802, -0.9951650832, 0.0982163789, 0.7724397808, -0.6350880136, 0.7573283322, -0.6530343002, -0.9928004525, -0.119780055,
        -0.0532665713, 0.9985803285, 0.9754253726, -0.2203300762, -0.7665018163, 0.6422421394, 0.991636706, 0.1290606184, -0.994696838, 0.1028503788, -0.5379205513, -0.84299554, 0.5022815471, -0.8647041387, 0.4559821461, -0.8899889226,
        -0.8659131224, -0.5001944266, 0.0879458407, -0.9961252577, -0.5051684983, 0.8630207346, 0.7753185226, -0.6315704146, -0.6921944612, 0.7217110418, -0.5191659449, -0.8546734591, 0.8978622882, -0.4402764035, -0.1706774107, 0.9853269617,
        -0.9353430106, -0.3537420705, -0.9992404798, 0.03896746794, -0.2882064021, -0.9575683108, -0.9663811329, 0.2571137995, -0.8759714238, -0.4823630009, -0.8303123018, -0.5572983775, 0.05110133755, -0.9986934731, -0.8558373281, -0.5172450752,
         0.09887025282, 0.9951003332, 0.9189016087, 0.3944867976, -0.2439375892, -0.9697909324, -0.8121409387, -0.5834613061, -0.9910431363, 0.1335421355, 0.8492423985, -0.5280031709, -0.9717838994, -0.2358729591, 0.9949457207, 0.1004142068,
         0.6241065508, -0.7813392434, 0.662910307, 0.7486988212, -0.7197418176, 0.6942418282, -0.8143370775, -0.5803922158, 0.104521054, -0.9945226741, -0.1065926113, -0.9943027784, 0.445799684, -0.8951327509, 0.105547406, 0.9944142724,
        -0.992790267, 0.1198644477, -0.8334366408, 0.552615025, 0.9115561563, -0.4111755999, 0.8285544909, -0.5599084351, 0.7217097654, -0.6921957921, 0.4940492677, -0.8694339084, -0.3652321272, -0.9309164803, -0.9696606758, 0.2444548501,
         0.08925509731, -0.996008799, 0.5354071276, -0.8445941083, -0.1053576186, 0.9944343981, -0.9890284586, 0.1477251101, 0.004856104961, 0.9999882091, 0.9885598478, 0.1508291331, 0.9286129562, -0.3710498316, -0.5832393863, -0.8123003252,
         0.3015207509, 0.9534596146, -0.9575110528, 0.2883965738, 0.9715802154, -0.2367105511, 0.229981792, 0.9731949318, 0.955763816, -0.2941352207, 0.740956116, 0.6715534485, -0.9971513787, -0.07542630764, 0.6905710663, -0.7232645452,
        -0.290713703, -0.9568100872, 0.5912777791, -0.8064679708, -0.9454592212, -0.325740481, 0.6664455681, 0.74555369, 0.6236134912, 0.7817328275, 0.9126993851, -0.4086316587, -0.8191762011, 0.5735419353, -0.8812745759, -0.4726046147,
         0.9953313627, 0.09651672651, 0.9855650846, -0.1692969699, -0.8495980887, 0.5274306472, 0.6174853946, -0.7865823463, 0.8508156371, 0.52546432, 0.9985032451, -0.05469249926, 0.1971371563, -0.9803759185, 0.6607855748, -0.7505747292,
        -0.03097494063, 0.9995201614, -0.6731660801, 0.739491331, -0.7195018362, -0.6944905383, 0.9727511689, 0.2318515979, 0.9997059088, -0.0242506907, 0.4421787429, -0.8969269532, 0.9981350961, -0.061043673, -0.9173660799, -0.3980445648,
        -0.8150056635, -0.5794529907, -0.8789331304, 0.4769450202, 0.0158605829, 0.999874213, -0.8095464474, 0.5870558317, -0.9165898907, -0.3998286786, -0.8023542565, 0.5968480938, -0.5176737917, 0.8555780767, -0.8154407307, -0.5788405779,
         0.4022010347, -0.9155513791, -0.9052556868, -0.4248672045, 0.7317445619, 0.6815789728, -0.5647632201, -0.8252529947, -0.8403276335, -0.5420788397, -0.9314281527, 0.363925262, 0.5238198472, 0.8518290719, 0.7432803869, -0.6689800195,
        -0.985371561, -0.1704197369, 0.4601468731, 0.88784281, 0.825855404, 0.5638819483, 0.6182366099, 0.7859920446, 0.8331502863, -0.553046653, 0.1500307506, 0.9886813308, -0.662330369, -0.7492119075, -0.668598664, 0.743623444,
         0.7025606278, 0.7116238924, -0.5419389763, -0.8404178401, -0.3388616456, 0.9408362159, 0.8331530315, 0.5530425174, -0.2989720662, -0.9542618632, 0.2638522993, 0.9645630949, 0.124108739, -0.9922686234, -0.7282649308, -0.6852956957,
         0.6962500149, 0.7177993569, -0.9183535368, 0.3957610156, -0.6326102274, -0.7744703352, -0.9331891859, -0.359385508, -0.1153779357, -0.9933216659, 0.9514974788, -0.3076565421, -0.08987977445, -0.9959526224, 0.6678496916, 0.7442961705,
         0.7952400393, -0.6062947138, -0.6462007402, -0.7631674805, -0.2733598753, 0.9619118351, 0.9669590226, -0.254931851, -0.9792894595, 0.2024651934, -0.5369502995, -0.8436138784, -0.270036471, -0.9628500944, -0.6400277131, 0.7683518247,
        -0.7854537493, -0.6189203566, 0.06005905383, -0.9981948257, -0.02455770378, 0.9996984141, -0.65983623, 0.751409442, -0.6253894466, -0.7803127835, -0.6210408851, -0.7837781695, 0.8348888491, 0.5504185768, -0.1592275245, 0.9872419133,
         0.8367622488, 0.5475663786, -0.8675753916, -0.4973056806, -0.2022662628, -0.9793305667, 0.9399189937, 0.3413975472, 0.9877404807, -0.1561049093, -0.9034455656, 0.4287028224, 0.1269804218, -0.9919052235, -0.3819600854, 0.924178821,
         0.9754625894, 0.2201652486, -0.3204015856, -0.9472818081, -0.9874760884, 0.1577687387, 0.02535348474, -0.9996785487, 0.4835130794, -0.8753371362, -0.2850799925, -0.9585037287, -0.06805516006, -0.99768156, -0.7885244045, -0.6150034663,
         0.3185392127, -0.9479096845, 0.8880043089, 0.4598351306, 0.6476921488, -0.7619021462, 0.9820241299, 0.1887554194, 0.9357275128, -0.3527237187, -0.8894895414, 0.4569555293, 0.7922791302, 0.6101588153, 0.7483818261, 0.6632681526,
        -0.7288929755, -0.6846276581, 0.8729032783, -0.4878932944, 0.8288345784, 0.5594937369, 0.08074567077, 0.9967347374, 0.9799148216, -0.1994165048, -0.580730673, -0.8140957471, -0.4700049791, -0.8826637636, 0.2409492979, 0.9705377045,
         0.9437816757, -0.3305694308, -0.8927998638, -0.4504535528, -0.8069622304, 0.5906030467, 0.06258973166, 0.9980393407, -0.9312597469, 0.3643559849, 0.5777449785, 0.8162173362, -0.3360095855, -0.941858566, 0.697932075, -0.7161639607,
        -0.002008157227, -0.9999979837, -0.1827294312, -0.9831632392, -0.6523911722, 0.7578824173, -0.4302626911, -0.9027037258, -0.9985126289, -0.05452091251, -0.01028102172, -0.9999471489, -0.4946071129, 0.8691166802, -0.2999350194, 0.9539596344,
         0.8165471961, 0.5772786819, 0.2697460475, 0.962931498, -0.7306287391, -0.6827749597, -0.7590952064, -0.6509796216, -0.907053853, 0.4210146171, -0.5104861064, -0.8598860013, 0.8613350597, 0.5080373165, 0.5007881595, -0.8655698812,
        -0.654158152, 0.7563577938, -0.8382755311, -0.545246856, 0.6940070834, 0.7199681717, 0.06950936031, 0.9975812994, 0.1702942185, -0.9853932612, 0.2695973274, 0.9629731466, 0.5519612192, -0.8338697815, 0.225657487, -0.9742067022,
         0.4215262855, -0.9068161835, 0.4881873305, -0.8727388672, -0.3683854996, -0.9296731273, -0.9825390578, 0.1860564427, 0.81256471, 0.5828709909, 0.3196460933, -0.9475370046, 0.9570913859, 0.2897862643, -0.6876655497, -0.7260276109,
        -0.9988770922, -0.047376731, -0.1250179027, 0.992154486, -0.8280133617, 0.560708367, 0.9324863769, -0.3612051451, 0.6394653183, 0.7688199442, -0.01623847064, -0.9998681473, -0.9955014666, -0.09474613458, -0.81453315, 0.580117012,
         0.4037327978, -0.9148769469, 0.9944263371, 0.1054336766, -0.1624711654, 0.9867132919, -0.9949487814, -0.100383875, -0.6995302564, 0.7146029809, 0.5263414922, -0.85027327, -0.5395221479, 0.841971408, 0.6579370318, 0.7530729462,
         0.01426758847, -0.9998982128, -0.6734383991, 0.7392433447, 0.639412098, -0.7688642071, 0.9211571421, 0.3891908523, -0.146637214, -0.9891903394, -0.782318098, 0.6228791163, -0.5039610839, -0.8637263605, -0.7743120191, -0.6328039957,
    ];

    #[rustfmt::skip]
    const GRADIENTS_3D: [f32; 256] = [
        0., 1., 1., 0.,  0.,-1., 1., 0.,  0., 1.,-1., 0.,  0.,-1.,-1., 0.,
        1., 0., 1., 0., -1., 0., 1., 0.,  1., 0.,-1., 0., -1., 0.,-1., 0.,
        1., 1., 0., 0., -1., 1., 0., 0.,  1.,-1., 0., 0., -1.,-1., 0., 0.,
        0., 1., 1., 0.,  0.,-1., 1., 0.,  0., 1.,-1., 0.,  0.,-1.,-1., 0.,
        1., 0., 1., 0., -1., 0., 1., 0.,  1., 0.,-1., 0., -1., 0.,-1., 0.,
        1., 1., 0., 0., -1., 1., 0., 0.,  1.,-1., 0., 0., -1.,-1., 0., 0.,
        0., 1., 1., 0.,  0.,-1., 1., 0.,  0., 1.,-1., 0.,  0.,-1.,-1., 0.,
        1., 0., 1., 0., -1., 0., 1., 0.,  1., 0.,-1., 0., -1., 0.,-1., 0.,
        1., 1., 0., 0., -1., 1., 0., 0.,  1.,-1., 0., 0., -1.,-1., 0., 0.,
        0., 1., 1., 0.,  0.,-1., 1., 0.,  0., 1.,-1., 0.,  0.,-1.,-1., 0.,
        1., 0., 1., 0., -1., 0., 1., 0.,  1., 0.,-1., 0., -1., 0.,-1., 0.,
        1., 1., 0., 0., -1., 1., 0., 0.,  1.,-1., 0., 0., -1.,-1., 0., 0.,
        0., 1., 1., 0.,  0.,-1., 1., 0.,  0., 1.,-1., 0.,  0.,-1.,-1., 0.,
        1., 0., 1., 0., -1., 0., 1., 0.,  1., 0.,-1., 0., -1., 0.,-1., 0.,
        1., 1., 0., 0., -1., 1., 0., 0.,  1.,-1., 0., 0., -1.,-1., 0., 0.,
        1., 1., 0., 0.,  0.,-1., 1., 0., -1., 1., 0., 0.,  0.,-1.,-1., 0.,
    ];

    #[rustfmt::skip]
    const RAND_VECS_3D: [f32; 1024] = [
        -0.7292736885, -0.6618439697, 0.1735581948, 0., 0.790292081, -0.5480887466, -0.2739291014, 0., 0.7217578935, 0.6226212466, -0.3023380997, 0., 0.565683137, -0.8208298145, -0.0790000257, 0., 0.760049034, -0.5555979497, -0.3370999617, 0., 0.3713945616, 0.5011264475, 0.7816254623, 0., -0.1277062463, -0.4254438999, -0.8959289049, 0., -0.2881560924, -0.5815838982, 0.7607405838, 0.,
         0.5849561111, -0.662820239, -0.4674352136, 0., 0.3307171178, 0.0391653737, 0.94291689, 0., 0.8712121778, -0.4113374369, -0.2679381538, 0., 0.580981015, 0.7021915846, 0.4115677815, 0., 0.503756873, 0.6330056931, -0.5878203852, 0., 0.4493712205, 0.601390195, 0.6606022552, 0., -0.6878403724, 0.09018890807, -0.7202371714, 0., -0.5958956522, -0.6469350577, 0.475797649, 0.,
        -0.5127052122, 0.1946921978, -0.8361987284, 0., -0.9911507142, -0.05410276466, -0.1212153153, 0., -0.2149721042, 0.9720882117, -0.09397607749, 0., -0.7518650936, -0.5428057603, 0.3742469607, 0., 0.5237068895, 0.8516377189, -0.02107817834, 0., 0.6333504779, 0.1926167129, -0.7495104896, 0., -0.06788241606, 0.3998305789, 0.9140719259, 0., -0.5538628599, -0.4729896695, -0.6852128902, 0.,
        -0.7261455366, -0.5911990757, 0.3509933228, 0., -0.9229274737, -0.1782808786, 0.3412049336, 0., -0.6968815002, 0.6511274338, 0.3006480328, 0., 0.9608044783, -0.2098363234, -0.1811724921, 0., 0.06817146062, -0.9743405129, 0.2145069156, 0., -0.3577285196, -0.6697087264, -0.6507845481, 0., -0.1868621131, 0.7648617052, -0.6164974636, 0., -0.6541697588, 0.3967914832, 0.6439087246, 0.,
         0.6993340405, -0.6164538506, 0.3618239211, 0., -0.1546665739, 0.6291283928, 0.7617583057, 0., -0.6841612949, -0.2580482182, -0.6821542638, 0., 0.5383980957, 0.4258654885, 0.7271630328, 0., -0.5026987823, -0.7939832935, -0.3418836993, 0., 0.3202971715, 0.2834415347, 0.9039195862, 0., 0.8683227101, -0.0003762656404, -0.4959995258, 0., 0.791120031, -0.08511045745, 0.6057105799, 0.,
        -0.04011016052, -0.4397248749, 0.8972364289, 0., 0.9145119872, 0.3579346169, -0.1885487608, 0., -0.9612039066, -0.2756484276, 0.01024666929, 0., 0.6510361721, -0.2877799159, -0.7023778346, 0., -0.2041786351, 0.7365237271, 0.644859585, 0., -0.7718263711, 0.3790626912, 0.5104855816, 0., -0.3060082741, -0.7692987727, 0.5608371729, 0., 0.454007341, -0.5024843065, 0.7357899537, 0.,
         0.4816795475, 0.6021208291, -0.6367380315, 0., 0.6961980369, -0.3222197429, 0.641469197, 0., -0.6532160499, -0.6781148932, 0.3368515753, 0., 0.5089301236, -0.6154662304, -0.6018234363, 0., -0.1635919754, -0.9133604627, -0.372840892, 0., 0.52408019, -0.8437664109, 0.1157505864, 0., 0.5902587356, 0.4983817807, -0.6349883666, 0., 0.5863227872, 0.494764745, 0.6414307729, 0.,
         0.6779335087, 0.2341345225, 0.6968408593, 0., 0.7177054546, -0.6858979348, 0.120178631, 0., -0.5328819713, -0.5205125012, 0.6671608058, 0., -0.8654874251, -0.0700727088, -0.4960053754, 0., -0.2861810166, 0.7952089234, 0.5345495242, 0., -0.04849529634, 0.9810836427, -0.1874115585, 0., -0.6358521667, 0.6058348682, 0.4781800233, 0., 0.6254794696, -0.2861619734, 0.7258696564, 0.,
        -0.2585259868, 0.5061949264, -0.8227581726, 0., 0.02136306781, 0.5064016808, -0.8620330371, 0., 0.200111773, 0.8599263484, 0.4695550591, 0., 0.4743561372, 0.6014985084, -0.6427953014, 0., 0.6622993731, -0.5202474575, -0.5391679918, 0., 0.08084972818, -0.6532720452, 0.7527940996, 0., -0.6893687501, 0.0592860349, 0.7219805347, 0., -0.1121887082, -0.9673185067, 0.2273952515, 0.,
         0.7344116094, 0.5979668656, -0.3210532909, 0., 0.5789393465, -0.2488849713, 0.7764570201, 0., 0.6988182827, 0.3557169806, -0.6205791146, 0., -0.8636845529, -0.2748771249, -0.4224826141, 0., -0.4247027957, -0.4640880967, 0.777335046, 0., 0.5257722489, -0.8427017621, 0.1158329937, 0., 0.9343830603, 0.316302472, -0.1639543925, 0., -0.1016836419, -0.8057303073, -0.5834887393, 0.,
        -0.6529238969, 0.50602126, -0.5635892736, 0., -0.2465286165, -0.9668205684, -0.06694497494, 0., -0.9776897119, -0.2099250524, -0.007368825344, 0., 0.7736893337, 0.5734244712, 0.2694238123, 0., -0.6095087895, 0.4995678998, 0.6155736747, 0., 0.5794535482, 0.7434546771, 0.3339292269, 0., -0.8226211154, 0.08142581855, 0.5627293636, 0., -0.510385483, 0.4703667658, 0.7199039967, 0.,
        -0.5764971849, -0.07231656274, -0.8138926898, 0., 0.7250628871, 0.3949971505, -0.5641463116, 0., -0.1525424005, 0.4860840828, -0.8604958341, 0., -0.5550976208, -0.4957820792, 0.667882296, 0., -0.1883614327, 0.9145869398, 0.357841725, 0., 0.7625556724, -0.5414408243, -0.3540489801, 0., -0.5870231946, -0.3226498013, -0.7424963803, 0., 0.3051124198, 0.2262544068, -0.9250488391, 0.,
         0.6379576059, 0.577242424, -0.5097070502, 0., -0.5966775796, 0.1454852398, -0.7891830656, 0., -0.658330573, 0.6555487542, -0.3699414651, 0., 0.7434892426, 0.2351084581, 0.6260573129, 0., 0.5562114096, 0.8264360377, -0.0873632843, 0., -0.3028940016, -0.8251527185, 0.4768419182, 0., 0.1129343818, -0.985888439, -0.1235710781, 0., 0.5937652891, -0.5896813806, 0.5474656618, 0.,
         0.6757964092, -0.5835758614, -0.4502648413, 0., 0.7242302609, -0.1152719764, 0.6798550586, 0., -0.9511914166, 0.0753623979, -0.2992580792, 0., 0.2539470961, -0.1886339355, 0.9486454084, 0., 0.571433621, -0.1679450851, -0.8032795685, 0., -0.06778234979, 0.3978269256, 0.9149531629, 0., 0.6074972649, 0.733060024, -0.3058922593, 0., -0.5435478392, 0.1675822484, 0.8224791405, 0.,
        -0.5876678086, -0.3380045064, -0.7351186982, 0., -0.7967562402, 0.04097822706, -0.6029098428, 0., -0.1996350917, 0.8706294745, 0.4496111079, 0., -0.02787660336, -0.9106232682, -0.4122962022, 0., -0.7797625996, -0.6257634692, 0.01975775581, 0., -0.5211232846, 0.7401644346, -0.4249554471, 0., 0.8575424857, 0.4053272873, -0.3167501783, 0., 0.1045223322, 0.8390195772, -0.5339674439, 0.,
         0.3501822831, 0.9242524096, -0.1520850155, 0., 0.1987849858, 0.07647613266, 0.9770547224, 0., 0.7845996363, 0.6066256811, -0.1280964233, 0., 0.09006737436, -0.9750989929, -0.2026569073, 0., -0.8274343547, -0.542299559, 0.1458203587, 0., -0.3485797732, -0.415802277, 0.840000362, 0., -0.2471778936, -0.7304819962, -0.6366310879, 0., -0.3700154943, 0.8577948156, 0.3567584454, 0.,
         0.5913394901, -0.548311967, -0.5913303597, 0., 0.1204873514, -0.7626472379, -0.6354935001, 0., 0.616959265, 0.03079647928, 0.7863922953, 0., 0.1258156836, -0.6640829889, -0.7369967419, 0., -0.6477565124, -0.1740147258, -0.7417077429, 0., 0.6217889313, -0.7804430448, -0.06547655076, 0., 0.6589943422, -0.6096987708, 0.4404473475, 0., -0.2689837504, -0.6732403169, -0.6887635427, 0.,
        -0.3849775103, 0.5676542638, 0.7277093879, 0., 0.5754444408, 0.8110471154, -0.1051963504, 0., 0.9141593684, 0.3832947817, 0.131900567, 0., -0.107925319, 0.9245493968, 0.3654593525, 0., 0.377977089, 0.3043148782, 0.8743716458, 0., -0.2142885215, -0.8259286236, 0.5214617324, 0., 0.5802544474, 0.4148098596, -0.7008834116, 0., -0.1982660881, 0.8567161266, -0.4761596756, 0.,
        -0.03381553704, 0.3773180787, -0.9254661404, 0., -0.6867922841, -0.6656597827, 0.2919133642, 0., 0.7731742607, -0.2875793547, -0.5652430251, 0., -0.09655941928, 0.9193708367, -0.3813575004, 0., 0.2715702457, -0.9577909544, -0.09426605581, 0., 0.2451015704, -0.6917998565, -0.6792188003, 0., 0.977700782, -0.1753855374, 0.1155036542, 0., -0.5224739938, 0.8521606816, 0.02903615945, 0.,
        -0.7734880599, -0.5261292347, 0.3534179531, 0., -0.7134492443, -0.269547243, 0.6467878011, 0., 0.1644037271, 0.5105846203, -0.8439637196, 0., 0.6494635788, 0.05585611296, 0.7583384168, 0., -0.4711970882, 0.5017280509, -0.7254255765, 0., -0.6335764307, -0.2381686273, -0.7361091029, 0., -0.9021533097, -0.270947803, -0.3357181763, 0., -0.3793711033, 0.872258117, 0.3086152025, 0.,
        -0.6855598966, -0.3250143309, 0.6514394162, 0., 0.2900942212, -0.7799057743, -0.5546100667, 0., -0.2098319339, 0.85037073, 0.4825351604, 0., -0.4592603758, 0.6598504336, -0.5947077538, 0., 0.8715945488, 0.09616365406, -0.4807031248, 0., -0.6776666319, 0.7118504878, -0.1844907016, 0., 0.7044377633, 0.312427597, 0.637304036, 0., -0.7052318886, -0.2401093292, -0.6670798253, 0.,
         0.081921007, -0.7207336136, -0.6883545647, 0., -0.6993680906, -0.5875763221, -0.4069869034, 0., -0.1281454481, 0.6419895885, 0.7559286424, 0., -0.6337388239, -0.6785471501, -0.3714146849, 0., 0.5565051903, -0.2168887573, -0.8020356851, 0., -0.5791554484, 0.7244372011, -0.3738578718, 0., 0.1175779076, -0.7096451073, 0.6946792478, 0., -0.6134619607, 0.1323631078, 0.7785527795, 0.,
         0.6984635305, -0.02980516237, -0.715024719, 0., 0.8318082963, -0.3930171956, 0.3919597455, 0., 0.1469576422, 0.05541651717, -0.9875892167, 0., 0.708868575, -0.2690503865, 0.6520101478, 0., 0.2726053183, 0.67369766, -0.68688995, 0., -0.6591295371, 0.3035458599, -0.6880466294, 0., 0.4815131379, -0.7528270071, 0.4487723203, 0., 0.9430009463, 0.1675647412, -0.2875261255, 0.,
         0.434802957, 0.7695304522, -0.4677277752, 0., 0.3931996188, 0.594473625, 0.7014236729, 0., 0.7254336655, -0.603925654, 0.3301814672, 0., 0.7590235227, -0.6506083235, 0.02433313207, 0., -0.8552768592, -0.3430042733, 0.3883935666, 0., -0.6139746835, 0.6981725247, 0.3682257648, 0., -0.7465905486, -0.5752009504, 0.3342849376, 0., 0.5730065677, 0.810555537, -0.1210916791, 0.,
        -0.9225877367, -0.3475211012, -0.167514036, 0., -0.7105816789, -0.4719692027, -0.5218416899, 0., -0.08564609717, 0.3583001386, 0.929669703, 0., -0.8279697606, -0.2043157126, 0.5222271202, 0., 0.427944023, 0.278165994, 0.8599346446, 0., 0.5399079671, -0.7857120652, -0.3019204161, 0., 0.5678404253, -0.5495413974, -0.6128307303, 0., -0.9896071041, 0.1365639107, -0.04503418428, 0.,
        -0.6154342638, -0.6440875597, 0.4543037336, 0., 0.1074204368, -0.7946340692, 0.5975094525, 0., -0.3595449969, -0.8885529948, 0.28495784, 0., -0.2180405296, 0.1529888965, 0.9638738118, 0., -0.7277432317, -0.6164050508, -0.3007234646, 0., 0.7249729114, -0.00669719484, 0.6887448187, 0., -0.5553659455, -0.5336586252, 0.6377908264, 0., 0.5137558015, 0.7976208196, -0.3160000073, 0.,
        -0.3794024848, 0.9245608561, -0.03522751494, 0., 0.8229248658, 0.2745365933, -0.4974176556, 0., -0.5404114394, 0.6091141441, 0.5804613989, 0., 0.8036581901, -0.2703029469, 0.5301601931, 0., 0.6044318879, 0.6832968393, 0.4095943388, 0., 0.06389988817, 0.9658208605, -0.2512108074, 0., 0.1087113286, 0.7402471173, -0.6634877936, 0., -0.713427712, -0.6926784018, 0.1059128479, 0.,
         0.6458897819, -0.5724548511, -0.5050958653, 0., -0.6553931414, 0.7381471625, 0.159995615, 0., 0.3910961323, 0.9188871375, -0.05186755998, 0., -0.4879022471, -0.5904376907, 0.6429111375, 0., 0.6014790094, 0.7707441366, -0.2101820095, 0., -0.5677173047, 0.7511360995, 0.3368851762, 0., 0.7858573506, 0.226674665, 0.5753666838, 0., -0.4520345543, -0.604222686, -0.6561857263, 0.,
         0.002272116345, 0.4132844051, -0.9105991643, 0., -0.5815751419, -0.5162925989, 0.6286591339, 0., -0.03703704785, 0.8273785755, 0.5604221175, 0., -0.5119692504, 0.7953543429, -0.3244980058, 0., -0.2682417366, -0.9572290247, -0.1084387619, 0., -0.2322482736, -0.9679131102, -0.09594243324, 0., 0.3554328906, -0.8881505545, 0.2913006227, 0., 0.7346520519, -0.4371373164, 0.5188422971, 0.,
         0.9985120116, 0.04659011161, -0.02833944577, 0., -0.3727687496, -0.9082481361, 0.1900757285, 0., 0.91737377, -0.3483642108, 0.1925298489, 0., 0.2714911074, 0.4147529736, -0.8684886582, 0., 0.5131763485, -0.7116334161, 0.4798207128, 0., -0.8737353606, 0.18886992, -0.4482350644, 0., 0.8460043821, -0.3725217914, 0.3814499973, 0., 0.8978727456, -0.1780209141, -0.4026575304, 0.,
         0.2178065647, -0.9698322841, -0.1094789531, 0., -0.1518031304, -0.7788918132, -0.6085091231, 0., -0.2600384876, -0.4755398075, -0.8403819825, 0., 0.572313509, -0.7474340931, -0.3373418503, 0., -0.7174141009, 0.1699017182, -0.6756111411, 0., -0.684180784, 0.02145707593, -0.7289967412, 0., -0.2007447902, 0.06555605789, -0.9774476623, 0., -0.1148803697, -0.8044887315, 0.5827524187, 0.,
        -0.7870349638, 0.03447489231, 0.6159443543, 0., -0.2015596421, 0.6859872284, 0.6991389226, 0., -0.08581082512, -0.10920836, -0.9903080513, 0., 0.5532693395, 0.7325250401, -0.396610771, 0., -0.1842489331, -0.9777375055, -0.1004076743, 0., 0.0775473789, -0.9111505856, 0.4047110257, 0., 0.1399838409, 0.7601631212, -0.6344734459, 0., 0.4484419361, -0.845289248, 0.2904925424, 0.,
    ];

    // ==============
    // Math functions
    // ==============

    #[inline(always)]
    fn fast_floor(f: Float) -> i32 {
        if f >= 0. {
            f as i32
        } else {
            f as i32 - 1
        }
    }

    #[inline(always)]
    fn fast_round(f: Float) -> i32 {
        if f >= 0. {
            (f + 0.5) as i32
        } else {
            (f - 0.5) as i32
        }
    }

    #[inline(always)]
    fn lerp(a: f32, b: f32, t: f32) -> f32 {
        a + t * (b - a)
    }

    #[inline(always)]
    fn interp_hermite(t: f32) -> f32 {
        t * t * (t * -2. + 3.)
    }

    #[inline(always)]
    fn interp_quintic(t: f32) -> f32 {
        t * t * t * (t * (t * 6. - 15.) + 10.)
    }

    #[inline(always)]
    fn cubic_lerp(a: f32, b: f32, c: f32, d: f32, t: f32) -> f32 {
        let p = (d - c) - (a - b);
        t * t * t * p + t * t * ((a - b) - p) + t * (c - a) + b
    }

    #[inline(always)]
    fn ping_pong(t: f32) -> f32 {
        let t = t - FloatOps::trunc(t * 0.5) * 2.;

        if t < 1. {
            t
        } else {
            2. - t
        }
    }

    fn calculate_fractal_bounding(&mut self) {
        let gain = FloatOps::abs(self.gain);
        let mut amp = gain;
        let mut amp_fractal = 1.;
        for _ in 1..self.octaves {
            amp_fractal += amp;
            amp *= gain;
        }
        self.fractal_bounding = 1. / amp_fractal;
    }

    // ==============
    // Hash functions
    // ==============

    // Primes for hashing
    const PRIME_X: i32 = 501125321;
    const PRIME_Y: i32 = 1136930381;
    const PRIME_Z: i32 = 1720413743;
    const PRIME_X_2: i32 = Self::PRIME_X.wrapping_mul(2);
    const PRIME_Y_2: i32 = Self::PRIME_Y.wrapping_mul(2);
    const PRIME_Z_2: i32 = Self::PRIME_Z.wrapping_mul(2);

    #[inline(always)]
    fn hash_2d(seed: i32, x_primed: i32, y_primed: i32) -> i32 {
        let hash = seed ^ x_primed ^ y_primed;
        hash.wrapping_mul(0x27d4eb2d)
    }

    #[inline(always)]
    fn hash_3d(seed: i32, x_primed: i32, y_primed: i32, z_primed: i32) -> i32 {
        let hash = seed ^ x_primed ^ y_primed ^ z_primed;
        hash.wrapping_mul(0x27d4eb2d)
    }

    // ===================================
    // Internal noise generator algorithms
    // ===================================

    #[inline(always)]
    fn val_coord_2d(seed: i32, x_primed: i32, y_primed: i32) -> f32 {
        let hash = Self::hash_2d(seed, x_primed, y_primed);
        let hash = hash.wrapping_mul(hash);
        let hash = hash ^ (hash << 19);
        hash as f32 * (1. / 2147483648.)
    }

    #[inline(always)]
    fn val_coord_3d(seed: i32, x_primed: i32, y_primed: i32, z_primed: i32) -> f32 {
        let hash = Self::hash_3d(seed, x_primed, y_primed, z_primed);

        let hash = hash.wrapping_mul(hash);
        let hash = hash ^ (hash << 19);
        hash as f32 * (1. / 2147483648.)
    }

    #[inline(always)]
    fn grad_coord_2d(seed: i32, x_primed: i32, y_primed: i32, xd: f32, yd: f32) -> f32 {
        let hash = Self::hash_2d(seed, x_primed, y_primed);
        let hash = hash ^ (hash >> 15);
        let hash = hash & (127 << 1);

        let xg = Self::GRADIENTS_2D[hash as usize];
        let yg = Self::GRADIENTS_2D[(hash | 1) as usize];

        xd * xg + yd * yg
    }

    #[inline(always)]
    fn grad_coord_3d(
        seed: i32,
        x_primed: i32,
        y_primed: i32,
        z_primed: i32,
        xd: f32,
        yd: f32,
        zd: f32,
    ) -> f32 {
        let hash = Self::hash_3d(seed, x_primed, y_primed, z_primed);
        let hash = hash ^ (hash >> 15);
        let hash = hash & (63 << 2);

        let xg = Self::GRADIENTS_3D[hash as usize];
        let yg = Self::GRADIENTS_3D[(hash | 1) as usize];
        let zg = Self::GRADIENTS_3D[(hash | 2) as usize];

        xd * xg + yd * yg + zd * zg
    }

    #[inline(always)]
    fn grad_coord_out_2d(seed: i32, x_primed: i32, y_primed: i32) -> (f32, f32) {
        let hash = Self::hash_2d(seed, x_primed, y_primed) & (255 << 1);

        let xo = Self::RAND_VECS_2D[hash as usize];
        let yo = Self::RAND_VECS_2D[(hash | 1) as usize];

        (xo, yo)
    }

    #[inline(always)]
    fn grad_coord_out_3d(
        seed: i32,
        x_primed: i32,
        y_primed: i32,
        z_primed: i32,
    ) -> (f32, f32, f32) {
        let hash = Self::hash_3d(seed, x_primed, y_primed, z_primed) & (255 << 2);

        let xo = Self::RAND_VECS_3D[hash as usize];
        let yo = Self::RAND_VECS_3D[(hash | 1) as usize];
        let zo = Self::RAND_VECS_3D[(hash | 2) as usize];

        (xo, yo, zo)
    }

    #[inline(always)]
    fn grad_coord_dual_2d(seed: i32, x_primed: i32, y_primed: i32, xd: f32, yd: f32) -> (f32, f32) {
        let hash = Self::hash_2d(seed, x_primed, y_primed);
        let index1 = hash & (127 << 1);
        let index2 = (hash >> 7) & (255 << 1);

        let xg = Self::GRADIENTS_2D[index1 as usize];
        let yg = Self::GRADIENTS_2D[(index1 | 1) as usize];
        let value = xd * xg + yd * yg;

        let xgo = Self::RAND_VECS_2D[index2 as usize];
        let ygo = Self::RAND_VECS_2D[(index2 | 1) as usize];

        let xo = value * xgo;
        let yo = value * ygo;

        (xo, yo)
    }

    #[inline(always)]
    fn grad_coord_dual_3d(
        seed: i32,
        x_primed: i32,
        y_primed: i32,
        z_primed: i32,
        xd: f32,
        yd: f32,
        zd: f32,
    ) -> (f32, f32, f32) {
        let hash = Self::hash_3d(seed, x_primed, y_primed, z_primed);
        let index1 = hash & (63 << 2);
        let index2 = (hash >> 6) & (255 << 2);

        let xg = Self::GRADIENTS_3D[index1 as usize];
        let yg = Self::GRADIENTS_3D[(index1 | 1) as usize];
        let zg = Self::GRADIENTS_3D[(index1 | 2) as usize];
        let value = xd * xg + yd * yg + zd * zg;

        let xgo = Self::RAND_VECS_3D[index2 as usize];
        let ygo = Self::RAND_VECS_3D[(index2 | 1) as usize];
        let zgo = Self::RAND_VECS_3D[(index2 | 2) as usize];

        let xo = value * xgo;
        let yo = value * ygo;
        let zo = value * zgo;

        (xo, yo, zo)
    }

    // Generic noise gen

    fn gen_noise_single_2d(&self, seed: i32, x: Float, y: Float) -> f32 {
        match self.noise_type {
            NoiseType::OpenSimplex2 => self.single_simplex_2d(seed, x, y),
            NoiseType::OpenSimplex2S => self.single_open_simplex_2s_2d(seed, x, y),
            NoiseType::Cellular => self.single_cellular_2d(seed, x, y),
            NoiseType::Perlin => self.single_perlin_2d(seed, x, y),
            NoiseType::ValueCubic => self.single_value_cubic_2d(seed, x, y),
            NoiseType::Value => self.single_value_2d(seed, x, y),
        }
    }

    fn gen_noise_single_3d(&self, seed: i32, x: Float, y: Float, z: Float) -> f32 {
        match self.noise_type {
            NoiseType::OpenSimplex2 => self.single_open_simplex_2(seed, x, y, z),
            NoiseType::OpenSimplex2S => self.single_open_simplex_2s_3d(seed, x, y, z),
            NoiseType::Cellular => self.single_cellular_3d(seed, x, y, z),
            NoiseType::Perlin => self.single_perlin_3d(seed, x, y, z),
            NoiseType::ValueCubic => self.single_value_cubic_3d(seed, x, y, z),
            NoiseType::Value => self.single_value_3d(seed, x, y, z),
        }
    }

    // Noise Coordinate Transforms (frequency, and possible skew or rotation)

    #[inline(always)]
    fn transform_noise_coordinate_2d(&self, x: Float, y: Float) -> (Float, Float) {
        let mut x = x * self.frequency as Float;
        let mut y = y * self.frequency as Float;

        match self.noise_type {
            NoiseType::OpenSimplex2 | NoiseType::OpenSimplex2S => {
                let sqrt3 = 1.7320508075688772935274463415059;
                let f2 = 0.5 * (sqrt3 - 1.);
                let t = (x + y) * f2;

                x += t;
                y += t;
            }
            _ => {}
        }

        (x, y)
    }

    #[inline(always)]
    fn transform_noise_coordinate_3d(&self, x: Float, y: Float, z: Float) -> (Float, Float, Float) {
        let mut x = x * self.frequency as Float;
        let mut y = y * self.frequency as Float;
        let mut z = z * self.frequency as Float;

        match self.transform_type_3d {
            TransformType3D::ImproveXYPlanes => {
                let xy = x + y;
                let s2 = xy * -0.211324865405187;
                z *= 0.577350269189626;
                x += s2 - z;
                y = y + s2 - z;
                z += xy * 0.577350269189626;
            }
            TransformType3D::ImproveXZPlanes => {
                let xz = x + z;
                let s2 = xz * -0.211324865405187;
                y *= 0.577350269189626;
                x += s2 - y;
                z += s2 - y;
                y += xz * 0.577350269189626;
            }
            TransformType3D::DefaultOpenSimplex2 => {
                let r3 = 2. / 3.;
                let r = (x + y + z) * r3; // Rotation, not skew
                x = r - x;
                y = r - y;
                z = r - z;
            }
            _ => {}
        }

        (x, y, z)
    }

    fn update_transform_type_3d(&mut self) {
        match self.rotation_type_3d {
            RotationType3D::ImproveXYPlanes => {
                self.transform_type_3d = TransformType3D::ImproveXYPlanes;
            }
            RotationType3D::ImproveXZPlanes => {
                self.transform_type_3d = TransformType3D::ImproveXZPlanes;
            }
            _ => match self.noise_type {
                NoiseType::OpenSimplex2 | NoiseType::OpenSimplex2S => {
                    self.transform_type_3d = TransformType3D::DefaultOpenSimplex2;
                }
                _ => {
                    self.transform_type_3d = TransformType3D::None;
                }
            },
        }
    }

    // Domain Warp Coordinate Transforms

    #[inline(always)]
    fn transform_domain_warp_coordinate_2d(&self, x: Float, y: Float) -> (Float, Float) {
        let mut x = x;
        let mut y = y;

        match self.domain_warp_type {
            DomainWarpType::OpenSimplex2 | DomainWarpType::OpenSimplex2Reduced => {
                let sqrt3 = 1.7320508075688772935274463415059;
                let f2 = 0.5 * (sqrt3 - 1.);
                let t = (x + y) * f2;

                x += t;
                y += t;
            }
            _ => {}
        }

        (x, y)
    }

    #[inline(always)]
    fn transform_domain_warp_coordinate_3d(
        &self,
        x: Float,
        y: Float,
        z: Float,
    ) -> (Float, Float, Float) {
        let mut x = x;
        let mut y = y;
        let mut z = z;

        match self.warp_transform_type_3d {
            TransformType3D::ImproveXYPlanes => {
                let xy = x + y;
                let s2 = xy * -0.211324865405187;
                z *= 0.577350269189626;
                x += s2 - z;
                y = y + s2 - z;
                z += xy * 0.577350269189626;
            }
            TransformType3D::ImproveXZPlanes => {
                let xz = x + z;
                let s2 = xz * -0.211324865405187;
                y *= 0.577350269189626;
                x += s2 - y;
                z += s2 - y;
                y += xz * 0.577350269189626;
            }
            TransformType3D::DefaultOpenSimplex2 => {
                let r3 = 2. / 3.;
                let r = (x + y + z) * r3; // Rotation, not skew
                x = r - x;
                y = r - y;
                z = r - z;
            }
            _ => {}
        }

        (x, y, z)
    }

    fn update_warp_transform_type_3d(&mut self) {
        match self.rotation_type_3d {
            RotationType3D::ImproveXYPlanes => {
                self.warp_transform_type_3d = TransformType3D::ImproveXYPlanes;
            }
            RotationType3D::ImproveXZPlanes => {
                self.warp_transform_type_3d = TransformType3D::ImproveXZPlanes;
            }
            _ => match self.domain_warp_type {
                DomainWarpType::OpenSimplex2 | DomainWarpType::OpenSimplex2Reduced => {
                    self.warp_transform_type_3d = TransformType3D::DefaultOpenSimplex2;
                }
                _ => {
                    self.warp_transform_type_3d = TransformType3D::None;
                }
            },
        }
    }

    // Fractal FBm

    fn gen_fractal_fbm_2d(&self, x: Float, y: Float) -> f32 {
        let mut x = x;
        let mut y = y;

        let mut seed = self.seed;
        let mut sum = 0.;
        let mut amp = self.fractal_bounding;

        for _ in 0..self.octaves {
            let noise = self.gen_noise_single_2d(seed, x, y);

            seed += 1;
            sum += noise * amp;
            amp *= Self::lerp(1., (noise + 1.).min(2.) * 0.5, self.weighted_strength);

            x *= self.lacunarity as Float;
            y *= self.lacunarity as Float;
            amp *= self.gain;
        }

        sum
    }

    fn gen_fractal_fbm_3d(&self, x: Float, y: Float, z: Float) -> f32 {
        let mut x = x;
        let mut y = y;
        let mut z = z;

        let mut seed = self.seed;
        let mut sum = 0.;
        let mut amp = self.fractal_bounding;

        for _ in 0..self.octaves {
            let noise = self.gen_noise_single_3d(seed, x, y, z);

            seed += 1;
            sum += noise * amp;
            amp *= Self::lerp(1., (noise + 1.) * 0.5, self.weighted_strength);

            x *= self.lacunarity as Float;
            y *= self.lacunarity as Float;
            z *= self.lacunarity as Float;
            amp *= self.gain;
        }

        sum
    }

    // Fractal Ridged

    fn gen_fractal_ridged_2d(&self, x: Float, y: Float) -> f32 {
        let mut x = x;
        let mut y = y;

        let mut seed = self.seed;
        let mut sum = 0.;
        let mut amp = self.fractal_bounding;

        for _ in 0..self.octaves {
            let noise = FloatOps::abs(self.gen_noise_single_2d(seed, x, y));
            seed += 1;

            sum += (noise * -2. + 1.) * amp;
            amp *= Self::lerp(1., 1. - noise, self.weighted_strength);

            x *= self.lacunarity as Float;
            y *= self.lacunarity as Float;
            amp *= self.gain;
        }

        sum
    }

    fn gen_fractal_ridged_3d(&self, x: Float, y: Float, z: Float) -> f32 {
        let mut x = x;
        let mut y = y;
        let mut z = z;

        let mut seed = self.seed;
        let mut sum = 0.;
        let mut amp = self.fractal_bounding;

        for _ in 0..self.octaves {
            let noise = FloatOps::abs(self.gen_noise_single_3d(seed, x, y, z));
            seed += 1;

            sum += (noise * -2. + 1.) * amp;
            amp *= Self::lerp(1., 1. - noise, self.weighted_strength);

            x *= self.lacunarity as Float;
            y *= self.lacunarity as Float;
            z *= self.lacunarity as Float;
            amp *= self.gain;
        }

        sum
    }

    // Fractal PingPong

    fn gen_fractal_ping_pong_2d(&self, x: Float, y: Float) -> f32 {
        let mut x = x;
        let mut y = y;

        let mut seed = self.seed;
        let mut sum = 0.;
        let mut amp = self.fractal_bounding; // DEBUG: This is 0.400000006, should be 0.4

        for _ in 0..self.octaves {
            let noise = Self::ping_pong(
                (self.gen_noise_single_2d(seed, x, y) + 1.) * self.ping_pong_strength,
            );
            seed += 1;

            sum += (noise - 0.5) * 2. * amp;
            amp *= Self::lerp(1., noise, self.weighted_strength);

            x *= self.lacunarity as Float;
            y *= self.lacunarity as Float;
            amp *= self.gain;
        }

        sum
    }

    fn gen_fractal_ping_pong_3d(&self, x: Float, y: Float, z: Float) -> f32 {
        let mut x = x;
        let mut y = y;
        let mut z = z;

        let mut seed = self.seed;
        let mut sum = 0.;
        let mut amp = self.fractal_bounding;

        for _ in 0..self.octaves {
            let noise = Self::ping_pong(
                (self.gen_noise_single_3d(seed, x, y, z) + 1.) * self.ping_pong_strength,
            );
            seed += 1;

            sum += (noise - 0.5) * 2. * amp;
            amp *= Self::lerp(1., noise, self.weighted_strength);

            x *= self.lacunarity as Float;
            y *= self.lacunarity as Float;
            z *= self.lacunarity as Float;
            amp *= self.gain;
        }

        sum
    }

    // Simplex/OpenSimplex2 Noise

    fn single_simplex_2d(&self, seed: i32, x: Float, y: Float) -> f32 {
        // 2D OpenSimplex2 case uses the same algorithm as ordinary Simplex.

        let sqrt3 = 1.7320508075688772935274463415059;
        let g2 = (3. - sqrt3) / 6.;

        /*
         * --- Skew moved to TransformNoiseCoordinateXY method ---
         * let f2 = 0.5 * (sqrt3 - 1.);
         * let s = (x + y) * f2;
         * x += s; y += s;
         */

        let i = Self::fast_floor(x);
        let j = Self::fast_floor(y);
        #[allow(clippy::unnecessary_cast)]
        let xi = (x - i as Float) as f32;
        #[allow(clippy::unnecessary_cast)]
        let yi = (y - j as Float) as f32;

        let t = (xi + yi) * g2;
        let x0 = xi - t;
        let y0 = yi - t;

        let i = i.wrapping_mul(Self::PRIME_X);
        let j = j.wrapping_mul(Self::PRIME_Y);

        let a = 0.5 - x0 * x0 - y0 * y0;
        let n0 = if a <= 0. {
            0.
        } else {
            (a * a) * (a * a) * Self::grad_coord_2d(seed, i, j, x0, y0)
        };

        let c = (2. * (1. - 2. * g2) * (1. / g2 - 2.)) * t
            + ((-2. * (1. - 2. * g2) * (1. - 2. * g2)) + a);
        let n2 = if c <= 0. {
            0.
        } else {
            let x2 = x0 + (2. * g2 - 1.);
            let y2 = y0 + (2. * g2 - 1.);

            (c * c)
                * (c * c)
                * Self::grad_coord_2d(
                    seed,
                    i.wrapping_add(Self::PRIME_X),
                    j.wrapping_add(Self::PRIME_Y),
                    x2,
                    y2,
                )
        };

        let n1 = if y0 > x0 {
            let x1 = x0 + g2;
            let y1 = y0 + (g2 - 1.);
            let b = 0.5 - x1 * x1 - y1 * y1;

            if b <= 0. {
                0.
            } else {
                (b * b)
                    * (b * b)
                    * Self::grad_coord_2d(seed, i, j.wrapping_add(Self::PRIME_Y), x1, y1)
            }
        } else {
            let x1 = x0 + (g2 - 1.);
            let y1 = y0 + g2;
            let b = 0.5 - x1 * x1 - y1 * y1;

            if b <= 0. {
                0.
            } else {
                (b * b)
                    * (b * b)
                    * Self::grad_coord_2d(seed, i.wrapping_add(Self::PRIME_X), j, x1, y1)
            }
        };

        (n0 + n1 + n2) * 99.83685446303647
    }

    fn single_open_simplex_2(&self, seed: i32, x: Float, y: Float, z: Float) -> f32 {
        // 3D OpenSimplex2 case uses two offset rotated cube grids.

        /*
         * --- Rotation moved to TransformNoiseCoordinateXYZ method ---
         * let r3 = 2. / 3.;
         * let r = (x + y + z) * r3; // Rotation, not skew
         * x = r - x; y = r - y; z = r - z;
         */

        let mut seed = seed;

        let i = Self::fast_round(x);
        let j = Self::fast_round(y);
        let k = Self::fast_round(z);
        #[allow(clippy::unnecessary_cast)]
        let mut x0 = (x - i as Float) as f32;
        #[allow(clippy::unnecessary_cast)]
        let mut y0 = (y - j as Float) as f32;
        #[allow(clippy::unnecessary_cast)]
        let mut z0 = (z - k as Float) as f32;

        let mut x_n_sign = (-1. - x0) as i32 | 1;
        let mut y_n_sign = (-1. - y0) as i32 | 1;
        let mut z_n_sign = (-1. - z0) as i32 | 1;

        let mut ax0 = x_n_sign as f32 * -x0;
        let mut ay0 = y_n_sign as f32 * -y0;
        let mut az0 = z_n_sign as f32 * -z0;

        let mut i = i.wrapping_mul(Self::PRIME_X);
        let mut j = j.wrapping_mul(Self::PRIME_Y);
        let mut k = k.wrapping_mul(Self::PRIME_Z);

        let mut value = 0.;
        let mut a = (0.6 - x0 * x0) - (y0 * y0 + z0 * z0);

        let mut l = 0;
        loop {
            if a > 0. {
                value += (a * a) * (a * a) * Self::grad_coord_3d(seed, i, j, k, x0, y0, z0);
            }

            if ax0 >= ay0 && ax0 >= az0 {
                let b = a + ax0 + ax0;
                if b > 1. {
                    let b = b - 1.;
                    value += (b * b)
                        * (b * b)
                        * Self::grad_coord_3d(
                            seed,
                            i.wrapping_sub(x_n_sign.wrapping_mul(Self::PRIME_X)),
                            j,
                            k,
                            x0 + x_n_sign as f32,
                            y0,
                            z0,
                        );
                }
            } else if ay0 > ax0 && ay0 >= az0 {
                let b = a + ay0 + ay0;
                if b > 1. {
                    let b = b - 1.;
                    value += (b * b)
                        * (b * b)
                        * Self::grad_coord_3d(
                            seed,
                            i,
                            j.wrapping_sub(y_n_sign.wrapping_mul(Self::PRIME_Y)),
                            k,
                            x0,
                            y0 + y_n_sign as f32,
                            z0,
                        );
                }
            } else {
                let b = a + az0 + az0;
                if b > 1. {
                    let b = b - 1.;
                    value += (b * b)
                        * (b * b)
                        * Self::grad_coord_3d(
                            seed,
                            i,
                            j,
                            k.wrapping_sub(z_n_sign.wrapping_mul(Self::PRIME_Z)),
                            x0,
                            y0,
                            z0 + z_n_sign as f32,
                        );
                }
            }

            if l == 1 {
                break;
            }

            ax0 = 0.5 - ax0;
            ay0 = 0.5 - ay0;
            az0 = 0.5 - az0;

            x0 = x_n_sign as f32 * ax0;
            y0 = y_n_sign as f32 * ay0;
            z0 = z_n_sign as f32 * az0;

            a = a + (0.75 - ax0) - (ay0 + az0);

            i = i.wrapping_add((x_n_sign >> 1) & Self::PRIME_X);
            j = j.wrapping_add((y_n_sign >> 1) & Self::PRIME_Y);
            k = k.wrapping_add((z_n_sign >> 1) & Self::PRIME_Z);

            x_n_sign = -x_n_sign;
            y_n_sign = -y_n_sign;
            z_n_sign = -z_n_sign;

            seed = !seed;

            l += 1;
        }

        value * 32.69428253173828125
    }

    // OpenSimplex2S Noise

    fn single_open_simplex_2s_2d(&self, seed: i32, x: Float, y: Float) -> f32 {
        // 2D OpenSimplex2S case is a modified 2D simplex noise.

        let sqrt3 = 1.7320508075688772935274463415059;
        let g2 = (3. - sqrt3) / 6.;

        /*
         * --- Skew moved to TransformNoiseCoordinateXY method ---
         * let f2 = 0.5 * (sqrt3 - 1);
         * let s = (x + y) * f2;
         * x += s; y += s;
         */

        let i = Self::fast_floor(x);
        let j = Self::fast_floor(y);
        #[allow(clippy::unnecessary_cast)]
        let xi = (x - i as Float) as f32;
        #[allow(clippy::unnecessary_cast)]
        let yi = (y - j as Float) as f32;

        let i = i.wrapping_mul(Self::PRIME_X);
        let j = j.wrapping_mul(Self::PRIME_Y);
        let i1 = i.wrapping_add(Self::PRIME_X);
        let j1 = j.wrapping_add(Self::PRIME_Y);

        let t = (xi + yi) * g2;
        let x0 = xi - t;
        let y0 = yi - t;

        let a0 = (2. / 3.) - x0 * x0 - y0 * y0;
        let mut value = (a0 * a0) * (a0 * a0) * Self::grad_coord_2d(seed, i, j, x0, y0);

        let a1 = (2. * (1. - 2. * g2) * (1. / g2 - 2.)) * t
            + ((-2. * (1. - 2. * g2) * (1. - 2. * g2)) + a0);
        let x1 = x0 - (1. - 2. * g2);
        let y1 = y0 - (1. - 2. * g2);
        value += (a1 * a1) * (a1 * a1) * Self::grad_coord_2d(seed, i1, j1, x1, y1);

        // Nested conditionals were faster than compact bit logic/arithmetic.
        let xmyi = xi - yi;
        if t > g2 {
            if xi + xmyi > 1. {
                let x2 = x0 + (3. * g2 - 2.);
                let y2 = y0 + (3. * g2 - 1.);
                let a2 = (2. / 3.) - x2 * x2 - y2 * y2;
                if a2 > 0. {
                    value += (a2 * a2)
                        * (a2 * a2)
                        * Self::grad_coord_2d(
                            seed,
                            i.wrapping_add(Self::PRIME_X << 1),
                            j.wrapping_add(Self::PRIME_Y),
                            x2,
                            y2,
                        )
                }
            } else {
                let x2 = x0 + g2;
                let y2 = y0 + (g2 - 1.);
                let a2 = (2. / 3.) - x2 * x2 - y2 * y2;
                if a2 > 0. {
                    value += (a2 * a2)
                        * (a2 * a2)
                        * Self::grad_coord_2d(seed, i, j.wrapping_add(Self::PRIME_Y), x2, y2)
                }
            }

            if yi - xmyi > 1. {
                let x3 = x0 + (3. * g2 - 1.);
                let y3 = y0 + (3. * g2 - 2.);
                let a3 = (2. / 3.) - x3 * x3 - y3 * y3;
                if a3 > 0. {
                    value += (a3 * a3)
                        * (a3 * a3)
                        * Self::grad_coord_2d(
                            seed,
                            i.wrapping_add(Self::PRIME_X),
                            j.wrapping_add(Self::PRIME_Y << 1),
                            x3,
                            y3,
                        )
                }
            } else {
                let x3 = x0 + (g2 - 1.);
                let y3 = y0 + g2;
                let a3 = (2. / 3.) - x3 * x3 - y3 * y3;
                if a3 > 0. {
                    value += (a3 * a3)
                        * (a3 * a3)
                        * Self::grad_coord_2d(seed, i.wrapping_add(Self::PRIME_X), j, x3, y3)
                }
            }
        } else {
            if xi + xmyi < 0. {
                let x2 = x0 + (1. - g2);
                let y2 = y0 - g2;
                let a2 = (2. / 3.) - x2 * x2 - y2 * y2;
                if a2 > 0. {
                    value += (a2 * a2)
                        * (a2 * a2)
                        * Self::grad_coord_2d(seed, i.wrapping_sub(Self::PRIME_X), j, x2, y2)
                }
            } else {
                let x2 = x0 + (g2 - 1.);
                let y2 = y0 + g2;
                let a2 = (2. / 3.) - x2 * x2 - y2 * y2;
                if a2 > 0. {
                    value += (a2 * a2)
                        * (a2 * a2)
                        * Self::grad_coord_2d(seed, i.wrapping_add(Self::PRIME_X), j, x2, y2)
                }
            }

            if yi < xmyi {
                let x2 = x0 - g2;
                let y2 = y0 - (g2 - 1.);
                let a2 = (2. / 3.) - x2 * x2 - y2 * y2;
                if a2 > 0. {
                    value += (a2 * a2)
                        * (a2 * a2)
                        * Self::grad_coord_2d(seed, i, j.wrapping_sub(Self::PRIME_Y), x2, y2)
                }
            } else {
                let x2 = x0 + g2;
                let y2 = y0 + (g2 - 1.);
                let a2 = (2. / 3.) - x2 * x2 - y2 * y2;
                if a2 > 0. {
                    value += (a2 * a2)
                        * (a2 * a2)
                        * Self::grad_coord_2d(seed, i, j.wrapping_add(Self::PRIME_Y), x2, y2)
                }
            }
        }

        value * 18.24196194486065
    }

    fn single_open_simplex_2s_3d(&self, seed: i32, x: Float, y: Float, z: Float) -> f32 {
        // 3D OpenSimplex2S case uses two offset rotated cube grids.

        /*
         * --- Rotation moved to TransformNoiseCoordinateXYZ method ---
         * let R3 = 2. / 3.;
         * let r = (x + y + z) * R3; // Rotation, not skew
         * x = r - x; y = r - y; z = r - z;
        	*/

        let i = Self::fast_floor(x);
        let j = Self::fast_floor(y);
        let k = Self::fast_floor(z);
        #[allow(clippy::unnecessary_cast)]
        let xi = (x - i as Float) as f32;
        #[allow(clippy::unnecessary_cast)]
        let yi = (y - j as Float) as f32;
        #[allow(clippy::unnecessary_cast)]
        let zi = (z - k as Float) as f32;

        let i = i.wrapping_mul(Self::PRIME_X);
        let j = j.wrapping_mul(Self::PRIME_Y);
        let k = k.wrapping_mul(Self::PRIME_Z);
        let seed2 = seed + 1293373;

        let x_n_mask = (-0.5 - xi) as i32;
        let y_n_mask = (-0.5 - yi) as i32;
        let z_n_mask = (-0.5 - zi) as i32;

        let x0 = xi + x_n_mask as f32;
        let y0 = yi + y_n_mask as f32;
        let z0 = zi + z_n_mask as f32;
        let a0 = 0.75 - x0 * x0 - y0 * y0 - z0 * z0;
        let mut value = (a0 * a0)
            * (a0 * a0)
            * Self::grad_coord_3d(
                seed,
                i.wrapping_add(x_n_mask & Self::PRIME_X),
                j.wrapping_add(y_n_mask & Self::PRIME_Y),
                k.wrapping_add(z_n_mask & Self::PRIME_Z),
                x0,
                y0,
                z0,
            );

        let x1 = xi - 0.5;
        let y1 = yi - 0.5;
        let z1 = zi - 0.5;
        let a1 = 0.75 - x1 * x1 - y1 * y1 - z1 * z1;
        value += (a1 * a1)
            * (a1 * a1)
            * Self::grad_coord_3d(
                seed2,
                i.wrapping_add(Self::PRIME_X),
                j.wrapping_add(Self::PRIME_Y),
                k.wrapping_add(Self::PRIME_Z),
                x1,
                y1,
                z1,
            );

        let x_a_flip_mask_0 = ((x_n_mask | 1) << 1) as f32 * x1;
        let y_a_flip_mask_0 = ((y_n_mask | 1) << 1) as f32 * y1;
        let z_a_flip_mask_0 = ((z_n_mask | 1) << 1) as f32 * z1;
        let x_a_flip_mask_1 = (-2 - (x_n_mask << 2)) as f32 * x1 - 1.;
        let y_a_flip_mask_1 = (-2 - (y_n_mask << 2)) as f32 * y1 - 1.;
        let z_a_flip_mask_1 = (-2 - (z_n_mask << 2)) as f32 * z1 - 1.;

        let mut skip_5 = false;
        let a2 = x_a_flip_mask_0 + a0;
        if a2 > 0. {
            let x2 = x0 - (x_n_mask | 1) as f32;
            let y2 = y0;
            let z2 = z0;
            value += (a2 * a2)
                * (a2 * a2)
                * Self::grad_coord_3d(
                    seed,
                    i.wrapping_add(!x_n_mask & Self::PRIME_X),
                    j.wrapping_add(y_n_mask & Self::PRIME_Y),
                    k.wrapping_add(z_n_mask & Self::PRIME_Z),
                    x2,
                    y2,
                    z2,
                );
        } else {
            let a3 = y_a_flip_mask_0 + z_a_flip_mask_0 + a0;
            if a3 > 0. {
                let x3 = x0;
                let y3 = y0 - (y_n_mask | 1) as f32;
                let z3 = z0 - (z_n_mask | 1) as f32;
                value += (a3 * a3)
                    * (a3 * a3)
                    * Self::grad_coord_3d(
                        seed,
                        i.wrapping_add(x_n_mask & Self::PRIME_X),
                        j.wrapping_add(!y_n_mask & Self::PRIME_Y),
                        k.wrapping_add(!z_n_mask & Self::PRIME_Z),
                        x3,
                        y3,
                        z3,
                    );
            }

            let a4 = x_a_flip_mask_1 + a1;
            if a4 > 0. {
                let x4 = (x_n_mask | 1) as f32 + x1;
                let y4 = y1;
                let z4 = z1;
                value += (a4 * a4)
                    * (a4 * a4)
                    * Self::grad_coord_3d(
                        seed2,
                        i.wrapping_add(x_n_mask & Self::PRIME_X_2),
                        j.wrapping_add(Self::PRIME_Y),
                        k.wrapping_add(Self::PRIME_Z),
                        x4,
                        y4,
                        z4,
                    );
                skip_5 = true;
            }
        }

        let mut skip_9 = false;
        let a6 = y_a_flip_mask_0 + a0;
        if a6 > 0. {
            let x6 = x0;
            let y6 = y0 - (y_n_mask | 1) as f32;
            let z6 = z0;
            value += (a6 * a6)
                * (a6 * a6)
                * Self::grad_coord_3d(
                    seed,
                    i.wrapping_add(x_n_mask & Self::PRIME_X),
                    j.wrapping_add(!y_n_mask & Self::PRIME_Y),
                    k.wrapping_add(z_n_mask & Self::PRIME_Z),
                    x6,
                    y6,
                    z6,
                );
        } else {
            let a7 = x_a_flip_mask_0 + z_a_flip_mask_0 + a0;
            if a7 > 0. {
                let x7 = x0 - (x_n_mask | 1) as f32;
                let y7 = y0;
                let z7 = z0 - (z_n_mask | 1) as f32;
                value += (a7 * a7)
                    * (a7 * a7)
                    * Self::grad_coord_3d(
                        seed,
                        i.wrapping_add(!x_n_mask & Self::PRIME_X),
                        j.wrapping_add(y_n_mask & Self::PRIME_Y),
                        k.wrapping_add(!z_n_mask & Self::PRIME_Z),
                        x7,
                        y7,
                        z7,
                    );
            }

            let a8 = y_a_flip_mask_1 + a1;
            if a8 > 0. {
                let x8 = x1;
                let y8 = (y_n_mask | 1) as f32 + y1;
                let z8 = z1;
                value += (a8 * a8)
                    * (a8 * a8)
                    * Self::grad_coord_3d(
                        seed2,
                        i.wrapping_add(Self::PRIME_X),
                        j.wrapping_add(y_n_mask & (Self::PRIME_Y << 1)),
                        k.wrapping_add(Self::PRIME_Z),
                        x8,
                        y8,
                        z8,
                    );
                skip_9 = true;
            }
        }

        let mut skip_d = false;
        let a_a = z_a_flip_mask_0 + a0;
        if a_a > 0. {
            let x_a = x0;
            let y_a = y0;
            let z_a = z0 - (z_n_mask | 1) as f32;
            value += (a_a * a_a)
                * (a_a * a_a)
                * Self::grad_coord_3d(
                    seed,
                    i.wrapping_add(x_n_mask & Self::PRIME_X),
                    j.wrapping_add(y_n_mask & Self::PRIME_Y),
                    k.wrapping_add(!z_n_mask & Self::PRIME_Z),
                    x_a,
                    y_a,
                    z_a,
                );
        } else {
            let a_b = x_a_flip_mask_0 + y_a_flip_mask_0 + a0;
            if a_b > 0. {
                let x_b = x0 - (x_n_mask | 1) as f32;
                let y_b = y0 - (y_n_mask | 1) as f32;
                let z_b = z0;
                value += (a_b * a_b)
                    * (a_b * a_b)
                    * Self::grad_coord_3d(
                        seed,
                        i.wrapping_add(!x_n_mask & Self::PRIME_X),
                        j.wrapping_add(!y_n_mask & Self::PRIME_Y),
                        k.wrapping_add(z_n_mask & Self::PRIME_Z),
                        x_b,
                        y_b,
                        z_b,
                    );
            }

            let a_c = z_a_flip_mask_1 + a1;
            if a_c > 0. {
                let x_c = x1;
                let y_c = y1;
                let z_c = (z_n_mask | 1) as f32 + z1;
                value += (a_c * a_c)
                    * (a_c * a_c)
                    * Self::grad_coord_3d(
                        seed2,
                        i.wrapping_add(Self::PRIME_X),
                        j.wrapping_add(Self::PRIME_Y),
                        k.wrapping_add(z_n_mask & (Self::PRIME_Z << 1)),
                        x_c,
                        y_c,
                        z_c,
                    );
                skip_d = true;
            }
        }

        if !skip_5 {
            let a5 = y_a_flip_mask_1 + z_a_flip_mask_1 + a1;
            if a5 > 0. {
                let x5 = x1;
                let y5 = (y_n_mask | 1) as f32 + y1;
                let z5 = (z_n_mask | 1) as f32 + z1;
                value += (a5 * a5)
                    * (a5 * a5)
                    * Self::grad_coord_3d(
                        seed2,
                        i.wrapping_add(Self::PRIME_X),
                        j.wrapping_add(y_n_mask & (Self::PRIME_Y << 1)),
                        k.wrapping_add(z_n_mask & (Self::PRIME_Z << 1)),
                        x5,
                        y5,
                        z5,
                    );
            }
        }

        if !skip_9 {
            let a9 = x_a_flip_mask_1 + z_a_flip_mask_1 + a1;
            if a9 > 0. {
                let x9 = (x_n_mask | 1) as f32 + x1;
                let y9 = y1;
                let z9 = (z_n_mask | 1) as f32 + z1;
                value += (a9 * a9)
                    * (a9 * a9)
                    * Self::grad_coord_3d(
                        seed2,
                        i.wrapping_add(x_n_mask & Self::PRIME_X_2),
                        j.wrapping_add(Self::PRIME_Y),
                        k.wrapping_add(z_n_mask & (Self::PRIME_Z << 1)),
                        x9,
                        y9,
                        z9,
                    );
            }
        }

        if !skip_d {
            let a_d = x_a_flip_mask_1 + y_a_flip_mask_1 + a1;
            if a_d > 0. {
                let x_d = (x_n_mask | 1) as f32 + x1;
                let y_d = (y_n_mask | 1) as f32 + y1;
                let z_d = z1;
                value += (a_d * a_d)
                    * (a_d * a_d)
                    * Self::grad_coord_3d(
                        seed2,
                        i.wrapping_add(x_n_mask & (Self::PRIME_X << 1)),
                        j.wrapping_add(y_n_mask & (Self::PRIME_Y << 1)),
                        k.wrapping_add(Self::PRIME_Z),
                        x_d,
                        y_d,
                        z_d,
                    );
            }
        }

        value * 9.046026385208288
    }

    // Cellular Noise

    fn single_cellular_2d(&self, seed: i32, x: Float, y: Float) -> f32 {
        let xr = Self::fast_round(x);
        let yr = Self::fast_round(y);

        let mut distance0 = f32::MAX;
        let mut distance1 = f32::MAX;
        let mut closest_hash = 0;

        let cellular_jitter = 0.43701595 * self.cellular_jitter_modifier;

        let mut x_primed = (xr - 1).wrapping_mul(Self::PRIME_X);
        let y_primed_base = (yr - 1).wrapping_mul(Self::PRIME_Y);

        match self.cellular_distance_function {
            CellularDistanceFunction::Euclidean | CellularDistanceFunction::EuclideanSq => {
                for xi in xr - 1..=xr + 1 {
                    let mut y_primed = y_primed_base;

                    for yi in yr - 1..=yr + 1 {
                        let hash = Self::hash_2d(seed, x_primed, y_primed);
                        let idx = hash & (255 << 1);

                        #[allow(clippy::unnecessary_cast)]
                        let vec_x = (xi as Float - x) as f32
                            + Self::RAND_VECS_2D[idx as usize] * cellular_jitter;
                        #[allow(clippy::unnecessary_cast)]
                        let vec_y = (yi as Float - y) as f32
                            + Self::RAND_VECS_2D[(idx | 1) as usize] * cellular_jitter;

                        let new_distance = vec_x * vec_x + vec_y * vec_y;

                        distance1 = distance1.min(new_distance).max(distance0);
                        if new_distance < distance0 {
                            distance0 = new_distance;
                            closest_hash = hash;
                        }
                        y_primed = y_primed.wrapping_add(Self::PRIME_Y);
                    }
                    x_primed = x_primed.wrapping_add(Self::PRIME_X);
                }
            }
            CellularDistanceFunction::Manhattan => {
                for xi in xr - 1..=xr + 1 {
                    let mut y_primed = y_primed_base;

                    for yi in yr - 1..=yr + 1 {
                        let hash = Self::hash_2d(seed, x_primed, y_primed);
                        let idx = hash & (255 << 1);

                        #[allow(clippy::unnecessary_cast)]
                        let vec_x = (xi as Float - x) as f32
                            + Self::RAND_VECS_2D[idx as usize] * cellular_jitter;
                        #[allow(clippy::unnecessary_cast)]
                        let vec_y = (yi as Float - y) as f32
                            + Self::RAND_VECS_2D[(idx | 1) as usize] * cellular_jitter;

                        let new_distance = FloatOps::abs(vec_x) + FloatOps::abs(vec_y);

                        distance1 = distance1.min(new_distance).max(distance0);
                        if new_distance < distance0 {
                            distance0 = new_distance;
                            closest_hash = hash;
                        }
                        y_primed = y_primed.wrapping_add(Self::PRIME_Y);
                    }
                    x_primed = x_primed.wrapping_add(Self::PRIME_X);
                }
            }
            CellularDistanceFunction::Hybrid => {
                for xi in xr - 1..=xr + 1 {
                    let mut y_primed = y_primed_base;

                    for yi in yr - 1..=yr + 1 {
                        let hash = Self::hash_2d(seed, x_primed, y_primed);
                        let idx = hash & (255 << 1);

                        #[allow(clippy::unnecessary_cast)]
                        let vec_x = (xi as Float - x) as f32
                            + Self::RAND_VECS_2D[idx as usize] * cellular_jitter;
                        #[allow(clippy::unnecessary_cast)]
                        let vec_y = (yi as Float - y) as f32
                            + Self::RAND_VECS_2D[(idx | 1) as usize] * cellular_jitter;

                        let new_distance = (FloatOps::abs(vec_x) + FloatOps::abs(vec_y))
                            + (vec_x * vec_x + vec_y * vec_y);

                        distance1 = distance1.min(new_distance).max(distance0);
                        if new_distance < distance0 {
                            distance0 = new_distance;
                            closest_hash = hash;
                        }
                        y_primed = y_primed.wrapping_add(Self::PRIME_Y);
                    }
                    x_primed = x_primed.wrapping_add(Self::PRIME_X);
                }
            }
        }

        if self.cellular_distance_function == CellularDistanceFunction::Euclidean
            && self.cellular_return_type >= CellularReturnType::Distance
        {
            distance0 = FloatOps::sqrt(distance0);

            if self.cellular_return_type >= CellularReturnType::Distance2 {
                distance1 = FloatOps::sqrt(distance1);
            }
        }

        match self.cellular_return_type {
            CellularReturnType::CellValue => closest_hash as f32 * (1. / 2147483648.),
            CellularReturnType::Distance => distance0 - 1.,
            CellularReturnType::Distance2 => distance1 - 1.,
            CellularReturnType::Distance2Add => (distance1 + distance0) * 0.5 - 1.,
            CellularReturnType::Distance2Sub => distance1 - distance0 - 1.,
            CellularReturnType::Distance2Mul => distance1 * distance0 * 0.5 - 1.,
            CellularReturnType::Distance2Div => distance0 / distance1 - 1.,
        }
    }

    fn single_cellular_3d(&self, seed: i32, x: Float, y: Float, z: Float) -> f32 {
        let xr = Self::fast_round(x);
        let yr = Self::fast_round(y);
        let zr = Self::fast_round(z);

        let mut distance0 = f32::MAX;
        let mut distance1 = f32::MAX;
        let mut closest_hash = 0;

        let cellular_jitter = 0.39614353 * self.cellular_jitter_modifier;

        let mut x_primed = (xr - 1).wrapping_mul(Self::PRIME_X);
        let y_primed_base = (yr - 1).wrapping_mul(Self::PRIME_Y);
        let z_primed_base = (zr - 1).wrapping_mul(Self::PRIME_Z);

        match self.cellular_distance_function {
            CellularDistanceFunction::Euclidean | CellularDistanceFunction::EuclideanSq => {
                for xi in xr - 1..=xr + 1 {
                    let mut y_primed = y_primed_base;

                    for yi in yr - 1..=yr + 1 {
                        let mut z_primed = z_primed_base;

                        for zi in zr - 1..=zr + 1 {
                            let hash = Self::hash_3d(seed, x_primed, y_primed, z_primed);
                            let idx = hash & (255 << 2);

                            #[allow(clippy::unnecessary_cast)]
                            let vec_x = (xi as Float - x) as f32
                                + Self::RAND_VECS_3D[idx as usize] * cellular_jitter;
                            #[allow(clippy::unnecessary_cast)]
                            let vec_y = (yi as Float - y) as f32
                                + Self::RAND_VECS_3D[(idx | 1) as usize] * cellular_jitter;
                            #[allow(clippy::unnecessary_cast)]
                            let vec_z = (zi as Float - z) as f32
                                + Self::RAND_VECS_3D[(idx | 2) as usize] * cellular_jitter;

                            let new_distance = vec_x * vec_x + vec_y * vec_y + vec_z * vec_z;

                            distance1 = distance1.min(new_distance).max(distance0);
                            if new_distance < distance0 {
                                distance0 = new_distance;
                                closest_hash = hash;
                            }
                            z_primed = z_primed.wrapping_add(Self::PRIME_Z);
                        }
                        y_primed = y_primed.wrapping_add(Self::PRIME_Y);
                    }
                    x_primed = x_primed.wrapping_add(Self::PRIME_X);
                }
            }
            CellularDistanceFunction::Manhattan => {
                for xi in xr - 1..=xr + 1 {
                    let mut y_primed = y_primed_base;

                    for yi in yr - 1..=yr + 1 {
                        let mut z_primed = z_primed_base;

                        for zi in zr - 1..=zr + 1 {
                            let hash = Self::hash_3d(seed, x_primed, y_primed, z_primed);
                            let idx = hash & (255 << 2);

                            #[allow(clippy::unnecessary_cast)]
                            let vec_x = (xi as Float - x) as f32
                                + Self::RAND_VECS_3D[idx as usize] * cellular_jitter;
                            #[allow(clippy::unnecessary_cast)]
                            let vec_y = (yi as Float - y) as f32
                                + Self::RAND_VECS_3D[(idx | 1) as usize] * cellular_jitter;
                            #[allow(clippy::unnecessary_cast)]
                            let vec_z = (zi as Float - z) as f32
                                + Self::RAND_VECS_3D[(idx | 2) as usize] * cellular_jitter;

                            let new_distance =
                                FloatOps::abs(vec_x) + FloatOps::abs(vec_y) + FloatOps::abs(vec_z);

                            distance1 = distance1.min(new_distance).max(distance0);
                            if new_distance < distance0 {
                                distance0 = new_distance;
                                closest_hash = hash;
                            }
                            z_primed = z_primed.wrapping_add(Self::PRIME_Z);
                        }
                        y_primed = y_primed.wrapping_add(Self::PRIME_Y);
                    }
                    x_primed = x_primed.wrapping_add(Self::PRIME_X);
                }
            }
            CellularDistanceFunction::Hybrid => {
                for xi in xr - 1..=xr + 1 {
                    let mut y_primed = y_primed_base;

                    for yi in yr - 1..=yr + 1 {
                        let mut z_primed = z_primed_base;

                        for zi in zr - 1..=zr + 1 {
                            let hash = Self::hash_3d(seed, x_primed, y_primed, z_primed);
                            let idx = hash & (255 << 2);

                            #[allow(clippy::unnecessary_cast)]
                            let vec_x = (xi as Float - x) as f32
                                + Self::RAND_VECS_3D[idx as usize] * cellular_jitter;
                            #[allow(clippy::unnecessary_cast)]
                            let vec_y = (yi as Float - y) as f32
                                + Self::RAND_VECS_3D[(idx | 1) as usize] * cellular_jitter;
                            #[allow(clippy::unnecessary_cast)]
                            let vec_z = (zi as Float - z) as f32
                                + Self::RAND_VECS_3D[(idx | 2) as usize] * cellular_jitter;

                            let new_distance = (FloatOps::abs(vec_x)
                                + FloatOps::abs(vec_y)
                                + FloatOps::abs(vec_z))
                                + (vec_x * vec_x + vec_y * vec_y + vec_z * vec_z);

                            distance1 = distance1.min(new_distance).max(distance0);
                            if new_distance < distance0 {
                                distance0 = new_distance;
                                closest_hash = hash;
                            }
                            z_primed = z_primed.wrapping_add(Self::PRIME_Z);
                        }
                        y_primed = y_primed.wrapping_add(Self::PRIME_Y);
                    }
                    x_primed = x_primed.wrapping_add(Self::PRIME_X);
                }
            }
        }

        if self.cellular_distance_function == CellularDistanceFunction::Euclidean
            && self.cellular_return_type >= CellularReturnType::Distance
        {
            distance0 = FloatOps::sqrt(distance0);

            if self.cellular_return_type >= CellularReturnType::Distance2 {
                distance1 = FloatOps::sqrt(distance1);
            }
        }

        match self.cellular_return_type {
            CellularReturnType::CellValue => closest_hash as f32 * (1. / 2147483648.),
            CellularReturnType::Distance => distance0 - 1.,
            CellularReturnType::Distance2 => distance1 - 1.,
            CellularReturnType::Distance2Add => (distance1 + distance0) * 0.5 - 1.,
            CellularReturnType::Distance2Sub => distance1 - distance0 - 1.,
            CellularReturnType::Distance2Mul => distance1 * distance0 * 0.5 - 1.,
            CellularReturnType::Distance2Div => distance0 / distance1 - 1.,
        }
    }

    // Perlin Noise

    fn single_perlin_2d(&self, seed: i32, x: Float, y: Float) -> f32 {
        let x0 = Self::fast_floor(x);
        let y0 = Self::fast_floor(y);

        #[allow(clippy::unnecessary_cast)]
        let xd0 = (x - x0 as Float) as f32;
        #[allow(clippy::unnecessary_cast)]
        let yd0 = (y - y0 as Float) as f32;
        let xd1 = xd0 - 1.;
        let yd1 = yd0 - 1.;

        let xs = Self::interp_quintic(xd0);
        let ys = Self::interp_quintic(yd0);

        let x0 = x0.wrapping_mul(Self::PRIME_X);
        let y0 = y0.wrapping_mul(Self::PRIME_Y);
        let x1 = x0.wrapping_add(Self::PRIME_X);
        let y1 = y0.wrapping_add(Self::PRIME_Y);

        let xf0 = Self::lerp(
            Self::grad_coord_2d(seed, x0, y0, xd0, yd0),
            Self::grad_coord_2d(seed, x1, y0, xd1, yd0),
            xs,
        );
        let xf1 = Self::lerp(
            Self::grad_coord_2d(seed, x0, y1, xd0, yd1),
            Self::grad_coord_2d(seed, x1, y1, xd1, yd1),
            xs,
        );

        Self::lerp(xf0, xf1, ys) * 1.4247691104677813
    }

    fn single_perlin_3d(&self, seed: i32, x: Float, y: Float, z: Float) -> f32 {
        let x0 = Self::fast_floor(x);
        let y0 = Self::fast_floor(y);
        let z0 = Self::fast_floor(z);

        #[allow(clippy::unnecessary_cast)]
        let xd0 = (x - x0 as Float) as f32;
        #[allow(clippy::unnecessary_cast)]
        let yd0 = (y - y0 as Float) as f32;
        #[allow(clippy::unnecessary_cast)]
        let zd0 = (z - z0 as Float) as f32;
        let xd1 = xd0 - 1.;
        let yd1 = yd0 - 1.;
        let zd1 = zd0 - 1.;

        let xs = Self::interp_quintic(xd0);
        let ys = Self::interp_quintic(yd0);
        let zs = Self::interp_quintic(zd0);

        let x0 = x0.wrapping_mul(Self::PRIME_X);
        let y0 = y0.wrapping_mul(Self::PRIME_Y);
        let z0 = z0.wrapping_mul(Self::PRIME_Z);
        let x1 = x0.wrapping_add(Self::PRIME_X);
        let y1 = y0.wrapping_add(Self::PRIME_Y);
        let z1 = z0.wrapping_add(Self::PRIME_Z);

        let xf00 = Self::lerp(
            Self::grad_coord_3d(seed, x0, y0, z0, xd0, yd0, zd0),
            Self::grad_coord_3d(seed, x1, y0, z0, xd1, yd0, zd0),
            xs,
        );
        let xf10 = Self::lerp(
            Self::grad_coord_3d(seed, x0, y1, z0, xd0, yd1, zd0),
            Self::grad_coord_3d(seed, x1, y1, z0, xd1, yd1, zd0),
            xs,
        );
        let xf01 = Self::lerp(
            Self::grad_coord_3d(seed, x0, y0, z1, xd0, yd0, zd1),
            Self::grad_coord_3d(seed, x1, y0, z1, xd1, yd0, zd1),
            xs,
        );
        let xf11 = Self::lerp(
            Self::grad_coord_3d(seed, x0, y1, z1, xd0, yd1, zd1),
            Self::grad_coord_3d(seed, x1, y1, z1, xd1, yd1, zd1),
            xs,
        );

        let yf0 = Self::lerp(xf00, xf10, ys);
        let yf1 = Self::lerp(xf01, xf11, ys);

        Self::lerp(yf0, yf1, zs) * 0.964921414852142333984375
    }

    // Value Cubic Noise

    fn single_value_cubic_2d(&self, seed: i32, x: Float, y: Float) -> f32 {
        let x1 = Self::fast_floor(x);
        let y1 = Self::fast_floor(y);

        #[allow(clippy::unnecessary_cast)]
        let xs = (x - x1 as Float) as f32;
        #[allow(clippy::unnecessary_cast)]
        let ys = (y - y1 as Float) as f32;

        let x1 = x1.wrapping_mul(Self::PRIME_X);
        let y1 = y1.wrapping_mul(Self::PRIME_Y);
        let x0 = x1.wrapping_sub(Self::PRIME_X);
        let y0 = y1.wrapping_sub(Self::PRIME_Y);
        let x2 = x1.wrapping_add(Self::PRIME_X);
        let y2 = y1.wrapping_add(Self::PRIME_Y);
        let x3 = x1.wrapping_add(Self::PRIME_X_2);
        let y3 = y1.wrapping_add(Self::PRIME_Y_2);

        Self::cubic_lerp(
            Self::cubic_lerp(
                Self::val_coord_2d(seed, x0, y0),
                Self::val_coord_2d(seed, x1, y0),
                Self::val_coord_2d(seed, x2, y0),
                Self::val_coord_2d(seed, x3, y0),
                xs,
            ),
            Self::cubic_lerp(
                Self::val_coord_2d(seed, x0, y1),
                Self::val_coord_2d(seed, x1, y1),
                Self::val_coord_2d(seed, x2, y1),
                Self::val_coord_2d(seed, x3, y1),
                xs,
            ),
            Self::cubic_lerp(
                Self::val_coord_2d(seed, x0, y2),
                Self::val_coord_2d(seed, x1, y2),
                Self::val_coord_2d(seed, x2, y2),
                Self::val_coord_2d(seed, x3, y2),
                xs,
            ),
            Self::cubic_lerp(
                Self::val_coord_2d(seed, x0, y3),
                Self::val_coord_2d(seed, x1, y3),
                Self::val_coord_2d(seed, x2, y3),
                Self::val_coord_2d(seed, x3, y3),
                xs,
            ),
            ys,
        ) * (1. / (1.5 * 1.5))
    }

    fn single_value_cubic_3d(&self, seed: i32, x: Float, y: Float, z: Float) -> f32 {
        let x1 = Self::fast_floor(x);
        let y1 = Self::fast_floor(y);
        let z1 = Self::fast_floor(z);

        #[allow(clippy::unnecessary_cast)]
        let xs = (x - x1 as Float) as f32;
        #[allow(clippy::unnecessary_cast)]
        let ys = (y - y1 as Float) as f32;
        #[allow(clippy::unnecessary_cast)]
        let zs = (z - z1 as Float) as f32;

        let x1 = x1.wrapping_mul(Self::PRIME_X);
        let y1 = y1.wrapping_mul(Self::PRIME_Y);
        let z1 = z1.wrapping_mul(Self::PRIME_Z);

        let x0 = x1.wrapping_sub(Self::PRIME_X);
        let y0 = y1.wrapping_sub(Self::PRIME_Y);
        let z0 = z1.wrapping_sub(Self::PRIME_Z);
        let x2 = x1.wrapping_add(Self::PRIME_X);
        let y2 = y1.wrapping_add(Self::PRIME_Y);
        let z2 = z1.wrapping_add(Self::PRIME_Z);
        let x3 = x1.wrapping_add(Self::PRIME_X_2);
        let y3 = y1.wrapping_add(Self::PRIME_Y_2);
        let z3 = z1.wrapping_add(Self::PRIME_Z_2);

        Self::cubic_lerp(
            Self::cubic_lerp(
                Self::cubic_lerp(
                    Self::val_coord_3d(seed, x0, y0, z0),
                    Self::val_coord_3d(seed, x1, y0, z0),
                    Self::val_coord_3d(seed, x2, y0, z0),
                    Self::val_coord_3d(seed, x3, y0, z0),
                    xs,
                ),
                Self::cubic_lerp(
                    Self::val_coord_3d(seed, x0, y1, z0),
                    Self::val_coord_3d(seed, x1, y1, z0),
                    Self::val_coord_3d(seed, x2, y1, z0),
                    Self::val_coord_3d(seed, x3, y1, z0),
                    xs,
                ),
                Self::cubic_lerp(
                    Self::val_coord_3d(seed, x0, y2, z0),
                    Self::val_coord_3d(seed, x1, y2, z0),
                    Self::val_coord_3d(seed, x2, y2, z0),
                    Self::val_coord_3d(seed, x3, y2, z0),
                    xs,
                ),
                Self::cubic_lerp(
                    Self::val_coord_3d(seed, x0, y3, z0),
                    Self::val_coord_3d(seed, x1, y3, z0),
                    Self::val_coord_3d(seed, x2, y3, z0),
                    Self::val_coord_3d(seed, x3, y3, z0),
                    xs,
                ),
                ys,
            ),
            Self::cubic_lerp(
                Self::cubic_lerp(
                    Self::val_coord_3d(seed, x0, y0, z1),
                    Self::val_coord_3d(seed, x1, y0, z1),
                    Self::val_coord_3d(seed, x2, y0, z1),
                    Self::val_coord_3d(seed, x3, y0, z1),
                    xs,
                ),
                Self::cubic_lerp(
                    Self::val_coord_3d(seed, x0, y1, z1),
                    Self::val_coord_3d(seed, x1, y1, z1),
                    Self::val_coord_3d(seed, x2, y1, z1),
                    Self::val_coord_3d(seed, x3, y1, z1),
                    xs,
                ),
                Self::cubic_lerp(
                    Self::val_coord_3d(seed, x0, y2, z1),
                    Self::val_coord_3d(seed, x1, y2, z1),
                    Self::val_coord_3d(seed, x2, y2, z1),
                    Self::val_coord_3d(seed, x3, y2, z1),
                    xs,
                ),
                Self::cubic_lerp(
                    Self::val_coord_3d(seed, x0, y3, z1),
                    Self::val_coord_3d(seed, x1, y3, z1),
                    Self::val_coord_3d(seed, x2, y3, z1),
                    Self::val_coord_3d(seed, x3, y3, z1),
                    xs,
                ),
                ys,
            ),
            Self::cubic_lerp(
                Self::cubic_lerp(
                    Self::val_coord_3d(seed, x0, y0, z2),
                    Self::val_coord_3d(seed, x1, y0, z2),
                    Self::val_coord_3d(seed, x2, y0, z2),
                    Self::val_coord_3d(seed, x3, y0, z2),
                    xs,
                ),
                Self::cubic_lerp(
                    Self::val_coord_3d(seed, x0, y1, z2),
                    Self::val_coord_3d(seed, x1, y1, z2),
                    Self::val_coord_3d(seed, x2, y1, z2),
                    Self::val_coord_3d(seed, x3, y1, z2),
                    xs,
                ),
                Self::cubic_lerp(
                    Self::val_coord_3d(seed, x0, y2, z2),
                    Self::val_coord_3d(seed, x1, y2, z2),
                    Self::val_coord_3d(seed, x2, y2, z2),
                    Self::val_coord_3d(seed, x3, y2, z2),
                    xs,
                ),
                Self::cubic_lerp(
                    Self::val_coord_3d(seed, x0, y3, z2),
                    Self::val_coord_3d(seed, x1, y3, z2),
                    Self::val_coord_3d(seed, x2, y3, z2),
                    Self::val_coord_3d(seed, x3, y3, z2),
                    xs,
                ),
                ys,
            ),
            Self::cubic_lerp(
                Self::cubic_lerp(
                    Self::val_coord_3d(seed, x0, y0, z3),
                    Self::val_coord_3d(seed, x1, y0, z3),
                    Self::val_coord_3d(seed, x2, y0, z3),
                    Self::val_coord_3d(seed, x3, y0, z3),
                    xs,
                ),
                Self::cubic_lerp(
                    Self::val_coord_3d(seed, x0, y1, z3),
                    Self::val_coord_3d(seed, x1, y1, z3),
                    Self::val_coord_3d(seed, x2, y1, z3),
                    Self::val_coord_3d(seed, x3, y1, z3),
                    xs,
                ),
                Self::cubic_lerp(
                    Self::val_coord_3d(seed, x0, y2, z3),
                    Self::val_coord_3d(seed, x1, y2, z3),
                    Self::val_coord_3d(seed, x2, y2, z3),
                    Self::val_coord_3d(seed, x3, y2, z3),
                    xs,
                ),
                Self::cubic_lerp(
                    Self::val_coord_3d(seed, x0, y3, z3),
                    Self::val_coord_3d(seed, x1, y3, z3),
                    Self::val_coord_3d(seed, x2, y3, z3),
                    Self::val_coord_3d(seed, x3, y3, z3),
                    xs,
                ),
                ys,
            ),
            zs,
        ) * (1. / (1.5 * 1.5 * 1.5))
    }

    // Value Noise

    fn single_value_2d(&self, seed: i32, x: Float, y: Float) -> f32 {
        let x0 = Self::fast_floor(x);
        let y0 = Self::fast_floor(y);

        #[allow(clippy::unnecessary_cast)]
        let xs = Self::interp_hermite((x - x0 as Float) as f32);
        #[allow(clippy::unnecessary_cast)]
        let ys = Self::interp_hermite((y - y0 as Float) as f32);

        let x0 = x0.wrapping_mul(Self::PRIME_X);
        let y0 = y0.wrapping_mul(Self::PRIME_Y);
        let x1 = x0.wrapping_add(Self::PRIME_X);
        let y1 = y0.wrapping_add(Self::PRIME_Y);

        let xf0 = Self::lerp(
            Self::val_coord_2d(seed, x0, y0),
            Self::val_coord_2d(seed, x1, y0),
            xs,
        );
        let xf1 = Self::lerp(
            Self::val_coord_2d(seed, x0, y1),
            Self::val_coord_2d(seed, x1, y1),
            xs,
        );

        Self::lerp(xf0, xf1, ys)
    }

    fn single_value_3d(&self, seed: i32, x: Float, y: Float, z: Float) -> f32 {
        let x0 = Self::fast_floor(x);
        let y0 = Self::fast_floor(y);
        let z0 = Self::fast_floor(z);

        #[allow(clippy::unnecessary_cast)]
        let xs = Self::interp_hermite((x - x0 as Float) as f32);
        #[allow(clippy::unnecessary_cast)]
        let ys = Self::interp_hermite((y - y0 as Float) as f32);
        #[allow(clippy::unnecessary_cast)]
        let zs = Self::interp_hermite((z - z0 as Float) as f32);

        let x0 = x0.wrapping_mul(Self::PRIME_X);
        let y0 = y0.wrapping_mul(Self::PRIME_Y);
        let z0 = z0.wrapping_mul(Self::PRIME_Z);
        let x1 = x0.wrapping_add(Self::PRIME_X);
        let y1 = y0.wrapping_add(Self::PRIME_Y);
        let z1 = z0.wrapping_add(Self::PRIME_Z);

        let xf00 = Self::lerp(
            Self::val_coord_3d(seed, x0, y0, z0),
            Self::val_coord_3d(seed, x1, y0, z0),
            xs,
        );
        let xf10 = Self::lerp(
            Self::val_coord_3d(seed, x0, y1, z0),
            Self::val_coord_3d(seed, x1, y1, z0),
            xs,
        );
        let xf01 = Self::lerp(
            Self::val_coord_3d(seed, x0, y0, z1),
            Self::val_coord_3d(seed, x1, y0, z1),
            xs,
        );
        let xf11 = Self::lerp(
            Self::val_coord_3d(seed, x0, y1, z1),
            Self::val_coord_3d(seed, x1, y1, z1),
            xs,
        );

        let yf0 = Self::lerp(xf00, xf10, ys);
        let yf1 = Self::lerp(xf01, xf11, ys);

        Self::lerp(yf0, yf1, zs)
    }

    // Domain Warp

    #[allow(clippy::too_many_arguments)]
    fn do_single_domain_warp_2d(
        &self,
        seed: i32,
        amp: f32,
        freq: f32,
        x: Float,
        y: Float,
        xr: Float,
        yr: Float,
    ) -> (Float, Float) {
        match self.domain_warp_type {
            DomainWarpType::OpenSimplex2 => self.single_domain_warp_simplex_gradient_2d(
                seed,
                amp * 38.283687591552734375,
                freq,
                x,
                y,
                xr,
                yr,
                false,
            ),
            DomainWarpType::OpenSimplex2Reduced => self.single_domain_warp_simplex_gradient_2d(
                seed,
                amp * 16.,
                freq,
                x,
                y,
                xr,
                yr,
                true,
            ),
            DomainWarpType::BasicGrid => {
                self.single_domain_warp_basic_grid_2d(seed, amp, freq, x, y, xr, yr)
            }
        }
    }

    #[allow(clippy::too_many_arguments)]
    fn do_single_domain_warp_3d(
        &self,
        seed: i32,
        amp: f32,
        freq: f32,
        x: Float,
        y: Float,
        z: Float,
        xr: Float,
        yr: Float,
        zr: Float,
    ) -> (Float, Float, Float) {
        match self.domain_warp_type {
            DomainWarpType::OpenSimplex2 => self.single_domain_warp_open_simplex_2_gradient(
                seed,
                amp * 32.69428253173828125,
                freq,
                x,
                y,
                z,
                xr,
                yr,
                zr,
                false,
            ),
            DomainWarpType::OpenSimplex2Reduced => self.single_domain_warp_open_simplex_2_gradient(
                seed,
                amp * 7.71604938271605,
                freq,
                x,
                y,
                z,
                xr,
                yr,
                zr,
                true,
            ),
            DomainWarpType::BasicGrid => {
                self.single_domain_warp_basic_grid_3d(seed, amp, freq, x, y, z, xr, yr, zr)
            }
        }
    }

    // Domain Warp Single Wrapper

    fn domain_warp_single_2d(&self, x: Float, y: Float) -> (Float, Float) {
        let seed = self.seed;
        let amp = self.domain_warp_amp * self.fractal_bounding;
        let freq = self.frequency;

        let (xs, ys) = self.transform_domain_warp_coordinate_2d(x, y);

        self.do_single_domain_warp_2d(seed, amp, freq, xs, ys, x, y)
    }

    fn domain_warp_single_3d(&self, x: Float, y: Float, z: Float) -> (Float, Float, Float) {
        let seed = self.seed;
        let amp = self.domain_warp_amp * self.fractal_bounding;
        let freq = self.frequency;

        let (xs, ys, zs) = self.transform_domain_warp_coordinate_3d(x, y, z);

        self.do_single_domain_warp_3d(seed, amp, freq, xs, ys, zs, x, y, z)
    }

    // Domain Warp Fractal Progressive

    fn domain_warp_fractal_progressive_2d(&self, x: Float, y: Float) -> (Float, Float) {
        let mut x = x;
        let mut y = y;

        let mut seed = self.seed;
        let mut amp = self.domain_warp_amp * self.fractal_bounding;
        let mut freq = self.frequency;

        for _ in 0..self.octaves {
            let (xs, ys) = self.transform_domain_warp_coordinate_2d(x, y);

            (x, y) = self.do_single_domain_warp_2d(seed, amp, freq, xs, ys, x, y);

            seed += 1;
            amp *= self.gain;
            freq *= self.lacunarity;
        }

        (x, y)
    }

    fn domain_warp_fractal_progressive_3d(
        &self,
        x: Float,
        y: Float,
        z: Float,
    ) -> (Float, Float, Float) {
        let mut x = x;
        let mut y = y;
        let mut z = z;

        let mut seed = self.seed;
        let mut amp = self.domain_warp_amp * self.fractal_bounding;
        let mut freq = self.frequency;

        for _ in 0..self.octaves {
            let (xs, ys, zs) = self.transform_domain_warp_coordinate_3d(x, y, z);

            (x, y, z) = self.do_single_domain_warp_3d(seed, amp, freq, xs, ys, zs, x, y, z);

            seed += 1;
            amp *= self.gain;
            freq *= self.lacunarity;
        }

        (x, y, z)
    }

    // Domain Warp Fractal Independant

    fn domain_warp_fractal_independent_2d(&self, x: Float, y: Float) -> (Float, Float) {
        let mut x = x;
        let mut y = y;

        let (xs, ys) = self.transform_domain_warp_coordinate_2d(x, y);

        let mut seed = self.seed;
        let mut amp = self.domain_warp_amp * self.fractal_bounding;
        let mut freq = self.frequency;

        for _ in 0..self.octaves {
            (x, y) = self.do_single_domain_warp_2d(seed, amp, freq, xs, ys, x, y);

            seed += 1;
            amp *= self.gain;
            freq *= self.lacunarity;
        }

        (x, y)
    }

    fn domain_warp_fractal_independent_3d(
        &self,
        x: Float,
        y: Float,
        z: Float,
    ) -> (Float, Float, Float) {
        let mut x = x;
        let mut y = y;
        let mut z = z;

        let (xs, ys, zs) = self.transform_domain_warp_coordinate_3d(x, y, z);

        let mut seed = self.seed;
        let mut amp = self.domain_warp_amp * self.fractal_bounding;
        let mut freq = self.frequency;

        for _ in 0..self.octaves {
            (x, y, z) = self.do_single_domain_warp_3d(seed, amp, freq, xs, ys, zs, x, y, z);

            seed += 1;
            amp *= self.gain;
            freq *= self.lacunarity;
        }

        (x, y, z)
    }

    // Domain Warp Basic Grid

    #[allow(clippy::too_many_arguments)]
    fn single_domain_warp_basic_grid_2d(
        &self,
        seed: i32,
        warp_amp: f32,
        frequency: f32,
        x: Float,
        y: Float,
        xr: Float,
        yr: Float,
    ) -> (Float, Float) {
        let xf = x * frequency as Float;
        let yf = y * frequency as Float;

        let x0 = Self::fast_floor(xf);
        let y0 = Self::fast_floor(yf);

        #[allow(clippy::unnecessary_cast)]
        let xs = Self::interp_hermite((xf - x0 as Float) as f32);
        #[allow(clippy::unnecessary_cast)]
        let ys = Self::interp_hermite((yf - y0 as Float) as f32);

        let x0 = x0.wrapping_mul(Self::PRIME_X);
        let y0 = y0.wrapping_mul(Self::PRIME_Y);
        let x1 = x0.wrapping_add(Self::PRIME_X);
        let y1 = y0.wrapping_add(Self::PRIME_Y);

        let hash0 = Self::hash_2d(seed, x0, y0) & (255 << 1);
        let hash1 = Self::hash_2d(seed, x1, y0) & (255 << 1);

        let lx0x = Self::lerp(
            Self::RAND_VECS_2D[hash0 as usize],
            Self::RAND_VECS_2D[hash1 as usize],
            xs,
        );
        let ly0x = Self::lerp(
            Self::RAND_VECS_2D[(hash0 | 1) as usize],
            Self::RAND_VECS_2D[(hash1 | 1) as usize],
            xs,
        );

        let hash0 = Self::hash_2d(seed, x0, y1) & (255 << 1);
        let hash1 = Self::hash_2d(seed, x1, y1) & (255 << 1);

        let lx1x = Self::lerp(
            Self::RAND_VECS_2D[hash0 as usize],
            Self::RAND_VECS_2D[hash1 as usize],
            xs,
        );
        let ly1x = Self::lerp(
            Self::RAND_VECS_2D[(hash0 | 1) as usize],
            Self::RAND_VECS_2D[(hash1 | 1) as usize],
            xs,
        );

        let xr = xr + (Self::lerp(lx0x, lx1x, ys) * warp_amp) as Float;
        let yr = yr + (Self::lerp(ly0x, ly1x, ys) * warp_amp) as Float;

        (xr, yr)
    }

    #[allow(clippy::too_many_arguments)]
    fn single_domain_warp_basic_grid_3d(
        &self,
        seed: i32,
        warp_amp: f32,
        frequency: f32,
        x: Float,
        y: Float,
        z: Float,
        xr: Float,
        yr: Float,
        zr: Float,
    ) -> (Float, Float, Float) {
        let xf = x * frequency as Float;
        let yf = y * frequency as Float;
        let zf = z * frequency as Float;

        let x0 = Self::fast_floor(xf);
        let y0 = Self::fast_floor(yf);
        let z0 = Self::fast_floor(zf);

        #[allow(clippy::unnecessary_cast)]
        let xs = Self::interp_hermite((xf - x0 as Float) as f32);
        #[allow(clippy::unnecessary_cast)]
        let ys = Self::interp_hermite((yf - y0 as Float) as f32);
        #[allow(clippy::unnecessary_cast)]
        let zs = Self::interp_hermite((zf - z0 as Float) as f32);

        let x0 = x0.wrapping_mul(Self::PRIME_X);
        let y0 = y0.wrapping_mul(Self::PRIME_Y);
        let z0 = z0.wrapping_mul(Self::PRIME_Z);
        let x1 = x0.wrapping_add(Self::PRIME_X);
        let y1 = y0.wrapping_add(Self::PRIME_Y);
        let z1 = z0.wrapping_add(Self::PRIME_Z);

        let hash0 = Self::hash_3d(seed, x0, y0, z0) & (255 << 2);
        let hash1 = Self::hash_3d(seed, x1, y0, z0) & (255 << 2);

        let lx0x = Self::lerp(
            Self::RAND_VECS_3D[hash0 as usize],
            Self::RAND_VECS_3D[hash1 as usize],
            xs,
        );
        let ly0x = Self::lerp(
            Self::RAND_VECS_3D[(hash0 | 1) as usize],
            Self::RAND_VECS_3D[(hash1 | 1) as usize],
            xs,
        );
        let lz0x = Self::lerp(
            Self::RAND_VECS_3D[(hash0 | 2) as usize],
            Self::RAND_VECS_3D[(hash1 | 2) as usize],
            xs,
        );

        let hash0 = Self::hash_3d(seed, x0, y1, z0) & (255 << 2);
        let hash1 = Self::hash_3d(seed, x1, y1, z0) & (255 << 2);

        let lx1x = Self::lerp(
            Self::RAND_VECS_3D[hash0 as usize],
            Self::RAND_VECS_3D[hash1 as usize],
            xs,
        );
        let ly1x = Self::lerp(
            Self::RAND_VECS_3D[(hash0 | 1) as usize],
            Self::RAND_VECS_3D[(hash1 | 1) as usize],
            xs,
        );
        let lz1x = Self::lerp(
            Self::RAND_VECS_3D[(hash0 | 2) as usize],
            Self::RAND_VECS_3D[(hash1 | 2) as usize],
            xs,
        );

        let lx0y = Self::lerp(lx0x, lx1x, ys);
        let ly0y = Self::lerp(ly0x, ly1x, ys);
        let lz0y = Self::lerp(lz0x, lz1x, ys);

        let hash0 = Self::hash_3d(seed, x0, y0, z1) & (255 << 2);
        let hash1 = Self::hash_3d(seed, x1, y0, z1) & (255 << 2);

        let lx0x = Self::lerp(
            Self::RAND_VECS_3D[hash0 as usize],
            Self::RAND_VECS_3D[hash1 as usize],
            xs,
        );
        let ly0x = Self::lerp(
            Self::RAND_VECS_3D[(hash0 | 1) as usize],
            Self::RAND_VECS_3D[(hash1 | 1) as usize],
            xs,
        );
        let lz0x = Self::lerp(
            Self::RAND_VECS_3D[(hash0 | 2) as usize],
            Self::RAND_VECS_3D[(hash1 | 2) as usize],
            xs,
        );

        let hash0 = Self::hash_3d(seed, x0, y1, z1) & (255 << 2);
        let hash1 = Self::hash_3d(seed, x1, y1, z1) & (255 << 2);

        let lx1x = Self::lerp(
            Self::RAND_VECS_3D[hash0 as usize],
            Self::RAND_VECS_3D[hash1 as usize],
            xs,
        );
        let ly1x = Self::lerp(
            Self::RAND_VECS_3D[(hash0 | 1) as usize],
            Self::RAND_VECS_3D[(hash1 | 1) as usize],
            xs,
        );
        let lz1x = Self::lerp(
            Self::RAND_VECS_3D[(hash0 | 2) as usize],
            Self::RAND_VECS_3D[(hash1 | 2) as usize],
            xs,
        );

        let xr = xr + (Self::lerp(lx0y, Self::lerp(lx0x, lx1x, ys), zs) * warp_amp) as Float;
        let yr = yr + (Self::lerp(ly0y, Self::lerp(ly0x, ly1x, ys), zs) * warp_amp) as Float;
        let zr = zr + (Self::lerp(lz0y, Self::lerp(lz0x, lz1x, ys), zs) * warp_amp) as Float;

        (xr, yr, zr)
    }

    // Domain Warp Simplex/OpenSimplex2
    #[allow(clippy::too_many_arguments)]
    fn single_domain_warp_simplex_gradient_2d(
        &self,
        seed: i32,
        warp_amp: f32,
        frequency: f32,
        x: Float,
        y: Float,
        xr: Float,
        yr: Float,
        out_frad_only: bool,
    ) -> (Float, Float) {
        const SQRT3: f32 = 1.7320508075688772935274463415059;
        const G2: f32 = (3. - SQRT3) / 6.;

        let x = x * frequency as Float;
        let y = y * frequency as Float;

        /*
         * --- Skew moved to TransformNoiseCoordinateXY method ---
         * let f2 = 0.5 * (sqrt3 - 1);
         * let s = (x + y) * f2;
         * x += s; y += s;
         */

        let i = Self::fast_floor(x);
        let j = Self::fast_floor(y);
        #[allow(clippy::unnecessary_cast)]
        let xi = (x - i as Float) as f32;
        #[allow(clippy::unnecessary_cast)]
        let yi = (y - j as Float) as f32;

        let t = (xi + yi) * G2;
        let x0 = xi - t;
        let y0 = yi - t;

        let i = i.wrapping_mul(Self::PRIME_X);
        let j = j.wrapping_mul(Self::PRIME_Y);

        let mut vx = 0.;
        let mut vy = 0.;

        let a = 0.5 - x0 * x0 - y0 * y0;
        if a > 0. {
            let aaaa = (a * a) * (a * a);
            let (xo, yo) = if out_frad_only {
                Self::grad_coord_out_2d(seed, i, j)
            } else {
                Self::grad_coord_dual_2d(seed, i, j, x0, y0)
            };
            vx += aaaa * xo;
            vy += aaaa * yo;
        }

        let c = (2. * (1. - 2. * G2) * (1. / G2 - 2.)) * t
            + ((-2. * (1. - 2. * G2) * (1. - 2. * G2)) + a);
        if c > 0. {
            let x2 = x0 + (2. * G2 - 1.);
            let y2 = y0 + (2. * G2 - 1.);
            let cccc = (c * c) * (c * c);
            let (xo, yo) = if out_frad_only {
                Self::grad_coord_out_2d(
                    seed,
                    i.wrapping_add(Self::PRIME_X),
                    j.wrapping_add(Self::PRIME_Y),
                )
            } else {
                Self::grad_coord_dual_2d(
                    seed,
                    i.wrapping_add(Self::PRIME_X),
                    j.wrapping_add(Self::PRIME_Y),
                    x2,
                    y2,
                )
            };
            vx += cccc * xo;
            vy += cccc * yo;
        }

        if y0 > x0 {
            let x1 = x0 + G2;
            let y1 = y0 + (G2 - 1.);
            let b = 0.5 - x1 * x1 - y1 * y1;
            if b > 0. {
                let bbbb = (b * b) * (b * b);
                let (xo, yo) = if out_frad_only {
                    Self::grad_coord_out_2d(seed, i, j.wrapping_add(Self::PRIME_Y))
                } else {
                    Self::grad_coord_dual_2d(seed, i, j.wrapping_add(Self::PRIME_Y), x1, y1)
                };
                vx += bbbb * xo;
                vy += bbbb * yo;
            }
        } else {
            let x1 = x0 + (G2 - 1.);
            let y1 = y0 + G2;
            let b = 0.5 - x1 * x1 - y1 * y1;
            if b > 0. {
                let bbbb = (b * b) * (b * b);
                let (xo, yo) = if out_frad_only {
                    Self::grad_coord_out_2d(seed, i.wrapping_add(Self::PRIME_X), j)
                } else {
                    Self::grad_coord_dual_2d(seed, i.wrapping_add(Self::PRIME_X), j, x1, y1)
                };
                vx += bbbb * xo;
                vy += bbbb * yo;
            }
        }

        let xr = xr + (vx * warp_amp) as Float;
        let yr = yr + (vy * warp_amp) as Float;

        (xr, yr)
    }

    #[allow(clippy::too_many_arguments)]
    fn single_domain_warp_open_simplex_2_gradient(
        &self,
        seed: i32,
        warp_amp: f32,
        frequency: f32,
        x: Float,
        y: Float,
        z: Float,
        xr: Float,
        yr: Float,
        zr: Float,
        out_grad_only: bool,
    ) -> (Float, Float, Float) {
        let mut seed = seed;

        let x = x * frequency as Float;
        let y = y * frequency as Float;
        let z = z * frequency as Float;

        /*
         * --- Rotation moved to TransformDomainWarpCoordinate method ---
         * let r3 = 2. / 3.;
         * let r = (x + y + z) * r3; // Rotation, not skew
         * x = r - x; y = r - y; z = r - z;
         */

        let i = Self::fast_round(x);
        let j = Self::fast_round(y);
        let k = Self::fast_round(z);
        #[allow(clippy::unnecessary_cast)]
        let mut x0 = (x - i as Float) as f32;
        #[allow(clippy::unnecessary_cast)]
        let mut y0 = (y - j as Float) as f32;
        #[allow(clippy::unnecessary_cast)]
        let mut z0 = (z - k as Float) as f32;

        let mut x_n_sign = (-x0 - 1.) as i32 | 1;
        let mut y_n_sign = (-y0 - 1.) as i32 | 1;
        let mut z_n_sign = (-z0 - 1.) as i32 | 1;

        let mut ax0 = x_n_sign as f32 * -x0;
        let mut ay0 = y_n_sign as f32 * -y0;
        let mut az0 = z_n_sign as f32 * -z0;

        let mut i = i.wrapping_mul(Self::PRIME_X);
        let mut j = j.wrapping_mul(Self::PRIME_Y);
        let mut k = k.wrapping_mul(Self::PRIME_Z);

        let mut vx = 0.;
        let mut vy = 0.;
        let mut vz = 0.;

        let mut a = (0.6 - x0 * x0) - (y0 * y0 + z0 * z0);
        let mut l = 0;
        loop {
            if a > 0. {
                let aaaa = (a * a) * (a * a);
                let (xo, yo, zo) = if out_grad_only {
                    Self::grad_coord_out_3d(seed, i, j, k)
                } else {
                    Self::grad_coord_dual_3d(seed, i, j, k, x0, y0, z0)
                };
                vx += aaaa * xo;
                vy += aaaa * yo;
                vz += aaaa * zo;
            }

            let mut b = a;
            let mut i1 = i;
            let mut j1 = j;
            let mut k1 = k;
            let mut x1 = x0;
            let mut y1 = y0;
            let mut z1 = z0;

            if ax0 >= ay0 && ax0 >= az0 {
                x1 += x_n_sign as f32;
                b = b + ax0 + ax0;
                i1 = i1.wrapping_sub(x_n_sign.wrapping_mul(Self::PRIME_X));
            } else if ay0 > ax0 && ay0 >= az0 {
                y1 += y_n_sign as f32;
                b = b + ay0 + ay0;
                j1 = j1.wrapping_sub(y_n_sign.wrapping_mul(Self::PRIME_Y));
            } else {
                z1 += z_n_sign as f32;
                b = b + az0 + az0;
                k1 = k1.wrapping_sub(z_n_sign.wrapping_mul(Self::PRIME_Z));
            }

            if b > 1. {
                b -= 1.;
                let bbbb = (b * b) * (b * b);
                let (xo, yo, zo) = if out_grad_only {
                    Self::grad_coord_out_3d(seed, i1, j1, k1)
                } else {
                    Self::grad_coord_dual_3d(seed, i1, j1, k1, x1, y1, z1)
                };
                vx += bbbb * xo;
                vy += bbbb * yo;
                vz += bbbb * zo;
            }

            if l == 1 {
                break;
            }

            ax0 = 0.5 - ax0;
            ay0 = 0.5 - ay0;
            az0 = 0.5 - az0;

            x0 = x_n_sign as f32 * ax0;
            y0 = y_n_sign as f32 * ay0;
            z0 = z_n_sign as f32 * az0;

            a += (0.75 - ax0) - (ay0 + az0);

            i = i.wrapping_add((x_n_sign >> 1) & Self::PRIME_X);
            j = j.wrapping_add((y_n_sign >> 1) & Self::PRIME_Y);
            k = k.wrapping_add((z_n_sign >> 1) & Self::PRIME_Z);

            x_n_sign = -x_n_sign;
            y_n_sign = -y_n_sign;
            z_n_sign = -z_n_sign;

            seed += 1293373;

            l += 1;
        }

        let xr = xr + (vx * warp_amp) as Float;
        let yr = yr + (vy * warp_amp) as Float;
        let zr = zr + (vz * warp_amp) as Float;

        (xr, yr, zr)
    }
}

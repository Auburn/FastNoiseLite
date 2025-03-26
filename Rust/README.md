[![GitHub](https://img.shields.io/github/v/release/Auburn/FastNoiseLite?logo=github&label=GitHub&color=blue
 "GitHub")](https://github.com/Auburn/FastNoiseLite) [![crates.io](https://img.shields.io/crates/v/fastnoise-lite?logo=rust&color=blue "crates.io")](https://crates.io/crates/fastnoise-lite) [![docs.rs](https://img.shields.io/docsrs/fastnoise-lite/latest?logo=docs.rs&label=docs.rs&color=blue "docs.rs")](https://docs.rs/fastnoise-lite/latest/fastnoise_lite/)

# FastNoise Lite

FastNoise Lite is an extremely portable open source noise generation library with a large selection of noise algorithms. This library focuses on high performance while avoiding platform/language specific features, allowing for easy ports to as many possible languages.

## Features

- 2D & 3D sampling
- OpenSimplex2 noise
- OpenSimplex2S noise
- Cellular (Voronoi) noise
- Perlin noise
- Value noise
- Value Cubic noise
- OpenSimplex2-based domain warp
- Basic Grid Gradient domain warp
- Multiple fractal options for all of the above
- Supports `f32` or `f64` precision floats for X/Y sampling positions (see "Feature Flags" below)
- `no_std` (see "Feature Flags" below)

## Feature Flags

Optionally enable these in your `Cargo.toml` file with your `fastnoise-lite` dependency.

- `"f64"`: Uses `f64`, instead of the default `f32`, X/Y coordinate sampling inputs.
- `"std"`: Uses Rust's standard library for floating point operations (`sqrt()`, `trunc()`, and `abs()`). Either this or `"libm"` must be enabled. This is enabled by default if no feature flags are specified (but note that specifying `"f64"` will mean this is no longer default, and must be specified too).
- `"libm"`: Enables `no_std` support. Either this or `"std"` must be enabled. Uses the [libm](https://crates.io/crates/libm) optional dependency (through the [num-traits](https://crates.io/crates/num-traits) optional dependency) to allow floating point operations (`sqrt()`, `trunc()`, and `abs()`) without relying on the Rust standard library's implementations. FNL is dependency-free except when using this feature flag, which pulls in these optional dependencies.

## Getting Started

- First, construct a [`FastNoiseLite` struct](https://docs.rs/fastnoise-lite/latest/fastnoise_lite/struct.FastNoiseLite.html) using [`FastNoiseLite::new()`](https://docs.rs/fastnoise-lite/latest/fastnoise_lite/struct.FastNoiseLite.html#method.new) or [`FastNoiseLite::with_seed(seed)`](https://docs.rs/fastnoise-lite/latest/fastnoise_lite/struct.FastNoiseLite.html#method.with_seed).
- Next, you may call the various setter functions to configure the object's noise generation settings to your needs from their defaults.
- Optionally, you may use [`domain_warp_2d(x, y)`](https://docs.rs/fastnoise-lite/latest/fastnoise_lite/struct.FastNoiseLite.html#method.domain_warp_2d) and [`domain_warp_3d(x, y, z)`](https://docs.rs/fastnoise-lite/latest/fastnoise_lite/struct.FastNoiseLite.html#method.domain_warp_3d) to translate your original sampling coordinates into domain-warped coordinates before using them to sample the noise in the next step.
- Finally, sample the noise value at X/Y(/Z) coordinates for each pixel of your output data by calling [`get_noise_2d(x, y)`](https://docs.rs/fastnoise-lite/latest/fastnoise_lite/struct.FastNoiseLite.html#method.get_noise_2d) or [`get_noise_3d(x, y, z)`](https://docs.rs/fastnoise-lite/latest/fastnoise_lite/struct.FastNoiseLite.html#method.get_noise_3d). You may need to remap the output range from -1..1 to your desired range.

Additional documentation is available in the project's [Getting Started](https://github.com/Auburn/FastNoiseLite/wiki#getting-started) and [Documentation](https://github.com/Auburn/FastNoiseLite/wiki/Documentation) pages from its GitHub wiki.

Below is an example for creating a 128x128 array of OpenSimplex2 noise.

```rs
use fastnoise_lite::*;

// Create and configure the FastNoise object
let mut noise = FastNoiseLite::new();
noise.set_noise_type(Some(NoiseType::OpenSimplex2));

const WIDTH: usize = 128;
const HEIGHT: usize = 128;
let mut noise_data = [[0.; HEIGHT]; WIDTH];

// Sample noise pixels
for x in 0..WIDTH {
    for y in 0..HEIGHT {
        // Domain warp can optionally be employed to transform the coordinates before sampling:
        // let (x, y) = noise.domain_warp_2d(x as f32, y as f32);
        
        let negative_1_to_1 = noise.get_noise_2d(x as f32, y as f32);
        // You may want to remap the -1..1 range data to the 0..1 range:
        noise_data[x][y] = (neg_1_to_1 + 1.) / 2.;
        
        // (Uses of `as f32` above should become `as f64` if you're using FNL with the "f64" feature flag)
    }
}

// Do something with this data...
```

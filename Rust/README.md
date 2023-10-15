[![crates.io](https://img.shields.io/crates/v/fastnoise-lite?logo=rust "crates.io")](https://crates.io/crates/fastnoise-lite) • [docs.rs](https://docs.rs/fastnoise-lite/latest/fastnoise_lite/) • [GitHub](https://github.com/Auburn/FastNoiseLite)

# FastNoise Lite

FastNoise Lite is an extremely portable open source noise generation library with a large selection of noise algorithms. This library focuses on high performance while avoiding platform/language specific features, allowing for easy ports to as many possible languages.

## Features

- 2D & 3D
- OpenSimplex2 Noise
- OpenSimplex2S Noise
- Cellular (Voronoi) Noise
- Perlin Noise
- Value Noise
- Value Cubic Noise
- OpenSimplex2-based Domain Warp
- Basic Grid Gradient Domain Warp
- Multiple fractal options for all of the above
- Supports floats and/or doubles
  - Switch from `f32` to `f64` position inputs with the `"f64"` feature flag in your `Cargo.toml`

## Getting Started

Below is an example for creating a 128x128 array of OpenSimplex2 noise.

Additional documentation is available at [docs.rs](https://docs.rs/fastnoise-lite/latest/fastnoise_lite/) and the project's [Getting Started](https://github.com/Auburn/FastNoiseLite/wiki#getting-started) and [Documentation](https://github.com/Auburn/FastNoiseLite/wiki/Documentation) pages from its GitHub wiki.

```rs
use fastnoise_lite::*;

// Create and configure FastNoise object
let mut noise = FastNoiseLite::new();
noise.SetNoiseType(NoiseType::OpenSimplex2);

const WIDTH: usize = 128;
const HEIGHT: usize = 128;
let mut noise_data = [[0.; HEIGHT]; WIDTH];

// Gather noise data
for x in 0..WIDTH {
    for y in 0..HEIGHT {
        // Assuming `noise.get_noise_2d(x, y)` returns a float, and takes f32 parameters.
        // Adapt accordingly if the function signature is different.
        noise_data[x][y] = noise.get_noise_2d(x as f32, y as f32);
    }
}

// Do something with this data...
```

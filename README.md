[![discord](https://img.shields.io/discord/703636892901441577?style=flat-square&logo=discord "Discord")](https://discord.gg/SHVaVfV)
[![npm](https://img.shields.io/npm/v/fastnoise-lite)](https://www.npmjs.com/package/fastnoise-lite)

# FastNoise Lite

FastNoise Lite is an extremely portable open source noise generation library with a large selection of noise algorithms. This library focuses on high performance while avoiding platform/language specific features, allowing for easy ports to as many possible languages.

This project is an evolution of the [original FastNoise](https://github.com/Auburn/FastNoise/tree/FastNoise-Legacy) library and shares the same goal: An easy to use library that can quickly be integrated into a project and provides performant modern noise generation. See a breakdown of changes from the transition to FastNoise Lite [here](https://github.com/Auburn/FastNoise/pull/49)

If you are looking for a more extensive noise generation library consider using [FastNoise2](https://github.com/Auburn/FastNoise2). It provides large performance gains thanks to SIMD and uses a node graph structure to allow complex noise configurations with lots of flexibility.

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

### Supported Languages

- [C#](/CSharp/README.md)
- [C++98](/Cpp/README.md)
- [C99](/C/README.md)
- [Java](/Java/README.md)
- [JavaScript](/JavaScript/README.md)
- [HLSL](/HLSL/README.md)

### [Getting Started](https://github.com/Auburn/FastNoiseLite/wiki#getting-started)
### [Documentation](https://github.com/Auburn/FastNoiseLite/wiki/Documentation)

## FastNoise Lite Preview App

A compact testing application is available for testing all features included in FastNoise Lite with a visual representation. This can be used for development purposes and testing noise settings.

Download links can be found in the [Releases](https://github.com/Auburns/FastNoise/releases). Build instructions can be found in the PreviewApp directory.

![FastNoise GUI](https://user-images.githubusercontent.com/1349548/93670916-b19b3a00-fa96-11ea-9990-e866bc3d719e.png)

## Performance Comparisons

Benchmarked using C++ version with [NoiseBenchmarking](https://github.com/Auburn/NoiseBenchmarking)

- CPU: Intel 7820X @ 4.9Ghz
- OS: Win10 x64
- Compiler: clang-cl 10.0.0 -m64 /O2

Million points of noise generated per second (higher = better)

| 3D                 | Value  | Perlin | (*Open)Simplex | Cellular |
|--------------------|--------|--------|----------------|----------|
| FastNoise Lite     | 64.13  | 47.93  | 36.83*         | 12.49    |
| FastNoise (Legacy) | 49.34  | 37.75  | 44.74          | 13.27    |
| FastNoise 2 (AVX2) | 494.49 | 261.10 | 268.44         | 52.43    |
| libnoise           |        | 27.35  |                | 0.65     |
| stb perlin         |        | 34.32  |                |          |

| 2D                 | Value  | Perlin | Simplex | Cellular |
|--------------------|--------|--------|---------|----------|
| FastNoise Lite     | 114.01 | 92.83  | 71.30   | 39.15    |
| FastNoise (Legacy) | 102.12 | 87.99  | 65.29   | 36.84    |
| FastNoise 2 (AVX2) | 776.33 | 624.27 | 466.03  | 194.30   |

## Credits:

- [OpenSimplex2](https://github.com/KdotJPG/OpenSimplex2) for the OpenSimplex2 noise algorithm
- [@KdotJPG](https://github.com/KdotJPG) for implementing all the OpenSimplex alogrithms and the Java port
- [CubicNoise](https://github.com/jobtalle/CubicNoise) for the Value (Cubic) noise algorithm
- [@Rover656](https://github.com/Rover656) for creating the preview GUI and porting FastNoise Lite to C and HLSL.

# Examples

![Ridged Fractal](https://user-images.githubusercontent.com/1349548/93671180-b2cd6680-fa98-11ea-9026-0fb58b346c23.png)

![Cellular](https://user-images.githubusercontent.com/1349548/93670968-12c30d80-fa97-11ea-87ee-452173e784e0.png)

![Cellular Fractal](https://user-images.githubusercontent.com/1349548/93671060-dfcd4980-fa97-11ea-9792-da7df5cc7fa9.png)

![Cellular Warped](https://user-images.githubusercontent.com/1349548/93671113-3470c480-fa98-11ea-9da8-a279538ef7c2.png)

![Value Warped](https://user-images.githubusercontent.com/1349548/93671571-ff667100-fa9b-11ea-934c-1f1ab1f8d3f4.png)

![Cellular Scrolling](https://user-images.githubusercontent.com/1349548/93672159-97665980-faa0-11ea-9fcc-f5309b1a3a4b.gif)

![OpenSimplex2 Warp](https://user-images.githubusercontent.com/1349548/93671333-f674a000-fa99-11ea-8a34-1338b104dd82.png)

[![discord](https://img.shields.io/discord/703636892901441577?style=flat-square&logo=discord "Discord")](https://discord.gg/SHVaVfV)

# FastNoise Lite

FastNoise Lite is an extremely portable open source noise generation library with a large selection of noise algorithms. This library focuses on high performance while avoiding platform/language specific features, allowing for easy ports to as many possible languages.

This project is an evolution of the original FastNoise library and shares the same goal: An easy to use library that can quickly be integrated into a project and provides performant modern noise generation. 

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
- Supports floats or doubles

### Supported Languages

- C#
- C++
- C
- Java
- HLSL

## FastNoise Preview App

A compact testing application is available for testing all features included in FastNoise Lite with a visual representation. This can be used for development purposes and testing noise settings.

Download links can be found in the [Releases](https://github.com/Auburns/FastNoise/releases). Build instructions can also be found in the PreviewApp directory if you wish to build the application.

<!-- TODO: Update image -->
![FastNoise Preview](http://i.imgur.com/uG7Vepc.png)

## Performance Comparisons

Benchmarked using C++ version with [NoiseBenchmarking](https://github.com/Auburn/NoiseBenchmarking)
Using default noise settings on FastNoise Lite and matching those settings across the other libraries where possible.

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

## Cellular Noise

![Cellular Noise](http://i.imgur.com/quAic8M.png)

![Cellular Noise](http://i.imgur.com/gAd9Y2t.png)

![Cellular Noise](http://i.imgur.com/7kJd4fA.png)

## Fractal Noise

![Fractal Noise](http://i.imgur.com/XqSD7eR.png)

## Value Noise

![Value Noise](http://i.imgur.com/X2lbFZR.png)

## Basic Grid Gradient Perturb

![Gradient Perturb](http://i.imgur.com/gOjc1u1.png)

![Gradient Perturb](http://i.imgur.com/ui045Bk.png)

![Gradient Perturb](http://i.imgur.com/JICFypT.png)

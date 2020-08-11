[![discord](https://img.shields.io/discord/703636892901441577?style=flat-square&logo=discord "Discord")](https://discord.gg/SHVaVfV)

# FastNoise Lite

**FastNoise Lite is a work in progress**

FastNoise Lite is a portable open source noise generation library with a large selection of noise algorithms. This library focuses on high performance while avoiding platform/language specific features, allowing for easy ports to as many possible languages.

This project is an evolution of the original FastNoise library and shares the same goal: An easy to use library that can quickly be integrated into a project and provides performant modern noise generation. 

If you are looking for a more extensive noise generation library consider using [FastNoise2](https://github.com/Auburn/FastNoise2). It provides large performance gains thanks to SIMD and uses a node graph structure to allow huge amounts of flexibilty and configuration.

### Features

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

### Credits:

- [OpenSimplex2](https://github.com/KdotJPG/OpenSimplex2) for the OpenSimplex2 noise algorithm and [@KdotJPG](https://github.com/KdotJPG) for implementing it
- [CubicNoise](https://github.com/jobtalle/CubicNoise) for the Value (Cubic) noise algorithm
- [@Rover656](https://github.com/Rover656) for creating the preview GUI

## FastNoise Preview App

A compact testing application is available for testing all features included in FastNoise Lite with a visual representation. This is used for development purposes and testing noise settings used in terrain generation.

Download links can be found in the [Releases Section](https://github.com/Auburns/FastNoise/releases). Build instructions can also be found in the PreviewApp directory if you wish to change any of the settings.

<!-- TODO: Update image -->
![FastNoise Preview](http://i.imgur.com/uG7Vepc.png)


<!-- TODO: Update metrics for FastNoiseLite -->
# Performance Comparisons

Using default noise settings on FastNoise and matching those settings across the other libraries where possible.

Timings below are x1000 ns to generate 32x32x32 points of noise on a single thread.

- CPU: Intel Xeon Skylake @ 2.0Ghz
- Compiler: Intel 17.0 x64

| Noise Type    | FastNoise | FastNoiseSIMD AVX2 | LibNoise | FastNoise 2D |
|---------------|-----------|--------------------|----------|--------------|
| Simplex       | 1194      | 294                |          | 883          |
| Cellular      | 2979      | 1283               | 58125    | 1074         |
| Perlin        | 1002      | 324                | 1368     | 473          |
| Value (Cubic) | 2979      | 952                |          | 858          |
| Value         | 642       | 152                |          | 361          |

Comparision of fractal performance [here](https://github.com/Auburns/FastNoiseSIMD/wiki/In-depth-SIMD-level).

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


# Any suggestions or questions welcome

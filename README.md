[![discord](https://img.shields.io/discord/703636892901441577?style=flat-square&logo=discord "Discord")](https://discord.gg/SHVaVfV)

# FastNoise (Lite)

FastNoise Lite is a **portable** open source noise generation library with a similar collection of different noise algorithms to the original. This library has been designed for realtime usage from the ground up, so has been optimised for speed without sacrificing noise quality. FastNoise Lite will make it easier for ports to different languages to be made.

FastNoise Lite is a work in progress.

This project started when my search to find a good noise library for procedural terrain generation concluded without an obvious choice. I enjoyed the options and customisation of Accidental Noise Library and the speed of LibNoise, so many of the techniques from these libraries and the knowledge I gained from reading through their source has gone into creating FastNoise.

I have now also created [FastNoise2](https://github.com/Auburn/FastNoise2), successor to [FastNoise SIMD](https://github.com/Auburns/FastNoiseSIMD), which utilises SIMD CPU instructions to gain huge performance boosts. It is slightly less flexible and cannot be converted to other languages, but if you can I would highly suggest using this for heavy noise generation loads.

### Features

- Simplex Noise 2D, 3D
- OpenSimplex2 Noise 2D, 3D
- Cellular (Voronoi) Noise 2D, 3D
- Perlin Noise 2D, 3D
- Value (Cubic) Noise 2D, 3D
- Value Noise 2D, 3D
- OpenSimplex2-based Domain Warp 2D, 3D
- Basic Grid Gradient Domain Warp 2D, 3D
- Multiple fractal options for all of the above
- Supports floats or doubles

### Wiki

Usage and documentation available in wiki

[Wiki Link](https://github.com/Auburns/FastNoise/wiki)

<!-- TODO: Some of these could be removed soon -->
### Related repositories

 - [FastNoise2](https://github.com/Auburn/FastNoise2)
 - [FastNoise C#](https://github.com/Auburns/FastNoise_CSharp)
 - [FastNoise Java](https://github.com/Auburns/FastNoise_Java)
 - [FastNoise SIMD](https://github.com/Auburns/FastNoiseSIMD)
 - [FastNoise Unity](https://www.assetstore.unity3d.com/en/#!/content/70706)
 - [Unreal FastNoise](https://github.com/midgen/UnrealFastNoise)

Credits:

 - [OpenSimplex2](https://github.com/KdotJPG/OpenSimplex2) for the OpenSimplex2 noise algorithm
 - [CubicNoise](https://github.com/jobtalle/CubicNoise) for the Value (Cubic) noise algorithm

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

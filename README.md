# FastNoise

FastNoise is an open source noise generation library with a large collection of different noise algorithms. This library has been designed for realtime usage from the ground up, so has been optimised for speed without sacrificing noise quality.

This project started when my search to find a good noise library for procedural terrain generation concluded without an obvious choice. I enjoyed the options and customisation of Accidental Noise Library and the speed of LibNoise, so many of the techniques from these libraries and the knowledge I gained from reading through their source has gone into creating FastNoise. 

### Features
- Value Noise 2D, 3D
- Gradient (Perlin) Noise 2D, 3D
- Simplex Noise 2D, 3D, 4D
- Multiple fractal options for all of the above
- Cellular (Voronoi) Noise 2D, 3D
- White Noise 2D, 3D, 4D

### Wiki
Usage and documentation available in wiki

[Wiki Link](https://github.com/Auburns/FastNoise/wiki)

## FastNoise Preview

I have written a compact testing application for all the features included in FastNoise with a visual representation. I use this for development purposes and testing noise settings used in terrain generation.

Download links can be found in the [Releases Section](https://github.com/Auburns/FastNoise/releases).

![FastNoise Preview](http://i.imgur.com/33QdL8m.png)


#Performance Comparisons
Using default noise settings on FastNoise and matching those settings across the other libraries where possible.

Timing are averages of time taken for 1 million iterations on a single thread.

- CPU: i7 4790k @ 4.0Ghz
- Compiler: MSVC v140 x64

| Noise Type       | FastNoise | FastNoise SIMD - AVX2 | LibNoise | ANL      |
|------------------|-----------|-----------------------|----------|----------|
| Value            | 13.85     | 4.12                  | 24.16    | 94.73    |
| Value Fractal    | 46.99     | 12.22                 |          | 289.91   |
| Gradient         | 21.69     | 7.68                  | 32.68    | 109.26   |
| Gradient Fractal | 84.22     | 21.54                 | 122.15   | 325.04   |
| Simplex          | 27.56     | 7.03                  |          | 43.68    |
| Simplex Fractal  | 85.47     | 19.91                 |          | 154.41   |
| White Noise      | 2.81      | 0.47                  |          |          |
| Cellular         | 122.21    | 31.42                 | 1,122.60 | 2,473.06 |

#Notes

- I have a partially complete C# and Java version of FastNoise that I will release when I get it to the same feature level
- Any suggestions or questions welcome

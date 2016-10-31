# FastNoise

FastNoise is an open source noise generation library with a large collection of different noise algorithms. This library has been designed for realtime usage from the ground up, so has been optimised for speed without sacrificing noise quality.

This project started when my search to find a good noise library for procedural terrain generation concluded without an obvious choice. I enjoyed the options and customisation of Accidental Noise Library and the speed of LibNoise, so many of the techniques from these libraries and the knowledge I gained from reading through their source has gone into creating FastNoise.

I have now also created [FastNoise SIMD](https://github.com/Auburns/FastNoiseSIMD), which utilises SIMD CPU instructions to gain huge performance boosts. It is slightly less flexible and cannot be converted to other languages, but if you can I would highly suggest using this for heavy noise generation loads.

### Features
- Value Noise 2D, 3D
- Gradient (Perlin) Noise 2D, 3D
- Simplex Noise 2D, 3D, 4D
- Position Warping 2D, 3D
- Multiple fractal options for all of the above
- Cellular (Voronoi) Noise 2D, 3D
- White Noise 2D, 3D, 4D

### Wiki
Usage and documentation available in wiki

[Wiki Link](https://github.com/Auburns/FastNoise/wiki)

### Related repositories
 - [FastNoise C#](https://github.com/Auburns/FastNoise_CSharp)
 - [FastNoise SIMD](https://github.com/Auburns/FastNoiseSIMD)
 - [FastNoise Unity](https://www.assetstore.unity3d.com/en/#!/content/70706)
 - [Unreal FastNoise](https://github.com/midgen/UnrealFastNoise)

## FastNoise Preview

I have written a compact testing application for all the features included in FastNoise with a visual representation. I use this for development purposes and testing noise settings used in terrain generation.

Download links can be found in the [Releases Section](https://github.com/Auburns/FastNoise/releases).

![FastNoise Preview](http://i.imgur.com/uG7Vepc.png)


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

#Examples
##Cellular Noise
![Cellular Noise](http://i.imgur.com/quAic8M.png)

![Cellular Noise](http://i.imgur.com/gAd9Y2t.png)

![Cellular Noise](http://i.imgur.com/7kJd4fA.png)

##Fractal Noise
![Fractal Noise](http://i.imgur.com/XqSD7eR.png)

##Value Noise
![Value Noise](http://i.imgur.com/X2lbFZR.png)

##White Noise
![White Noise](http://i.imgur.com/QIlYvyQ.png)

##Position Warpring
![Position Warpring](http://i.imgur.com/gOjc1u1.png)

![Position Warpring](http://i.imgur.com/ui045Bk.png)

![Position Warpring](http://i.imgur.com/JICFypT.png)


#Any suggestions or questions welcome

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

## FastNoise Preview

I have written a compact testing application for all the features included in FastNoise with a visual representation. I use this for development purposes and testing noise settings used in terrain generation.

Download links can be found in the [Releases Section](https://github.com/Auburns/FastNoise/releases).

![FastNoise Preview](http://i.imgur.com/33QdL8m.png)

## LibNoise and Accidental Noise Library (ANL) Speed Comparisons

Using default noise settings on FastNoise and matching those settings across the other libraries where possible.

Timing are averages of time taken for 1 million iterations on a single thread.

CPU: i7 4790k @ 4.0Ghz

**WhiteNoise3D**

FastNoise |
1.52028 ms

**Value3D**

FastNoise |
14.752 ms

LibNoise |
23.324 ms

ANL |
101.81 

**ValueFractal3D**

FastNoise FBM |
49.485 ms

FastNoise Billow |
51.917 ms

FastNoise RigidMulti |
48.006 ms

ANL FBM |
308.46 ms

**Gradient3D**

FastNoise |
22.527 ms

LibNoise |
26.516 ms

ANL |
99.062 ms

**GradientFractal3D**

FastNoise FBM |
73.554 ms

FastNoise Billow |
79.088 ms

FastNoise RigidMulti |
73.024 ms

LibNoise Billow |
92.558 ms

ANL FBM |
302.5 ms

**Simplex3D**

FastNoise |
29.697 ms

ANL |
41.803 ms

**SimplexFractal3D**

FastNoise FBM |
101.44 ms

FastNoise Billow |
102.81 ms

FastNoise RigidMulti |
101.41 ms

ANL FBM |
145.98 ms

**Cellular3D**

FastNoise |
113.41 ms

FastNoise HQ |
449.82 ms

LibNoise |
1012.9 ms

ANL |
2934.5 ms

#Notes

- I have a partially complete C# and Java version of FastNoise that I will release when I get it to the same feature level
- Documentation coming at some point
- Any suggestions or questions welcome

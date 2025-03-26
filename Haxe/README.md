## Getting Started

Here's an example for creating a 128x128 array of OpenSimplex2 noise

```haxe
// Create and configure FastNoise object
var noise = new FastNoiseLite.Noise();
noise.noiseType = FastNoiseLite.NoiseType.OpenSimplex2;

// Gather noise data
var width = 128;
var height = 128;
var noiseData: Array<Float> = [];

for (x in 0...width) {
    for (y in 0...height) {
        noiseData[x + width * y] = noise.getNoise2D(x, y);
    }
}

// Do something with this data...
```

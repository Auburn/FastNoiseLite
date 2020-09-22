## Getting Started

Here's an example for creating a 128x128 array of OpenSimplex2 noise

```
// Create and configure FastNoise object
FastNoiseLite noise = new FastNoiseLite();
noise.SetNoiseType(FastNoiseLite.NoiseType.OpenSimplex2);

// Gather noise data
float[] noiseData = new float[128 * 128];
int index = 0;

for (int y = 0; y < 128; y++)
{
    for (int x = 0; x < 128; x++)
    {
        noiseData[index++] = noise.GetNoise(x, y);
    }
}

// Do something with this data...
```
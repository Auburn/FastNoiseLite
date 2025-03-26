## Getting Started

Here's an example for creating a 128x128 array of OpenSimplex2 noise

```cpp
// Create and configure FastNoise object
FastNoiseLite noise;
noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

// Gather noise data
std::vector<float> noiseData(128 * 128);
int index = 0;

for (int y = 0; y < 128; y++)
{
    for (int x = 0; x < 128; x++)
    {
        noiseData[index++] = noise.GetNoise((float)x, (float)y);
    }
}

// Do something with this data...
```

## Getting Started

Here's an example for creating a 128x128 array of OpenSimplex2 noise

```hlsl
// Create and configure noise state
fnl_state noise = fnlCreateState();
noise.noise_type = FNL_NOISE_OPENSIMPLEX2;

// Gather noise data
float noiseData[128 * 128];
int index = 0;

for (int y = 0; y < 128; y++)
{
    for (int x = 0; x < 128; x++) 
    {
        noiseData[index++] = fnlGetNoise2D(noise, x, y);
    }
}

// Do something with this data...
```

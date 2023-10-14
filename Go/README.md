## Getting Started

Here's an example for creating a 128x128 array of OpenSimplex2 noise

```go
import "fastnoise"

// Create and configure noise state (either float32 or float64)
var noise = fastnoise.New[float32]()
noise.NoiseType(fastnoise.OpenSimplex2)

// Gather noise data
var noiseData [128][128]float32

for x := 0; x < 128; x++ {
	for y := 0; y < 128; y++ {
		noiseData[x][y] = noise.Noise2D(x, y)
	}
}

// Do something with this data...
```

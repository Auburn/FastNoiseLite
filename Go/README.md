## Getting Started

Here's an example for creating a 128x128 array of Perlin noise

```go
import "fastnoise"

// Create and configure noise state (either float32 or float64)
noise := fastnoise.New[float32]()
noise.NoiseType(fastnoise.Perlin)

// Gather noise data
const size = 128
data := make([]float32, size * size)

for i := 0; i < len(data); i++ {		
	x := i % size
	y := i / size
	data[i] = noise.Noise2D(x, y)
}

// Use noise data (all values are in range of -1.0 and 1.0)
```

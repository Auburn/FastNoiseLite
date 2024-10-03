# Install

Simply copy the `fastnoise.zig` file directly into your project.

# Getting Started

The noise generator is implemented as a `comptime` function which returns a type, supporting both 32/64-bit floating-point types.
The generator does no allocations, nor is allocation required for initialization. 

While the initial state of the generator is valid, you will likely want to pass in some configuration...
```zig
const fastnoise = @import("fastnoise.zig");

const noise = fastnoise.Noise(f32) {
  .seed = 1337,
  .noise_type = .cellular,
  .frequency = 0.025,
  .gain = 0.40,
  .fractal_type = .fbm,
  .lucunarity = 0.40,
  .cellular_distance = .euclidian,
  .cellular_return = .distance2,
  .cellular_jitter_mod = 0.88,
};

const size = 128;
var values: [size * size]f32 = undefined;
for (0..values.len) |i| values[i] = noise.genNoise2D(@floatFromInt(i % size), @floatFromInt(i / size));

// Use noise values for whatever you please.
```

All functions of the generator are "pure" in that they do not alter its internal state, so you are free to initialize it as `const` if you will not be changing any of its fields afterwards.

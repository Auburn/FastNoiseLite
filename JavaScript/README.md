[![npm](https://img.shields.io/npm/v/fastnoise-lite?logo=npm "npm")](https://www.npmjs.com/package/fastnoise-lite) • [GitHub](https://github.com/Auburn/FastNoiseLite)

# FastNoise Lite

FastNoise Lite is an extremely portable open source noise generation library with a large selection of noise algorithms. This library focuses on high performance while avoiding platform/language specific features, allowing for easy ports to as many possible languages.

## Features

- 2D & 3D sampling
- OpenSimplex2 noise
- OpenSimplex2S noise
- Cellular (Voronoi) noise
- Perlin noise
- Value noise
- Value Cubic noise
- OpenSimplex2-based domain warp
- Basic Grid Gradient domain warp
- Multiple fractal options for all of the above

## Getting Started

### Using FastNoise Lite with npm

To begin install the npm package **fastnoise-lite**.

```sh
npm install fastnoise-lite
```

And import as follows:

```typescript
// using ESM
import FastNoiseLite from "fastnoise-lite";
// using CJS
const FastNoiseLite = require("fastnoise-lite").default;

let noise = new FastNoiseLite();
```

### Creating a 128x128 Array of OpenSimplex2 Noise

```typescript
// Create and configure FastNoiseLite object
let noise = new FastNoiseLite();
noise.SetNoiseType(FastNoiseLite.NoiseType.OpenSimplex2);

// Gather noise data
let noiseData: number[] = [];

for (let x = 0; x < 128; x++) {
    noiseData[x] = [];

    for (let y = 0; y < 128; y++) {        
        noiseData[x][y] = noise.GetNoise(x,y);
    }
}

// Do something with this data...
```

### BYO Vector2 / Vector3 classes

The DomainWarp method takes in a `Vector2 | Vector3` instance, but this library does not provide implementations of Vector classes.

You can use Vector classes from any other math library (like three.js), or just plain objects. Anything with x, y propeties (for 2D) or x, y, z properties (for 3D) will work. Input vectors are treated as 3D if they have a `z` property.

```typescript
let noise = new FastNoiseLite();
noise.SetDomainWarpType(FastNoiseLite.DomainWarpType.OpenSimplex2);
noise.SetDomainWarpAmp(1.5);

let vec1 = new THREE.Vector2(1, 2);
noise.DomainWarp(vec);

let vec2 = {x: 1, y: 2};
noise.DomainWarp(vec);
```


### Internal Method Overloading

Since JavaScript does not support method overloading it has been simulated to make the user experience more continuous
between language versions.

Some of the code has been changed to use switch statements since it provided a minor performance boost in some small 
frequently used places eg the _Hash methods.

Here is a template for the method overloading:

```typescript
class FastNoiseLite {
    /**
     * JSdoc
     */
    Method(coord: Vector2 | Vector3) {
        let R2 = (coord: Vector2) => {
            // do something 2D
        }

        let R3 = (coord: Vector3) => {
            // do something 3D
        }

        if ("z" in coord) {
            return R3(coord);
        } else {
            return R2(coord);
        }
    }

}
```

## Testing

There's a test script for the JS version. It generates a collection of noise images.

When making changes, you can compare the output between your branch and the master version.

(It requires node version 23.6+)

```sh
npm install
node test.mts
# it outputs test-output.png
```

### DM dev_storm on discord or email w/ any questions or need any support or ask someone in our discord :D

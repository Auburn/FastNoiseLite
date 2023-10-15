[![npm](https://img.shields.io/npm/v/fastnoise-lite?logo=npm "npm")](https://www.npmjs.com/package/fastnoise-lite) • [GitHub](https://github.com/Auburn/FastNoiseLite)

# FastNoise Lite

FastNoise Lite is an extremely portable open source noise generation library with a large selection of noise algorithms. This library focuses on high performance while avoiding platform/language specific features, allowing for easy ports to as many possible languages.

## Features

- 2D & 3D
- OpenSimplex2 Noise
- OpenSimplex2S Noise
- Cellular (Voronoi) Noise
- Perlin Noise
- Value Noise
- Value Cubic Noise
- OpenSimplex2-based Domain Warp
- Basic Grid Gradient Domain Warp
- Multiple fractal options for all of the above

## Getting Started

### Using FastNoiseLite with npm

To begin install the npm package **fastnoise-lite** with


Note FastNoiseLite does **not** support the node.js require(''); function.
Instead, enable ES6 modules and **import**.

```javascript
import FastNoiseLite from "fastnoise-lite";

let noise = new FastNoiseLite();
```

### Creating a 128x128 Array of OpenSimplex2 Noise

```javascript
// Create and configure FastNoiseLite object
let noise = new FastNoiseLite();
noise.SetNoiseType(FastNoiseLite.NoiseType.OpenSimplex2);

// Gather noise data
let noiseData = [];

for (let x = 0; x < 128; x++) {
    noiseData[x] = [];

    for (let y = 0; y < 128; y++) {        
        noiseData[x][y] = noise.GetNoise(x,y);
    }
}

// Do something with this data...
```

### Internal Method Overloading

Since JavaScript does not support method overloading it has been simulated to make the user experience more continuous
between language versions.

Some of the code has been changed to use switch statements since it provided a minor performance boost in some small 
frequently used places eg the _Hash methods.

Here is a template for the method overloading:

```javascript
class FastNoiseLite {
    /**
     * JSdoc
     */
    Method() {
        let R2 = (Param1, Param2) => {
            // do something 2D
        }

        let R3 = (Param1, Param2, Param3) => {
            // do something 3D
        }

        if (arguments.lenght === 2) {
            return R2(arguments[0], arguments[1]);
        }

        if (arguments.length === 3) {
            return R3(arguments[0], arguments[1], arguments[2])
        }
    }

}
```

### DM dev_storm on discord or email w/ any questions or need any support or ask someone in our discord :D

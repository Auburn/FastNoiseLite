## Links

* [npm](https://www.npmjs.com/package/fastnoise-lite)
* [github](https://github.com/Auburn/FastNoiseLite)
* [discord](https://discord.gg/SHVaVfV)

# Getting Started

### Using FastNoiseLite with npm

To begin install the npm package **fastnoise-lite** with


Note FastNoiseLite does **not** support the node.js require(''); function.
Instead, enable ES6 modules and **import**.

``` javascript
import FastNoiseLite from "fastnoise-lite";

let noise = new FastNoiseLite();
```

### Creating a 128x128 Array of OpenSimplex2 Noise

``` javascript
// Create and configure FastNoiseLite object
let noise = new FastNoiseLite();
noise.SetNoiseType(FastNoiseLite.NoiseType.OpenSimplex2);

// Gather noise data
let noiseData = [];

for (let y = 0; y < 128; y++) {
    for (let x = 0; x < 128; x++) {
        if (typeof noiseData[x] == 'undefined']) {
            noiseData[x] = [];
        }

        noiseData[x][y] = noise.GetNoise(x,y);
    }
}

// Do something with this data...
```

### Internal Method Overloading

Since JavaScript does not support method overloading it has been simulated to make the user experience more continuous
between language versions.

Some of the code has been changed to use switch statments since it provided a minor preformance boost in some small 
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

### DM storm#8888 on discord or email w/ any questions or need any support or ask someone in our discord :D
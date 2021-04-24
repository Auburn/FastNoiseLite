## Links

* [npm](https://www.npmjs.com/package/fast-noise-lite)
* [github](https://github.com/Auburn/FastNoiseLite)
* [discord](https://discord.gg/SHVaVfV)

# Getting Started

### Using FastNoise with npm

To begin install the npm package **fast-nose-lite** with


Note FastNoise does **not** support use with the node.js require(''); function.
Instead, use ES6 modules and **import**.

``` javascript
import FastNoise from "fast-noise-lite";

let noise = new FastNoiseLite();
```

### Creating a 128x128 Array of OpenSimplex2 Noise

``` javascript
// Create and configure FastNoise object
let noise = new FastNoiseLite();
noise.SetNoiseType(FastNoiseLite.NoiseType.OpenSimplex2);

// Gather noise data
let noiseData = [];

for (let y= 0; y < 128; y++) {
   for (let x= 0; x < 128; x++) {
      if (typeof noiseData[x] == 'undefined']){
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

Here is a template for the method overloading:

```javascript
class FastNoise {
    /**
     * use if you want to have doc show up when called e.g public method
     * @param {number} param1
     * @param {number} param2
     * @param {number} [param3]
     */
    methodWithDoc(param1, param2, param3/*optional*/) {
        let R2 = (Param1, Param2) => {
            //do something 2D
        }

        let R3 = (Param1, Param2, Param3) => {
            //do something 3D
        }


        if (arguments.lenght === 2) {
            return R2(param1, param2);
        }

        if (arguments.length === 3) {
            return R3(param1, param2, param3)
        }
    }

    /**
     * use if you want to have no doc show up when called e.g private method
     */
    methodNoDoc() {
        let R2 = (Param1, Param2) => {
            //do something 2D
        }

        let R3 = (Param1, Param2, Param3) => {
            //do something 3D
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

####DM storm#0009 on discord or email w/ any questions or need any support or ask someone in our discord :D
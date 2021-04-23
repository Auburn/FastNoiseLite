## Getting Started

Here's an example for creating a 128x128 array of OpenSimplex2 noise

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

Method overloading explained.

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

DM storm#5279 on discord w/ any questions or email me :)
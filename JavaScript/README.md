## Getting Started

Here's an example for creating a 128x128 array of OpenSimplex2 noise

```
// Create and configure FastNoise object
let noise = new FastNoiseLite();
noise.SetNoiseType(FastNoiseLite.NoiseType.OpenSimplex2);

// Gather noise data
let noiseData = [];

for(let y=0; y < 128; y++){
   for(let x=0; x < 128; x++){
      if(typeof noiseData[x] == 'undefined']){
         noiseData[x] = [];
      }

     noiseData[x][y] = noise.GetNoise(x,y);
   }
}

// Do something with this data...
```
import FastNoiseLite from "../src/FastNoiseLite"

// Create and configure FastNoiseLite object
let noise = new FastNoiseLite();
noise.SetNoiseType(FastNoiseLite.NoiseType.OpenSimplex2);

// Gather noise data
let noiseData: number[][] = [];

for (let x = 0; x < 128; x++) {
  noiseData[x] = [];

  for (let y = 0; y < 128; y++) {        
    noiseData[x][y] = noise.GetNoise(x,y);
  }
}

noise.SetDomainWarpType(FastNoiseLite.DomainWarpType.OpenSimplex2);
noise.SetDomainWarpAmp(1.5);

// let vec1 = new THREE.Vector2(1, 2);
// noise.DomainWarp(vec1);

let vec2 = {x: 1, y: 2};
noise.DomainWarp(vec2);
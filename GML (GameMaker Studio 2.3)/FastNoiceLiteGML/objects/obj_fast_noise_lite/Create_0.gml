// Create and configure FastNoiseLite object
var noise = new FastNoiseLite();
noise.SetNoiseType(FastNoiseLite.NoiseType.OpenSimplex2);


// Gather noise data
noiseData = [];
for (var _x= 0; _x < room_width; _x++) {
		if (_x >= array_length(noiseData)){
				noiseData[_x] = [];
		}
		for (var _y= 0; _y < room_height; _y++) {
			noiseData[_x][_y] = noise.GetNoise(_x, _y);
		}
}


/// @desc Regenerate the noise

// Create and configure FastNoiseLite object
var noise = new FastNoiseLite(random(4_294_967_295));
noise.SetNoiseType(FastNoiseLite.NoiseType.OpenSimplex2);

//create a surface to cache the noise greyscale
noise_surface = surface_create(noise_width, noise_height);
surface_set_target(noise_surface);

// Gather noise data
noiseData = [];
noise_generation_time = 0
var _time_start = get_timer()

for (var _x= 0; _x < noise_width; _x++) {
		if (_x >= array_length(noiseData)){
				noiseData[_x] = [];
		}
		for (var _y= 0; _y < noise_height; _y++) {
			noiseData[_x][_y] = noise.GetNoise(_x, _y);
			
			//convert to color
			var _color = make_color_hsv(0, 0, (noiseData[_x][_y]+1)*128);
			draw_set_color(_color);
			
			//draw the greyscaled point
			draw_point(_x, _y);
		}
}

noise_generation_time = get_timer() - _time_start

//reset drawing
surface_reset_target();
draw_set_color(c_white);
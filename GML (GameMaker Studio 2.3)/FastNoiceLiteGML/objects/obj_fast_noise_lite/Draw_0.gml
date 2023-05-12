/// @desc Draw the noise data

for (var _x= 0; _x < array_length(noiseData); _x++) {
		for (var _y= 0; _y < array_length(noiseData[_x]); _y++) {
			
			//Note: Not all noise algorithms will produce a value between -1 and 1,
			// some will generate a value between 0-1. So be sure to update how you project the values.
			
			//convert the data into greyscale color
			var _color = make_color_hsv(0, 0, (noiseData[_x][_y]+1)*128);
			draw_set_color(_color);
			
			//draw the greyscaled point
			draw_point(_x, _y);
		}
}

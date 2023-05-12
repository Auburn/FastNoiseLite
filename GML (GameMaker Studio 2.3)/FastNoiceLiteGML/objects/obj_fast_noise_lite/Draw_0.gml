/// @desc Draw the noise data
draw_surface(noise_surface, 0, 0)

//draw_drop_shadow_text
var _thickness = 2;
var _color_bg = c_black;
var _color = c_white;
var _x = 8
var _y = 8
var _text = string("Seed: {0}\nFPS: {1}\nUnique Noise Generated: {2}\nTime to generate: {3}ms\nPress spacebar to regenerate noise", noise._Seed, fps_real, noise_width*noise_height, noise_generation_time)
draw_set_color(_color_bg)

for (var _xoff = -_thickness; _xoff <= _thickness; _xoff++) {
		for (var _yoff = -_thickness; _yoff <= _thickness; _yoff++) {
			draw_text(_x+_xoff, _y+_yoff, _text)
		}
}

draw_set_color(_color)
draw_text(_x, _y, _text)

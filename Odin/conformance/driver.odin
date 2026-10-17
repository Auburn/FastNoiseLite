// FastNoiseLite conformance driver (Odin). See conformance/README.md.
// Usage: driver VECTORS_DIR > output.txt
// Build with -define:USE_F64=true to test the f64 coordinate variant (FNL_Float = f64).
// Reading and printing go through libc, as in the C driver, so the output does not depend on the
// Odin release (dev-2024-10's strconv misparses "-10.0", core:os changed its API after dev-2025-04,
// and fmt's %.17g is not exact above 1e16).
package main

import "core:c/libc"
import "core:os"
import "core:strings"
import fnl ".."

F :: fnl.FNL_Float

open_in :: proc(dir, name: string) -> ^libc.FILE {
	path := strings.clone_to_cstring(strings.concatenate({dir, "/", name}))
	f := libc.fopen(path, "r")
	if f == nil {
		libc.fprintf(libc.stderr, "cannot open %s\n", path)
		os.exit(2)
	}
	return f
}

run :: proc(s: fnl.FNL_State, c: i32, p2: [][2]f64, p3: [][3]f64) {
	for p, i in p2 do libc.printf("N2 %d %d %.17g\n", c, i32(i), f64(fnl.get_noise_2d(s, F(p.x), F(p.y))))
	for p, i in p3 do libc.printf("N3 %d %d %.17g\n", c, i32(i), f64(fnl.get_noise_3d(s, F(p.x), F(p.y), F(p.z))))
	for p, i in p2 {
		x, y := F(p.x), F(p.y)
		fnl.domain_warp_2d(s, &x, &y)
		libc.printf("W2 %d %d %.17g %.17g\n", c, i32(i), f64(x), f64(y))
	}
	for p, i in p3 {
		x, y, z := F(p.x), F(p.y), F(p.z)
		fnl.domain_warp_3d(s, &x, &y, &z)
		libc.printf("W3 %d %d %.17g %.17g %.17g\n", c, i32(i), f64(x), f64(y), f64(z))
	}
}

main :: proc() {
	if len(os.args) < 2 {
		libc.fprintf(libc.stderr, "usage: driver VECTORS_DIR\n")
		os.exit(2)
	}
	dir := os.args[1]
	p2: [dynamic][2]f64
	p3: [dynamic][3]f64
	p: [3]f64
	f := open_in(dir, "points2d.txt")
	for libc.fscanf(f, "%lf %lf", &p.x, &p.y) == 2 do append(&p2, p.xy)
	f = open_in(dir, "points3d.txt")
	for libc.fscanf(f, "%lf %lf %lf", &p.x, &p.y, &p.z) == 3 do append(&p3, p)

	run(fnl.create_state(), -1, p2[:], p3[:])

	// The port is struct-based: assign the fields in the order the README lists the setters.
	// Its enums use the C++ ordinals.
	seed, nt, rot, ft, octaves, cdist, cret, wt: i32
	freq, lac, gain, ws, pp, jitter, amp: f32
	f = open_in(dir, "configs.txt")
	for c: i32 = 0; libc.fscanf(f, "%d %f %d %d %d %d %f %f %f %f %d %d %f %d %f", &seed, &freq, &nt, &rot, &ft,
	    &octaves, &lac, &gain, &ws, &pp, &cdist, &cret, &jitter, &wt, &amp) == 15; c += 1 {
		s := fnl.create_state()
		s.seed = seed
		s.frequency = freq
		s.noise_type = fnl.Noise_Type(nt)
		s.rotation_type_3d = fnl.Rotation_Type_3d(rot)
		s.fractal_type = fnl.Fractal_Type(ft)
		s.octaves = octaves
		s.lacunarity = lac
		s.gain = gain
		s.weighted_strength = ws
		s.ping_pong_strength = pp
		s.cellular_distance_func = fnl.Cellular_Distance_Func(cdist)
		s.cellular_return_type = fnl.Cellular_Return_Type(cret)
		s.cellular_jitter_mod = jitter
		s.domain_warp_type = fnl.Domain_Warp_Type(wt)
		s.domain_warp_amp = amp
		run(s, c, p2[:], p3[:])
	}
}

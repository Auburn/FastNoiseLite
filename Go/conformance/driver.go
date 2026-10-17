// FastNoiseLite conformance driver (Go). See conformance/README.md.
// Usage: driver VECTORS_DIR [float32|float64] > output.txt
// The port is generic; the second argument picks the instantiation (default float32).
// The Go enum constants have the same ordinals as the C++ enums.
package main

import (
	"bufio"
	"fmt"
	"os"
	"path/filepath"
	"strconv"
	"strings"

	fastnoise "github.com/Auburn/FastNoiseLite/Go"
)

var out = bufio.NewWriter(os.Stdout)

func rows(dir, name string) [][]float64 {
	data, err := os.ReadFile(filepath.Join(dir, name))
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(2)
	}
	var r [][]float64
	for _, line := range strings.Split(string(data), "\n") {
		var v []float64
		for _, s := range strings.Fields(line) {
			d, err := strconv.ParseFloat(s, 64)
			if err != nil {
				fmt.Fprintln(os.Stderr, err)
				os.Exit(2)
			}
			v = append(v, d)
		}
		if len(v) > 0 {
			r = append(r, v)
		}
	}
	return r
}

// emit prints one line. If the port panics, the line is skipped and counts as missing.
func emit(line func() string) {
	defer func() {
		if r := recover(); r != nil {
			fmt.Fprintln(os.Stderr, "panic:", r)
		}
	}()
	s := line()
	fmt.Fprintln(out, s)
}

func run[T fastnoise.Float](n *fastnoise.State[T], c int, p2, p3 [][]float64) {
	for i, p := range p2 {
		emit(func() string { return fmt.Sprintf("N2 %d %d %.17g", c, i, float64(n.GetNoise2D(T(p[0]), T(p[1])))) })
	}
	for i, p := range p3 {
		emit(func() string {
			return fmt.Sprintf("N3 %d %d %.17g", c, i, float64(n.GetNoise3D(T(p[0]), T(p[1]), T(p[2]))))
		})
	}
	for i, p := range p2 {
		emit(func() string {
			x, y := n.DomainWarp2D(T(p[0]), T(p[1]))
			return fmt.Sprintf("W2 %d %d %.17g %.17g", c, i, float64(x), float64(y))
		})
	}
	for i, p := range p3 {
		emit(func() string {
			x, y, z := n.DomainWarp3D(T(p[0]), T(p[1]), T(p[2]))
			return fmt.Sprintf("W3 %d %d %.17g %.17g %.17g", c, i, float64(x), float64(y), float64(z))
		})
	}
}

func all[T fastnoise.Float](dir string) {
	p2, p3 := rows(dir, "points2d.txt"), rows(dir, "points3d.txt")
	run(fastnoise.New[T](), -1, p2, p3)
	// Config values go through float32, like the float members of the C++ class.
	f := func(v float64) T { return T(float32(v)) }
	for c, v := range rows(dir, "configs.txt") {
		n := fastnoise.New[T]()
		n.Seed = int(v[0])
		n.Frequency = f(v[1])
		n.NoiseType(fastnoise.NoiseType(v[2]))
		n.RotationType3D = fastnoise.RotationType3D(v[3])
		n.FractalType(fastnoise.FractalType(v[4]))
		n.Octaves = int(v[5])
		n.Lacunarity = f(v[6])
		n.Gain = f(v[7])
		n.WeightedStrength = f(v[8])
		n.PingPongStrength = f(v[9])
		n.CellularDistanceFunc = fastnoise.CellularDistanceFunc(v[10])
		n.CellularReturnType = fastnoise.CellularReturnType(v[11])
		n.CellularJitterMod = f(v[12])
		n.DomainWarpType = fastnoise.DomainWarpType(v[13])
		n.DomainWarpAmp = f(v[14])
		run(n, c, p2, p3)
	}
}

func main() {
	defer out.Flush()
	if len(os.Args) < 2 {
		fmt.Fprintln(os.Stderr, "usage: driver VECTORS_DIR [float32|float64]")
		os.Exit(2)
	}
	if len(os.Args) > 2 && os.Args[2] == "float64" {
		all[float64](os.Args[1])
	} else {
		all[float32](os.Args[1])
	}
}

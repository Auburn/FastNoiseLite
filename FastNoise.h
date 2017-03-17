// FastNoise.h
//
// MIT License
//
// Copyright(c) 2016 Jordan Peck
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// The developer's email is jorzixdan.me2@gzixmail.com (for great email, take
// off every 'zix'.)
//

#ifndef FASTNOISE_H
#define FASTNOISE_H

// Uncomment the line below to use doubles throughout FastNoise instead of floats
//#define FN_USE_DOUBLES

#ifdef FN_USE_DOUBLES
typedef double FN_DECIMAL;
#else
typedef float FN_DECIMAL;
#endif

class FastNoise
{
public:
	FastNoise(int seed = 1337) { SetSeed(seed); CalculateFractalBounding(); };
	~FastNoise() { delete m_cellularNoiseLookup; }

	enum NoiseType { Value, ValueFractal, Perlin, PerlinFractal, Simplex, SimplexFractal, Cellular, WhiteNoise, Cubic, CubicFractal };
	enum Interp { Linear, Hermite, Quintic };
	enum FractalType { FBM, Billow, RigidMulti };
	enum CellularDistanceFunction { Euclidean, Manhattan, Natural };
	enum CellularReturnType { CellValue, NoiseLookup, Distance, Distance2, Distance2Add, Distance2Sub, Distance2Mul, Distance2Div };

	// Returns seed used for all noise types
	void SetSeed(int seed);

	// Sets seed used for all noise types
	// Default: 1337
	int GetSeed(void) const { return m_seed; }

	// Sets frequency for all noise types
	// Default: 0.01
	void SetFrequency(FN_DECIMAL frequency) { m_frequency = frequency; }

	// Changes the interpolation method used to smooth between noise values
	// Possible interpolation methods (lowest to highest quality) :
	// - Linear
	// - Hermite
	// - Quintic
	// Used in Value, Perlin Noise and Position Warping
	// Default: Quintic
	void SetInterp(Interp interp) { m_interp = interp; }

	// Sets noise return type of GetNoise(...)
	// Default: Simplex
	void SetNoiseType(NoiseType noiseType) { m_noiseType = noiseType; }

	// Sets octave count for all fractal noise types
	// Default: 3
	void SetFractalOctaves(int octaves) { m_octaves = octaves; CalculateFractalBounding(); }
	
	// Sets octave lacunarity for all fractal noise types
	// Default: 2.0
	void SetFractalLacunarity(FN_DECIMAL lacunarity) { m_lacunarity = lacunarity; }

	// Sets octave gain for all fractal noise types
	// Default: 0.5
	void SetFractalGain(FN_DECIMAL gain) { m_gain = gain; CalculateFractalBounding(); }

	// Sets method for combining octaves in all fractal noise types
	// Default: FBM
	void SetFractalType(FractalType fractalType) { m_fractalType = fractalType; }

	// Sets return type from cellular noise calculations
	// Note: NoiseLookup requires another FastNoise object be set with SetCellularNoiseLookup() to function
	// Default: CellValue
	void SetCellularDistanceFunction(CellularDistanceFunction cellularDistanceFunction) { m_cellularDistanceFunction = cellularDistanceFunction; }
	
	// Sets distance function used in cellular noise calculations
	// Default: Euclidean
	void SetCellularReturnType(CellularReturnType cellularReturnType) { m_cellularReturnType = cellularReturnType; }
	
	// Noise used to calculate a cell value if cellular return type is NoiseLookup
	// The lookup value is acquired through GetNoise() so ensure you SetNoiseType() on the noise lookup, value, Perlin or simplex is recommended
	void SetCellularNoiseLookup(FastNoise* noise) { m_cellularNoiseLookup = noise; }

	// Sets the maximum warp distance from original location when using GradientPerturb{Fractal}(...)
	// Default: 1.0
	void SetGradientPerturbAmp(FN_DECIMAL gradientPerturbAmp) { m_gradientPerturbAmp = gradientPerturbAmp / 0.45f; }

	//2D												
	FN_DECIMAL GetValue(FN_DECIMAL x, FN_DECIMAL y);					
	FN_DECIMAL GetValueFractal(FN_DECIMAL x, FN_DECIMAL y);			

	FN_DECIMAL GetPerlin(FN_DECIMAL x, FN_DECIMAL y);				
	FN_DECIMAL GetPerlinFractal(FN_DECIMAL x, FN_DECIMAL y);			

	FN_DECIMAL GetSimplex(FN_DECIMAL x, FN_DECIMAL y);					
	FN_DECIMAL GetSimplexFractal(FN_DECIMAL x, FN_DECIMAL y);			

	FN_DECIMAL GetCellular(FN_DECIMAL x, FN_DECIMAL y);				

	FN_DECIMAL GetWhiteNoise(FN_DECIMAL x, FN_DECIMAL y);				
	FN_DECIMAL GetWhiteNoiseInt(int x, int y);

	FN_DECIMAL GetCubic(FN_DECIMAL x, FN_DECIMAL y);
	FN_DECIMAL GetCubicFractal(FN_DECIMAL x, FN_DECIMAL y);

	FN_DECIMAL GetNoise(FN_DECIMAL x, FN_DECIMAL y);

	void GradientPerturb(FN_DECIMAL& x, FN_DECIMAL& y);
	void GradientPerturbFractal(FN_DECIMAL& x, FN_DECIMAL& y);

	//3D												
	FN_DECIMAL GetValue(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);			
	FN_DECIMAL GetValueFractal(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);	

	FN_DECIMAL GetPerlin(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);		
	FN_DECIMAL GetPerlinFractal(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);

	FN_DECIMAL GetSimplex(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);		
	FN_DECIMAL GetSimplexFractal(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);	

	FN_DECIMAL GetCellular(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);		

	FN_DECIMAL GetWhiteNoise(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);		
	FN_DECIMAL GetWhiteNoiseInt(int x, int y, int z);

	FN_DECIMAL GetCubic(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);
	FN_DECIMAL GetCubicFractal(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);

	FN_DECIMAL GetNoise(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);

	void GradientPerturb(FN_DECIMAL& x, FN_DECIMAL& y, FN_DECIMAL& z);
	void GradientPerturbFractal(FN_DECIMAL& x, FN_DECIMAL& y, FN_DECIMAL& z);

	//4D
	FN_DECIMAL GetSimplex(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, FN_DECIMAL w);

	FN_DECIMAL GetWhiteNoise(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, FN_DECIMAL w);
	FN_DECIMAL GetWhiteNoiseInt(int x, int y, int z, int w);

protected:
	unsigned char m_perm[512];
	unsigned char m_perm12[512];

	int m_seed = 1337;
	FN_DECIMAL m_frequency = FN_DECIMAL(0.01);
	Interp m_interp = Quintic;
	NoiseType m_noiseType = Simplex;

	int m_octaves = 3;
	FN_DECIMAL m_lacunarity = FN_DECIMAL(2);
	FN_DECIMAL m_gain = FN_DECIMAL(0.5);
	FractalType m_fractalType = FBM;

	FN_DECIMAL m_fractalBounding;
	void CalculateFractalBounding()
	{
		FN_DECIMAL amp = m_gain;
		FN_DECIMAL ampFractal = 1;
		for (int i = 1; i < m_octaves; i++)
		{
			ampFractal += amp;
			amp *= m_gain;
		}
		m_fractalBounding = 1 / ampFractal;
	}

	CellularDistanceFunction m_cellularDistanceFunction = Euclidean;
	CellularReturnType m_cellularReturnType = CellValue;
	FastNoise* m_cellularNoiseLookup = nullptr;

	FN_DECIMAL m_gradientPerturbAmp = 1 / FN_DECIMAL(0.45);

	//2D
	FN_DECIMAL SingleValueFractalFBM(FN_DECIMAL x, FN_DECIMAL y);
	FN_DECIMAL SingleValueFractalBillow(FN_DECIMAL x, FN_DECIMAL y);
	FN_DECIMAL SingleValueFractalRigidMulti(FN_DECIMAL x, FN_DECIMAL y);
	FN_DECIMAL SingleValue(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y);

	FN_DECIMAL SinglePerlinFractalFBM(FN_DECIMAL x, FN_DECIMAL y);
	FN_DECIMAL SinglePerlinFractalBillow(FN_DECIMAL x, FN_DECIMAL y);
	FN_DECIMAL SinglePerlinFractalRigidMulti(FN_DECIMAL x, FN_DECIMAL y);
	FN_DECIMAL SinglePerlin(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y);

	FN_DECIMAL SingleSimplexFractalFBM(FN_DECIMAL x, FN_DECIMAL y);
	FN_DECIMAL SingleSimplexFractalBillow(FN_DECIMAL x, FN_DECIMAL y);
	FN_DECIMAL SingleSimplexFractalRigidMulti(FN_DECIMAL x, FN_DECIMAL y);
	FN_DECIMAL SingleSimplexFractalBlend(FN_DECIMAL x, FN_DECIMAL y);
	FN_DECIMAL SingleSimplex(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y);

	FN_DECIMAL SingleCubicFractalFBM(FN_DECIMAL x, FN_DECIMAL y);
	FN_DECIMAL SingleCubicFractalBillow(FN_DECIMAL x, FN_DECIMAL y);
	FN_DECIMAL SingleCubicFractalRigidMulti(FN_DECIMAL x, FN_DECIMAL y);
	FN_DECIMAL SingleCubic(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y);

	FN_DECIMAL SingleCellular(FN_DECIMAL x, FN_DECIMAL y);
	FN_DECIMAL SingleCellular2Edge(FN_DECIMAL x, FN_DECIMAL y);

	void SingleGradientPerturb(unsigned char offset, FN_DECIMAL warpAmp, FN_DECIMAL frequency, FN_DECIMAL& x, FN_DECIMAL& y);

	//3D
	FN_DECIMAL SingleValueFractalFBM(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);
	FN_DECIMAL SingleValueFractalBillow(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);
	FN_DECIMAL SingleValueFractalRigidMulti(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);
	FN_DECIMAL SingleValue(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);

	FN_DECIMAL SinglePerlinFractalFBM(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);
	FN_DECIMAL SinglePerlinFractalBillow(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);
	FN_DECIMAL SinglePerlinFractalRigidMulti(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);
	FN_DECIMAL SinglePerlin(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);

	FN_DECIMAL SingleSimplexFractalFBM(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);
	FN_DECIMAL SingleSimplexFractalBillow(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);
	FN_DECIMAL SingleSimplexFractalRigidMulti(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);
	FN_DECIMAL SingleSimplex(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);

	FN_DECIMAL SingleCubicFractalFBM(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);
	FN_DECIMAL SingleCubicFractalBillow(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);
	FN_DECIMAL SingleCubicFractalRigidMulti(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);
	FN_DECIMAL SingleCubic(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);

	FN_DECIMAL SingleCellular(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);
	FN_DECIMAL SingleCellular2Edge(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);

	void SingleGradientPerturb(unsigned char offset, FN_DECIMAL warpAmp, FN_DECIMAL frequency, FN_DECIMAL& x, FN_DECIMAL& y, FN_DECIMAL& z);

	//4D
	FN_DECIMAL SingleSimplex(unsigned char offset, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z, FN_DECIMAL w);
private:
	inline unsigned char Index2D_12(unsigned char offset, int x, int y);
	inline unsigned char Index3D_12(unsigned char offset, int x, int y, int z);
	inline unsigned char Index4D_32(unsigned char offset, int x, int y, int z, int w);
	inline unsigned char Index2D_256(unsigned char offset, int x, int y);
	inline unsigned char Index3D_256(unsigned char offset, int x, int y, int z);
	inline unsigned char Index4D_256(unsigned char offset, int x, int y, int z, int w);

	inline FN_DECIMAL ValCoord2DFast(unsigned char offset, int x, int y);
	inline FN_DECIMAL ValCoord3DFast(unsigned char offset, int x, int y, int z);
	inline FN_DECIMAL GradCoord2D(unsigned char offset, int x, int y, FN_DECIMAL xd, FN_DECIMAL yd);
	inline FN_DECIMAL GradCoord3D(unsigned char offset, int x, int y, int z, FN_DECIMAL xd, FN_DECIMAL yd, FN_DECIMAL zd);
	inline FN_DECIMAL GradCoord4D(unsigned char offset, int x, int y, int z, int w, FN_DECIMAL xd, FN_DECIMAL yd, FN_DECIMAL zd, FN_DECIMAL wd);
};
#endif

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

class FastNoise
{
public:
	FastNoise(int seed = 1337) { m_seed = seed; }
	~FastNoise() { delete m_cellularNoiseLookup; }

	enum NoiseType { Value, ValueFractal, Gradient, GradientFractal, Simplex, SimplexFractal, Cellular, WhiteNoise };
	enum Interp { InterpLinear = 0, InterpHermite = 1, InterpQuintic = 2 };
	enum FractalType { FBM, Billow, RigidMulti };
	enum CellularDistanceFunction { Euclidean, Manhattan, Natural };
	enum CellularReturnType { CellValue, NoiseLookup, Distance2Center, Distance2CenterXValue, Distance2CenterSq, Distance2CenterSqXValue, Distance2Edge, Distance2EdgeXValue, Distance2EdgeSq, Distance2EdgeSqXValue };

	void SetSeed(int seed) { m_seed = seed; }
	int GetSeed(void) const { return m_seed; }
	void SetFrequency(float frequency) { m_frequency = frequency; }
	void SetInterp(Interp interp) { m_interp = interp; }
	void SetNoiseType(NoiseType noiseType) { m_noiseType = noiseType; }

	void SetFractalOctaves(unsigned int octaves) { m_octaves = octaves; }
	void SetFractalLacunarity(float lacunarity) { m_lacunarity = lacunarity; }
	void SetFractalGain(float gain) { m_gain = gain; }
	void SetFractalType(FractalType fractalType) { m_fractalType = fractalType; }

	void SetCellularDistanceFunction(CellularDistanceFunction cellularDistanceFunction) { m_cellularDistanceFunction = cellularDistanceFunction; }
	void SetCellularReturnType(CellularReturnType cellularReturnType) { m_cellularReturnType = cellularReturnType; }
	void SetCellularNoiseLookup(FastNoise* noise) { m_cellularNoiseLookup = noise; }

	/*
	Timing below are averages of time taken for 1 million iterations on a single thread
	Default noise settings
	CPU: i7 4790k @ 4.0Ghz
	VS 2013 - C++ Console Application
	*/

	//3D												// Win32	x64
	float GetValue(float x, float y, float z);			// 14 ms	14 ms
	float GetValueFractal(float x, float y, float z);	// 48 ms	49 ms

	float GetGradient(float x, float y, float z);		// 23 ms	22 ms
	float GetGradientFractal(float x, float y, float z);// 80 ms	73 ms

	float GetSimplex(float x, float y, float z);		// 30 ms	30 ms
	float GetSimplexFractal(float x, float y, float z);	// 98 ms	101 ms

	float GetCellular(float x, float y, float z);		// 123 ms	113 ms

	float GetWhiteNoise(float x, float y, float z);		// 1.5 ms	1.5 ms
	float GetWhiteNoiseInt(int x, int y, int z);

	float GetNoise(float x, float y, float z);

	//2D												// Win32	x64
	float GetValue(float x, float y);					// 8 ms 	8 ms
	float GetValueFractal(float x, float y);			// 29 ms	29 ms

	float GetGradient(float x, float y);				// 12 ms	11 ms
	float GetGradientFractal(float x, float y);			// 43 ms	40 ms

	float GetSimplex(float x, float y);					// 17 ms	17 ms
	float GetSimplexFractal(float x, float y);			// 55 ms	52 ms

	float GetCellular(float x, float y);				// 35 ms	33 ms

	float GetWhiteNoise(float x, float y);				// 1 ms		1 ms
	float GetWhiteNoiseInt(int x, int y);				// 1 ms		1 ms

	float GetNoise(float x, float y);

	//4D
	float GetSimplex(float x, float y, float z, float w);

	float GetWhiteNoise(float x, float y, float z, float w);
	float GetWhiteNoiseInt(int x, int y, int z, int w);

protected:
	int m_seed = 1337;
	float m_frequency = 0.01f;
	Interp m_interp = InterpQuintic;
	NoiseType m_noiseType = Simplex;

	unsigned int m_octaves = 3;
	float m_lacunarity = 2.0f;
	float m_gain = 0.5f;
	FractalType m_fractalType = FBM;

	CellularDistanceFunction m_cellularDistanceFunction = Euclidean;
	CellularReturnType m_cellularReturnType = CellValue;
	FastNoise* m_cellularNoiseLookup = nullptr;

	//2D
	float SingleValueFractalFBM(float x, float y);
	float SingleValueFractalBillow(float x, float y);
	float SingleValueFractalRigidMulti(float x, float y);
	float SingleValue(int seed, float x, float y);

	float SingleGradientFractalFBM(float x, float y);
	float SingleGradientFractalBillow(float x, float y);
	float SingleGradientFractalRigidMulti(float x, float y);
	float SingleGradient(int seed, float x, float y);

	float SingleSimplexFractalFBM(float x, float y);
	float SingleSimplexFractalBillow(float x, float y);
	float SingleSimplexFractalRigidMulti(float x, float y);
	float SingleSimplex(int seed, float x, float y);

	float SingleCellular(float x, float y);
	float SingleCellular2Edge(float x, float y);	

	//3D
	float SingleValueFractalFBM(float x, float y, float z);
	float SingleValueFractalBillow(float x, float y, float z);
	float SingleValueFractalRigidMulti(float x, float y, float z);
	float SingleValue(int seed, float x, float y, float z);

	float SingleGradientFractalFBM(float x, float y, float z);
	float SingleGradientFractalBillow(float x, float y, float z);
	float SingleGradientFractalRigidMulti(float x, float y, float z);
	float SingleGradient(int seed, float x, float y, float z);

	float SingleSimplexFractalFBM(float x, float y, float z);
	float SingleSimplexFractalBillow(float x, float y, float z);
	float SingleSimplexFractalRigidMulti(float x, float y, float z);
	float SingleSimplex(int seed, float x, float y, float z);

	float SingleCellular(float x, float y, float z);
	float SingleCellular2Edge(float x, float y, float z);

	//4D
	float SingleSimplex(float x, float y, float z, float w);
};

#endif

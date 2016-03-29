#pragma once

class FastNoise
{
public:
	FastNoise(int seed = 0) { m_seed = seed; }
	~FastNoise() { delete m_cellularNoiseLookup; }

	enum NoiseType { Value, ValueFractal, Gradient, GradientFractal, Simplex, SimplexFractal, Cellular, CellularHQ, CellularCaves, WhiteNoise };
	enum Interp { InterpLinear = 0, InterpHermite = 1, InterpQuintic = 2 };
	enum FractalType { FBM, Billow, RigidMulti };
	enum CellularDistanceFunction { Ecludian, Manhattan, Natural };
	enum CellularReturnType { CellValue, NoiseLookup, Distance2Center, Distance2CenterXValue, Distance2CenterSq, Distance2CenterSqXValue, Distance2Edge, Distance2EdgeXValue, Distance2EdgeSq, Distance2EdgeSqXValue };

	void SetSeed(int seed) { m_seed = seed; }
	int GetSeed(void) { return m_seed; }
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
	float GetValue(float x, float y, float z);			// 16 ms	14 ms
	float GetValueFractal(float x, float y, float z);	// 56 ms	45 ms

	float GetGradient(float x, float y, float z);		// 23 ms	21 ms
	float GetGradientFractal(float x, float y, float z);// 79 ms	73 ms

	float GetSimplex(float x, float y, float z);		// 30 ms	30 ms
	float GetSimplexFractal(float x, float y, float z);	// 105 ms	102 ms

	float GetCellular(float x, float y, float z);		// 111 ms	107 ms
	float GetCellularHQ(float x, float y, float z);		// 415 ms	430 ms
	float GetCellularCaves(float x, float y, float z);	// 196 ms	185 ms

	float GetWhiteNoise(float x, float y, float z);
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
	float GetCellularHQ(float x, float y);				// 96 ms	90 ms

	float GetWhiteNoise(float x, float y);
	float GetWhiteNoiseInt(int x, int y);

	float GetNoise(float x, float y);

	//4D
	float GetSimplex(float x, float y, float z, float w);

	float GetWhiteNoise(float x, float y, float z, float w);
	float GetWhiteNoiseInt(int x, int y, int z, int w);

protected:
	int m_seed = 0;
	float m_frequency = 0.01f;
	Interp m_interp = InterpQuintic;
	NoiseType m_noiseType = Value;

	unsigned int m_octaves = 3;
	float m_lacunarity = 2.0f;
	float m_gain = 0.5f;
	FractalType m_fractalType = FBM;

	CellularDistanceFunction m_cellularDistanceFunction = Ecludian;
	CellularReturnType m_cellularReturnType = CellValue;
	FastNoise* m_cellularNoiseLookup = nullptr;
	
	int m_caveSpacing = 3;
	float m_caveAreaFrequency = 0.1f;
	float m_caveAreaThreshold = 0.0f;
	float m_caveCavernThreshold = 0.92f;
	float m_caveTurbulenceAmp = 15.0f;
	float m_caveTurbulenceFrequency = 0.05f;

	//3D
	float _ValueFractalFBM(float x, float y, float z);
	float _ValueFractalBillow(float x, float y, float z);
	float _ValueFractalRigidMulti(float x, float y, float z);
	float _Value(float x, float y, float z);

	float _GradientFractalFBM(float x, float y, float z);
	float _GradientFractalBillow(float x, float y, float z);
	float _GradientFractalRigidMulti(float x, float y, float z);
	float _Gradient(float x, float y, float z);

	float _SimplexFractalFBM(float x, float y, float z);
	float _SimplexFractalBillow(float x, float y, float z);
	float _SimplexFractalRigidMulti(float x, float y, float z);
	float _Simplex(float x, float y, float z);

	float _Cellular(float x, float y, float z);
	float _CellularHQ(float x, float y, float z);
	float _Cellular2Edge(float x, float y, float z);
	float _Cellular2EdgeHQ(float x, float y, float z);

	inline int CoordLUTIndex(int x, int y, int z);
	inline float GetValCoord(int x, int y, int z);
	inline float GetGradCoord(int xi, int yi, int zi, float x, float y, float z);

	//Caves
	float _CellularCaves(float x, float y, float z);
	void _GradientVector(float* vec, float x, float y, float z);
	bool _IsCaveCell(int x, int y, int z);

	//2D
	float _ValueFractalFBM(float x, float y);
	float _ValueFractalBillow(float x, float y);
	float _ValueFractalRigidMulti(float x, float y);
	float _Value(float x, float y);

	float _GradientFractalFBM(float x, float y);
	float _GradientFractalBillow(float x, float y);
	float _GradientFractalRigidMulti(float x, float y);
	float _Gradient(float x, float y);

	float _SimplexFractalFBM(float x, float y);
	float _SimplexFractalBillow(float x, float y);
	float _SimplexFractalRigidMulti(float x, float y);
	float _Simplex(float x, float y);

	float _Cellular(float x, float y);
	float _CellularHQ(float x, float y);
	float _Cellular2Edge(float x, float y);
	float _Cellular2EdgeHQ(float x, float y);
	
	inline int CoordLUTIndex(int x, int y);
	inline float GetValCoord(int x, int y);
	inline float GetGradCoord(int xi, int yi, float x, float y);

	//4D
	float _Simplex(float x, float y, float z, float w);
	inline int CoordLUTIndex(int x, int y, int z, int w);
};


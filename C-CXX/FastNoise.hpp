/*
 * Header only C++ wrapper around C FastNoise Lite implementation.
 * This is easy to use if you don't want templating.
 * 
 * NOTE: Untested
 */

#include "FastNoise.h"

// Comment if you don't want to use enum classes
#define ENABLE_ENUM_CLASS

namespace FastNoise {
    #if defined(ENABLE_ENUM_CLASS)
    enum class NoiseType {
        Value = FN_NOISE_VALUE,
        ValueCubic = FN_NOISE_VALUE_CUBIC,
        Perlin = FN_NOISE_PERLIN,
        Simplex = FN_NOISE_SIMPLEX,
        OpenSimplex2f = FN_NOISE_OPENSIMPLEX2F,
        Cellular = FN_NOISE_CELLULAR,
        WhiteNoise = FN_NOISE_WHITENOISE
    };

    enum class FractalType {
        None = FN_FRACTAL_NONE,
        FBM = FN_FRACTAL_FBM,
        Billow = FN_FRACTAL_BILLOW,
        Ridged = FN_FRACTAL_RIDGED
    };

    enum class CellularDistanceFunc {
        Euclidean = FN_CELLULAR_DIST_EUCLIDEAN,
        Manhattan = FN_CELLULAR_DIST_MANHATTAN,
        Natural = FN_CELLULAR_DIST_NATURAL
    };

    enum class CellularReturnType {
        CellValue = FN_CELLULAR_RET_CELLVALUE,
        NoiseLookup = FN_CELLULAR_RET_NOISE_LOOKUP,
        Distance = FN_CELLULAR_RET_DISTANCE,
        Distance2 = FN_CELLULAR_RET_DISTANCE2,
        Distance2Add = FN_CELLULAR_RET_DISTANCE2ADD,
        Distance2Sub = FN_CELLULAR_RET_DISTANCE2SUB,
        Distance2Mul = FN_CELLULAR_RET_DISTANCE2MUL,
        Distance2Div = FN_CELLULAR_RET_DISTANCE2DIV,
    };
    #endif

    class FastNoise {
        public:
            FastNoise();
            FastNoise(int seed);

            int getSeed();
            void setSeed(int seed);
            void setFrequency(FN_DECIMAL frequency);
            #if defined(ENABLE_ENUM_CLASS)
            void setNoiseType(NoiseType type);
            #endif
            void setNoiseType(fn_noise_type type);
            void setFractalOctaves(int octaves);
            void setFractalLacunarity(FN_DECIMAL lacunarity);
            void setFractalGain(FN_DECIMAL gain);
            #if defined(ENABLE_ENUM_CLASS)
            void setFractalType(FractalType type);
            void setCellularDistanceFunction(CellularDistanceFunc func);
            void setCellularReturnType(CellularReturnType type);
            #endif
            void setFractalType(fn_fractal_type type);
            void setCellularDistanceFunction(fn_cellular_distance_func func);
            void setCellularReturnType(fn_cellular_return_type type);
            void setCellularJitter(FN_DECIMAL jitter);
            void setCellularNoiseLookup(const FastNoise &noise);
            void setDomainWarpAmp(FN_DECIMAL gradientPerturbAmp);

            FN_DECIMAL getNoise(FN_DECIMAL x, FN_DECIMAL y);
            FN_DECIMAL getNoise(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);
        private:
            fn_state state;
            fn_state *statePtr;
    };

#if defined(FN_IMPL)

FastNoise::FastNoise() { state = fnCreateState(); statePtr = &state; }
FastNoise::FastNoise(int seed) { state = fnCreateState(seed); statePtr = &state; }

int FastNoise::getSeed() { return state.seed; }
void FastNoise::setSeed(int seed) { state.seed = seed; }
void FastNoise::setFrequency(FN_DECIMAL frequency) { state.frequency = frequency; }
#if defined(ENABLE_ENUM_CLASS)
void FastNoise::setNoiseType(NoiseType type) { state.noise_type = (fn_noise_type) type; }
#endif
void FastNoise::setNoiseType(fn_noise_type type) { state.noise_type = type; }
void FastNoise::setFractalOctaves(int octaves) { state.octaves = octaves; fnValidateState(&state); }
void FastNoise::setFractalLacunarity(FN_DECIMAL lacunarity) { state.lacunarity = lacunarity; }
void FastNoise::setFractalGain(FN_DECIMAL gain) { state.gain = gain; fnValidateState(&state); }
#if defined(ENABLE_ENUM_CLASS)
void FastNoise::setFractalType(FractalType type) { state.fractal_type = (fn_fractal_type) type; }
void FastNoise::setCellularDistanceFunction(CellularDistanceFunc func) { state.cellular_distance_func = (fn_cellular_distance_func) func; }
void FastNoise::setCellularReturnType(CellularReturnType type) { state.cellular_return_type = (fn_cellular_return_type) type; }
#endif
void FastNoise::setFractalType(fn_fractal_type type) { state.fractal_type = type; }
void FastNoise::setCellularDistanceFunction(fn_cellular_distance_func func) { state.cellular_distance_func = func; }
void FastNoise::setCellularReturnType(fn_cellular_return_type type) { state.cellular_return_type = type; }
void FastNoise::setCellularJitter(FN_DECIMAL jitter) { state.cellular_jitter_mod = jitter; }
void FastNoise::setCellularNoiseLookup(const FastNoise &noise) { state.cellular_lookup_state = noise.statePtr; }
void FastNoise::setDomainWarpAmp(FN_DECIMAL gradientPerturbAmp) { state.domain_warp_amp = gradientPerturbAmp; }

FN_DECIMAL FastNoise::getNoise(FN_DECIMAL x, FN_DECIMAL y) { return fnGetNoise(statePtr, x, y); }
FN_DECIMAL FastNoise::getNoise(FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z) { return fnGetNoise(statePtr, x, y, z); }

#endif
}
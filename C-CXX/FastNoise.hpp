/*
 * Header only C++ wrapper around C FastNoise Lite implementation.
 * This is easy to use if you don't want templating.
 * 
 * NOTE: Untested
 */

#include "FastNoise.h"

namespace FastNoise {
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
        Ridged = FN_FRACTAL_RIDGED,
        DomainWarpProgressive = FN_FRACTAL_DOMAIN_WARP_PROGRESSIVE,
        DomainWarpIndependant = FN_FRACTAL_DOMAIN_WARP_INDEPENDANT
    };

    enum class CellularDistanceFunc {
        Euclidean = FN_CELLULAR_DIST_EUCLIDEAN,
        Manhattan = FN_CELLULAR_DIST_MANHATTAN,
        Natural = FN_CELLULAR_DIST_NATURAL
    };

    enum class CellularReturnType {
        CellValue = FN_CELLULAR_RET_CELLVALUE,
        Distance = FN_CELLULAR_RET_DISTANCE,
        Distance2 = FN_CELLULAR_RET_DISTANCE2,
        Distance2Add = FN_CELLULAR_RET_DISTANCE2ADD,
        Distance2Sub = FN_CELLULAR_RET_DISTANCE2SUB,
        Distance2Mul = FN_CELLULAR_RET_DISTANCE2MUL,
        Distance2Div = FN_CELLULAR_RET_DISTANCE2DIV,
    };

    enum class DomainWarpType {
        Gradient = FN_DOMAIN_WARP_GRADIENT,
        Simplex = FN_DOMAIN_WARP_SIMPLEX
    };

    class FastNoise {
        public:
            FastNoise();
            FastNoise(int seed);

            int getSeed();
            void setSeed(int seed);
            void setFrequency(FNfloat frequency);
            void setNoiseType(NoiseType type);

            void setFractalOctaves(int octaves);
            void setFractalLacunarity(FNfloat lacunarity);
            void setFractalGain(FNfloat gain);
            void setFractalType(FractalType type);

            void setCellularDistanceFunction(CellularDistanceFunc func);
            void setCellularReturnType(CellularReturnType type);
            void setCellularJitter(FNfloat jitter);

            void setDomainWarpType(DomainWarpType type);
            void setDomainWarpAmp(FNfloat gradientPerturbAmp);

            FNfloat getNoise(FNfloat x, FNfloat y);
            FNfloat getNoise(FNfloat x, FNfloat y, FNfloat z);

            void domainWarp(FNfloat *x, FNfloat *y);
            void domainWarp(FNfloat *x, FNfloat *y, FNfloat *z);
        private:
            fn_state state;
    };

#if defined(FN_IMPL)

FastNoise::FastNoise() { state = fnCreateState(1337); }
FastNoise::FastNoise(int seed) { state = fnCreateState(seed); }

int FastNoise::getSeed() { return state.seed; }
void FastNoise::setSeed(int seed) { state.seed = seed; }
void FastNoise::setFrequency(FNfloat frequency) { state.frequency = frequency; }
void FastNoise::setNoiseType(NoiseType type) { state.noise_type = (fn_noise_type) type; }

void FastNoise::setFractalOctaves(int octaves) { state.octaves = octaves; }
void FastNoise::setFractalLacunarity(FNfloat lacunarity) { state.lacunarity = lacunarity; }
void FastNoise::setFractalGain(FNfloat gain) { state.gain = gain; }
void FastNoise::setFractalType(FractalType type) { state.fractal_type = (fn_fractal_type) type; }

void FastNoise::setCellularDistanceFunction(CellularDistanceFunc func) { state.cellular_distance_func = (fn_cellular_distance_func) func; }
void FastNoise::setCellularReturnType(CellularReturnType type) { state.cellular_return_type = (fn_cellular_return_type) type; }
void FastNoise::setCellularJitter(FNfloat jitter) { state.cellular_jitter_mod = jitter; }

void FastNoise::setDomainWarpType(DomainWarpType type) { state.domain_warp_type = (fn_domain_warp_type) type; }
void FastNoise::setDomainWarpAmp(FNfloat gradientPerturbAmp) { state.domain_warp_amp = gradientPerturbAmp; }

FNfloat FastNoise::getNoise(FNfloat x, FNfloat y) { return fnGetNoise2D(&state, x, y); }
FNfloat FastNoise::getNoise(FNfloat x, FNfloat y, FNfloat z) { return fnGetNoise3D(&state, x, y, z); }

void FastNoise::domainWarp(FNfloat *x, FNfloat *y) { return fnDomainWarp2D(&state, x, y); }
void FastNoise::domainWarp(FNfloat *x, FNfloat *y, FNfloat *z) { return fnDomainWarp3D(&state, x, y, z); }

#endif
}
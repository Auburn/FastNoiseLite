/*
 * C Header-Only FastNoise Lite Implementation.
 */

#include <math.h>

// Types
#define FN_DECIMAL float

// So that VS code doesnt hide my work :(
#define FN_IMPL

// Enums
typedef enum {
    FN_INTERP_LINEAR,
    FN_INTERP_HERMITE,
    FN_INTERP_QUINTIC
} fn_interp;

typedef enum {
    FN_NOISE_VALUE,
    FN_NOISE_VALUE_CUBIC,
    FN_NOISE_PERLIN,
    FN_NOISE_SIMPLEX,
    FN_NOISE_OPENSIMPLEX2F,
    FN_NOISE_CELLULAR,
    FN_NOISE_WHITENOISE
} fn_noise_type;

typedef enum {
    FN_FRACTAL_NONE,
    FN_FRACTAL_FBM,
    FN_FRACTAL_BILLOW,
    FN_FRACTAL_RIDGED
} fn_fractal_type;

typedef enum {
    FN_CELLULAR_DIST_EUCLIDEAN,
    FN_CELLULAR_DIST_MANHATTAN,
    FN_CELLULAR_DIST_NATURAL
} fn_cellular_distance_func;

typedef enum {
    FN_CELLULAR_RET_CELLVALUE,
    FN_CELLULAR_RET_NOISE_LOOKUP,
    FN_CELLULAR_RET_DISTANCE,
    FN_CELLULAR_RET_DISTANCE2,
    FN_CELLULAR_RET_DISTANCE2ADD,
    FN_CELLULAR_RET_DISTANCE2SUB,
    FN_CELLULAR_RET_DISTANCE2MUL,
    FN_CELLULAR_RET_DISTANCE2DIV,
} fn_cellular_return_type;

// State struct
typedef struct fn_state {
    int seed;
    FN_DECIMAL frequency;
    fn_interp interpolation;
    fn_noise_type noise_type;

    int octaves;
    FN_DECIMAL lacunarity;
    FN_DECIMAL gain;
    fn_fractal_type fractal_type;
    
    fn_cellular_distance_func cellularDistanceFunc;
    fn_cellular_return_type cellularReturnType;
    FN_DECIMAL cellularJitterMod;
    struct fn_state *cellularLookupState;

    // Calculated in the necessary set calls, or in validate state.
    FN_DECIMAL _fractalBounding;
} fn_state;

// Note for HLSL, instead of using pointers, use the inout input modifier.
// Call after every state change.
void fnValidateState(fn_state *state);

// Simple function for getting the noise
FN_DECIMAL fnGetNoise(fn_state *state, FN_DECIMAL x, FN_DECIMAL y);
FN_DECIMAL fnGetNoise(fn_state *state, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z);

#if defined(FN_IMPL)

// ====================
// Private API
// ====================

static void _fnCalculateFractalBounding(fn_state *state) {
    FN_DECIMAL amp = state->gain;
    FN_DECIMAL ampFractal = 1;
    for (int i = 0; i < state->octaves; i++) {
        ampFractal += amp;
        amp *= state->gain;
    }
    state->_fractalBounding = 1 / ampFractal;
}

// ====================
// Utilities
// ====================

static int _fnFastFloor(FN_DECIMAL f) { return (f >= 0 ? (int)f : (int)f - 1); }

static int _fnFastRound(FN_DECIMAL f) { return (f >= 0) ? (int)(f + 0.5f) : (int)(f - 0.5f); }

static FN_DECIMAL _fnLerp(FN_DECIMAL a, FN_DECIMAL b, FN_DECIMAL t) { return a + t * (b - a); }

// TODO: Check this with Auburn
static FN_DECIMAL _fnInterpHermite(FN_DECIMAL t) { return t * t * (3 - 2 * t); }

static FN_DECIMAL _fnInterpQuintic(FN_DECIMAL t) { return t * t * t * (t * (t * 6 - 15) + 10); }

static FN_DECIMAL _fnCubicLerp(FN_DECIMAL a, FN_DECIMAL b, FN_DECIMAL c, FN_DECIMAL d, FN_DECIMAL t) {
    float p = (d - c) - (a - b);
    return t * t * t * p + t * t * ((a - b) - p) + t * (c - a) + b;
}

// ====================
// Hashing
// ====================

static const int PRIME_X = 1619;
static const int PRIME_Y = 31337;
static const int PRIME_Z = 6971;

static int _fnHash(int seed, int xPrimed, int yPrimed) {
    int hash = seed ^ xPrimed ^ yPrimed;

    hash *= 0x27d4eb2d;
    hash ^= hash >> 15;
    return hash;
}

static int _fnHash(int seed, int xPrimed, int yPrimed, int zPrimed) {
    int hash = seed ^ xPrimed ^ yPrimed ^ zPrimed;

    hash *= 0x27d4eb2d;
    hash ^= hash >> 15;
    return hash;
}

static float _fnValCoord(int seed, int xPrimed, int yPrimed) {
    int hash = seed ^ xPrimed ^ yPrimed;

    hash *= 0x27d4eb2d;
    return hash / 2147483648.0f;
}

static float _fnValCoord(int seed, int xPrimed, int yPrimed, int zPrimed) {
    int hash = seed ^ xPrimed ^ yPrimed ^ zPrimed;

    hash *= 0x27d4eb2d;
    return hash / 2147483648.0f;
}

static const FN_DECIMAL ROOT2 = (FN_DECIMAL) 1.4142135623730950488f;

static const FN_DECIMAL GRAD_2D_X[] = {
    1 + ROOT2, -1-ROOT2,   1+ROOT2, -1-ROOT2,
    1,          1,        -1        -1
};

static const FN_DECIMAL GRAD_2D_Y[] = {
    1,          1,        -1,       -1,
    1 + ROOT2, -1-ROOT2,   1+ROOT2, -1-ROOT2
};

static FN_DECIMAL _fnGradCoord(int seed, int xPrimed, int yPrimed, FN_DECIMAL xd, FN_DECIMAL yd) {
    int idx = _fnHash(seed, xPrimed, yPrimed) & 7;
    FN_DECIMAL gx = GRAD_2D_X[idx];
    FN_DECIMAL gy = GRAD_2D_Y[idx];

    return xd * gx + yd * gy;
}

static const FN_DECIMAL GRAD_3D_X[] = {
    1, -1, 1, -1,
    1, -1, 1, -1,
    0, 0, 0, 0
};

static const FN_DECIMAL GRAD_3D_Y[] = {
	1, 1, -1, -1,
	0, 0, 0, 0,
	1, -1, 1, -1
};

static const FN_DECIMAL GRAD_3D_Z[] = {
	0, 0, 0, 0,
	1, 1, -1, -1,
	1, 1, -1, -1
};

static FN_DECIMAL _fnGradCoord(int seed, int xPrimed, int yPrimed, int zPrimed, FN_DECIMAL xd, FN_DECIMAL yd, FN_DECIMAL zd) {
    int idx = _fnHash(seed, xPrimed, yPrimed, zPrimed) & 15;
    FN_DECIMAL gx = GRAD_3D_X[idx];
    FN_DECIMAL gy = GRAD_3D_Y[idx];
    FN_DECIMAL gz = GRAD_3D_Z[idx];

    return xd * gx + yd * gy + zd * gz;
}

// ====================
// Noise functions
// ====================

// White Noise

static int _fnFloatCast2Int(FN_DECIMAL f) {
    // TODO: Test
    long i = *(long*)(void*)&f;
    return (int)(i ^ (i >> 32));
}

static float _fnSingleWhiteNoise(int seed, FN_DECIMAL x, FN_DECIMAL y) {
    int xi = _fnFloatCast2Int(x) * PRIME_X;
    int yi = _fnFloatCast2Int(y) * PRIME_Y;
    return _fnValCoord(seed, xi, yi);
}

static float _fnSingleWhiteNoise(int seed, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z) {
    int xi = _fnFloatCast2Int(x) * PRIME_X;
    int yi = _fnFloatCast2Int(y) * PRIME_Y;
    int zi = _fnFloatCast2Int(z) * PRIME_Z;
    return _fnValCoord(seed, xi, yi, zi);
}

// Value noise

static FN_DECIMAL _fnSingleValue(fn_state *state, int seed, FN_DECIMAL x, FN_DECIMAL y) {
    int x0 = _fnFastFloor(x);
    int y0 = _fnFastFloor(y);

    FN_DECIMAL xs, ys;
    switch (state->interpolation) {
        default:
        case FN_INTERP_LINEAR:
            xs = x - x0;
            ys = y - y0;
            break;
        case FN_INTERP_HERMITE:
            xs = _fnInterpHermite((FN_DECIMAL)(x - x0));
            ys = _fnInterpHermite((FN_DECIMAL)(y - y0));
            break;
        case FN_INTERP_QUINTIC:
            xs = _fnInterpQuintic((FN_DECIMAL)(x - x0));
            ys = _fnInterpQuintic((FN_DECIMAL)(y - y0));
            break;
    }

    x0 *= PRIME_X;
    y0 *= PRIME_Y;
    int x1 = x0 + PRIME_X;
    int y1 = y0 + PRIME_Y;

    FN_DECIMAL xf0 = _fnLerp(_fnValCoord(seed, x0, y0), _fnValCoord(seed, x1, y0), xs);
    FN_DECIMAL xf1 = _fnLerp(_fnValCoord(seed, x0, y1), _fnValCoord(seed, x1, y1), xs);

    return _fnLerp(xf0, xf1, ys);
}

static FN_DECIMAL _fnSingleValue(fn_state *state, int seed, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z) {
    int x0 = _fnFastFloor(x);
    int y0 = _fnFastFloor(y);
    int z0 = _fnFastFloor(z);

    FN_DECIMAL xs, ys, zs;
    switch (state->interpolation) {
        default:
        case FN_INTERP_LINEAR:
            xs = x - x0;
            ys = y - y0;
            zs = z - z0;
            break;
        case FN_INTERP_HERMITE:
            xs = _fnInterpHermite((FN_DECIMAL)(x - x0));
            ys = _fnInterpHermite((FN_DECIMAL)(y - y0));
            zs = _fnInterpHermite((FN_DECIMAL)(z - z0));
            break;
        case FN_INTERP_QUINTIC:
            xs = _fnInterpQuintic((FN_DECIMAL)(x - x0));
            ys = _fnInterpQuintic((FN_DECIMAL)(y - y0));
            zs = _fnInterpQuintic((FN_DECIMAL)(z - z0));
            break;
    }

    x0 *= PRIME_X;
    y0 *= PRIME_Y;
    z0 *= PRIME_Z;
    int x1 = x0 + PRIME_X;
    int y1 = y0 + PRIME_Y;
    int z1 = z0 + PRIME_Z;

    float xf00 = _fnLerp(_fnValCoord(seed, x0, y0, z0), _fnValCoord(seed, x1, y0, z0), xs);
    float xf10 = _fnLerp(_fnValCoord(seed, x0, y1, z0), _fnValCoord(seed, x1, y1, z0), xs);
    float xf01 = _fnLerp(_fnValCoord(seed, x0, y0, z1), _fnValCoord(seed, x1, y0, z1), xs);
    float xf11 = _fnLerp(_fnValCoord(seed, x0, y1, z1), _fnValCoord(seed, x1, y1, z1), xs);
        
    float yf0 = _fnLerp(xf00, xf10, ys);
    float yf1 = _fnLerp(xf01, xf11, ys);

    return _fnLerp(yf0, yf1, zs);
}

// Perlin Noise

static float _fnSinglePerlin(fn_state *state, int seed, FN_DECIMAL x, FN_DECIMAL y) {
    int x0 = _fnFastFloor(x);
    int y0 = _fnFastFloor(y);

    FN_DECIMAL xd0 = (FN_DECIMAL) (x - x0);
    FN_DECIMAL yd0 = (FN_DECIMAL) (y - y0);
    FN_DECIMAL xd1 = xd0 - 1;
    FN_DECIMAL yd1 = yd0 - 1;

    FN_DECIMAL xs, ys;
    switch (state->interpolation) {
        default:
        case FN_INTERP_LINEAR:
            xs = xd0;
            ys = yd0;
            break;
        case FN_INTERP_HERMITE:
            xs = _fnInterpHermite(xd0);
            ys = _fnInterpHermite(yd0);
            break;
        case FN_INTERP_QUINTIC:
            xs = _fnInterpQuintic(xd0);
            ys = _fnInterpQuintic(yd0);
            break;
    }

    x0 *= PRIME_X;
    y0 *= PRIME_Y;
    int x1 = x0 + PRIME_X;
    int y1 = y0 + PRIME_Y;

    FN_DECIMAL xf0 = _fnLerp(_fnGradCoord(seed, x0, y0, xd0, yd0), _fnGradCoord(seed, x1, y0, xd1, yd0), xs);
    FN_DECIMAL xf1 = _fnLerp(_fnGradCoord(seed, x0, y1, xd0, yd1), _fnGradCoord(seed, x1, y1, xd1, yd1), xs);

    return _fnLerp(xf0, xf1, ys) * (FN_DECIMAL) 0.579106986522674560546875f;
}

static float _fnSinglePerlin(fn_state *state, int seed, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z) {
    int x0 = _fnFastFloor(x);
    int y0 = _fnFastFloor(y);
    int z0 = _fnFastFloor(z);

    FN_DECIMAL xd0 = (FN_DECIMAL) (x - x0);
    FN_DECIMAL yd0 = (FN_DECIMAL) (y - y0);
    FN_DECIMAL zd0 = (FN_DECIMAL) (z - z0);
    FN_DECIMAL xd1 = xd0 - 1;
    FN_DECIMAL yd1 = yd0 - 1;
    FN_DECIMAL zd1 = zd0 - 1;

    FN_DECIMAL xs, ys, zs;
    switch (state->interpolation) {
        default:
        case FN_INTERP_LINEAR:
            xs = xd0;
            ys = yd0;
            zs = zd0;
            break;
        case FN_INTERP_HERMITE:
            xs = _fnInterpHermite(xd0);
            ys = _fnInterpHermite(yd0);
            zs = _fnInterpHermite(yd0);
            break;
        case FN_INTERP_QUINTIC:
            xs = _fnInterpQuintic(xd0);
            ys = _fnInterpQuintic(yd0);
            zs = _fnInterpQuintic(zd0);
            break;
    }

    x0 *= PRIME_X;
    y0 *= PRIME_Y;
    z0 *= PRIME_Z;
    int x1 = x0 + PRIME_X;
    int y1 = y0 + PRIME_Y;
    int z1 = z0 + PRIME_Z;

    FN_DECIMAL xf00 = _fnLerp(_fnGradCoord(seed, x0, y0, z0, xd0, yd0, zd0), _fnGradCoord(seed, x1, y0, z0, xd1, yd0, zd0), xs);
    FN_DECIMAL xf10 = _fnLerp(_fnGradCoord(seed, x0, y1, z0, xd0, yd1, zd0), _fnGradCoord(seed, x1, y1, z0, xd1, yd1, zd0), xs);
    FN_DECIMAL xf01 = _fnLerp(_fnGradCoord(seed, x0, y0, z1, xd0, yd0, zd1), _fnGradCoord(seed, x1, y0, z1, xd1, yd0, zd1), xs);
    FN_DECIMAL xf11 = _fnLerp(_fnGradCoord(seed, x0, y1, z1, xd0, yd1, zd1), _fnGradCoord(seed, x1, y1, z1, xd1, yd1, zd1), xs);
         
    FN_DECIMAL yf0 = _fnLerp(xf00, xf10, ys);
    FN_DECIMAL yf1 = _fnLerp(xf01, xf11, ys);

    return _fnLerp(yf0, yf1, zs) * (FN_DECIMAL) 0.964921414852142333984375;
}

// ====================
// 2D Gen functions
// ====================

// DEV NOTE: When porting to HLSL, have an overload for an array of states, when cellular lookup is required, we look for an adjacent state

static FN_DECIMAL _fnGenNoiseSingle(fn_state *state, int seed, FN_DECIMAL x, FN_DECIMAL y) {
    switch (state->noise_type) {
        case FN_NOISE_VALUE:
            return _fnSingleValue(state, seed, x, y);
        // case FN_NOISE_VALUE_CUBIC:
        //     return _fnSingleValueCubic(state, seed, x, y);
        case FN_NOISE_PERLIN:
            return _fnSinglePerlin(state, seed, x, y);
        // case FN_NOISE_SIMPLEX:
        //     return _fnSingleSimplex(state, seed, x, y);
        // case FN_NOISE_OPENSIMPLEX2F:
        //     return 0; // TODO
        // case FN_NOISE_CELLULAR:
        //     return _fnSingleCellular(state, seed, x, y);
        case FN_NOISE_WHITENOISE:
            return _fnSingleWhiteNoise(seed, x, y);
        default:
            return 0;
    }
}

static FN_DECIMAL _fnGenFractalFBM(fn_state *state, FN_DECIMAL x, FN_DECIMAL y) {
    int seed = state->seed;
    FN_DECIMAL sum = _fnGenNoiseSingle(state, seed, x, y);
    FN_DECIMAL amp = (FN_DECIMAL) 1.0;

    for (int i = 1; i < state->octaves; i++) {
        x *= state->lacunarity;
        y *= state->lacunarity;

        amp *= state->gain;
        sum += _fnGenNoiseSingle(state, ++seed, x, y) * amp;
    }

    return sum * state->_fractalBounding;
}

static FN_DECIMAL _fnGenFractalBillow(fn_state *state, FN_DECIMAL x, FN_DECIMAL y) {
    int seed = state->seed;
    FN_DECIMAL sum = fabs(_fnGenNoiseSingle(state, seed, x, y)) * 2 - 1;
    FN_DECIMAL amp = (FN_DECIMAL) 1.0;

    for (int i = 1; i < state->octaves; i++) {
        x *= state->lacunarity;
        y *= state->lacunarity;

        amp *= state->gain;
        sum += (fabs(_fnGenNoiseSingle(state, ++seed, x, y)) * 2 - 1) * amp;
    }

    return sum * state->_fractalBounding;
}

static FN_DECIMAL _fnGenFractalRidged(fn_state *state, FN_DECIMAL x, FN_DECIMAL y) {
    int seed = state->seed;
    FN_DECIMAL sum = 1 - fabs(_fnGenNoiseSingle(state, seed, x, y));
    FN_DECIMAL amp = (FN_DECIMAL) 1.0;

    for (int i = 1; i < state->octaves; i++) {
        x *= state->lacunarity;
        y *= state->lacunarity;

        amp *= state->gain;
        sum -= (1 - fabs(_fnGenNoiseSingle(state, ++seed, x, y))) * amp;
    }

    return sum;
}

// ====================
// 3D Gen functions
// ====================

static FN_DECIMAL _fnGenNoiseSingle(fn_state *state, int seed, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z) {
    switch (state->noise_type) {
        case FN_NOISE_VALUE:
            return _fnSingleValue(state, seed, x, y, z);
        // case FN_NOISE_VALUE_CUBIC:
        //     return _fnSingleValueCubic(state, seed, x, y, z);
        case FN_NOISE_PERLIN:
            return _fnSinglePerlin(state, seed, x, y, z);
        // case FN_NOISE_SIMPLEX:
        //     return _fnSingleSimplex(state, seed, x, y, z);
        // case FN_NOISE_OPENSIMPLEX2F:
        //     return 0; // TODO
        // case FN_NOISE_CELLULAR:
        //     return _fnSingleCellular(state, seed, x, y, z);
        // case FN_NOISE_WHITENOISE:
        //     return _fnSingleWhiteNoise(seed, x, y, z);
        default:
            return 0;
    }
}

static FN_DECIMAL _fnGenFractalFBM(fn_state *state, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z) {
    int seed = state->seed;
    FN_DECIMAL sum = _fnGenNoiseSingle(state, seed, x, y);
    FN_DECIMAL amp = (FN_DECIMAL) 1.0;

    for (int i = 1; i < state->octaves; i++) {
        x *= state->lacunarity;
        y *= state->lacunarity;
        z *= state->lacunarity;

        amp *= state->gain;
        sum += _fnGenNoiseSingle(state, ++seed, x, y, z) * amp;
    }

    return sum * state->_fractalBounding;
}

static FN_DECIMAL _fnGenFractalBillow(fn_state *state, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z) {
    int seed = state->seed;
    FN_DECIMAL sum = fabs(_fnGenNoiseSingle(state, seed, x, y, z)) * 2 - 1;
    FN_DECIMAL amp = (FN_DECIMAL) 1.0;

    for (int i = 1; i < state->octaves; i++) {
        x *= state->lacunarity;
        y *= state->lacunarity;
        z *= state->lacunarity;

        amp *= state->gain;
        sum += (fabs(_fnGenNoiseSingle(state, ++seed, x, y, z)) * 2 - 1) * amp;
    }

    return sum * state->_fractalBounding;
}

static FN_DECIMAL _fnGenFractalRidged(fn_state *state, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z) {
    int seed = state->seed;
    FN_DECIMAL sum = 1 - fabs(_fnGenNoiseSingle(state, seed, x, y, z));
    FN_DECIMAL amp = (FN_DECIMAL) 1.0;

    for (int i = 1; i < state->octaves; i++) {
        x *= state->lacunarity;
        y *= state->lacunarity;
        z *= state->lacunarity;

        amp *= state->gain;
        sum -= (1 - fabs(_fnGenNoiseSingle(state, ++seed, x, y, z))) * amp;
    }

    return sum;
}

// ====================
// Public API
// ====================

void fnValidateState(fn_state *state) { _fnCalculateFractalBounding(state); }

FN_DECIMAL fnGetNoise(fn_state *state, FN_DECIMAL x, FN_DECIMAL y) {
    x *= state->frequency;
    y *= state->frequency;

    switch (state->fractal_type) {
        case FN_FRACTAL_NONE:
            return _fnGenNoiseSingle(state, state->seed, x, y);
        case FN_FRACTAL_FBM:
            return _fnGenFractalFBM(state, x, y);
        case FN_FRACTAL_BILLOW:
            return _fnGenFractalBillow(state, x, y);
        case FN_FRACTAL_RIDGED:
            return _fnGenFractalRidged(state, x, y);
        default:
            return 0;
    }
}

FN_DECIMAL fnGetNoise(fn_state *state, FN_DECIMAL x, FN_DECIMAL y, FN_DECIMAL z) {
    x *= state->frequency;
    y *= state->frequency;
    z *= state->frequency;

    // Select a noise type
    switch (state->fractal_type) {
        case FN_FRACTAL_NONE:
            return _fnGenNoiseSingle(state, state->seed, x, y, z);
        case FN_FRACTAL_FBM:
            return _fnGenFractalFBM(state, x, y, z);
        case FN_FRACTAL_BILLOW:
            return _fnGenFractalBillow(state, x, y, z);
        case FN_FRACTAL_RIDGED:
            return _fnGenFractalRidged(state, x, y, z);
        default:
            return 0;
    }
}

#endif
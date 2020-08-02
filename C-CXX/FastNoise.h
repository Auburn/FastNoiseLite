// FastNoise.h
//
// MIT License
//
// Copyright(c) 2020 Jordan Peck, Reece Mackie
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

// Uncomment to use doubles for inputs
//#define FN_USE_DOUBLE

#if defined(__cplusplus)
extern "C" {
#endif

#include <math.h>
#include <stdint.h>
#include <float.h>

// Typedefs
#if defined(FN_USE_DOUBLE)
typedef double FNfloat;
#else
typedef float FNfloat;
#endif

// Enums
typedef enum {
    FN_NOISE_SIMPLEX,
    FN_NOISE_OPENSIMPLEX2,
    /* FN_NOISE_OPENSIMPLEX2S, */
    FN_NOISE_CELLULAR,
    FN_NOISE_PERLIN,
    FN_NOISE_VALUE_CUBIC,
    FN_NOISE_VALUE
} fn_noise_type;

typedef enum {
    FN_FRACTAL_NONE,
    FN_FRACTAL_FBM,
    FN_FRACTAL_RIDGED,
    FN_FRACTAL_PINGPONG,
    FN_FRACTAL_DOMAIN_WARP_PROGRESSIVE,
    FN_FRACTAL_DOMAIN_WARP_INDEPENDENT
} fn_fractal_type;

typedef enum {
    FN_CELLULAR_DIST_EUCLIDEAN,
    FN_CELLULAR_DIST_EUCLIDEANSQ,
    FN_CELLULAR_DIST_MANHATTAN,
    FN_CELLULAR_DIST_HYBRID
} fn_cellular_distance_func;

typedef enum {
    FN_CELLULAR_RET_CELLVALUE,
    FN_CELLULAR_RET_DISTANCE,
    FN_CELLULAR_RET_DISTANCE2,
    FN_CELLULAR_RET_DISTANCE2ADD,
    FN_CELLULAR_RET_DISTANCE2SUB,
    FN_CELLULAR_RET_DISTANCE2MUL,
    FN_CELLULAR_RET_DISTANCE2DIV,
} fn_cellular_return_type;

typedef enum {
    FN_DOMAIN_WARP_OPENSIMPLEX2,
    FN_DOMAIN_WARP_OPENSIMPLEX2_REDUCED,
    FN_DOMAIN_WARP_BASICGRID
} fn_domain_warp_type;

// State struct
typedef struct fn_state {
    int seed;
    float frequency;
    fn_noise_type noise_type;

    fn_fractal_type fractal_type;
    int octaves;
    float lacunarity;
    float gain;
    float weighted_strength;
    float ping_pong_strength;
    
    fn_cellular_distance_func cellular_distance_func;
    fn_cellular_return_type cellular_return_type;
    float cellular_jitter_mod;

    fn_domain_warp_type domain_warp_type;
    float domain_warp_amp;
} fn_state;

// Creates a state with default values set
fn_state fnCreateState(int seed = 1337);

// Simple function for getting the noise
float fnGetNoise2D(fn_state *state, FNfloat x, FNfloat y);
float fnGetNoise3D(fn_state *state, FNfloat x, FNfloat y, FNfloat z);

// Domain warp
void fnDomainWarp2D(fn_state *state, FNfloat *x, FNfloat *y);
void fnDomainWarp3D(fn_state *state, FNfloat *x, FNfloat *y, FNfloat *z);

// ====================
// Below this line is the implementation
// ====================

#if defined(FN_IMPL)

// ====================
// Utilities
// ====================

static inline float _fnFastMin(float x, float y) { return x < y ? x : y; }

static inline float _fnFastMax(float x, float y) { return x > y ? x : y; }

static inline float _fnFastAbs(float f) { return f < 0 ? -f : f; }

static inline float _fnCasti32Tof32(int i) {
    union {
        float f;
        int32_t i;
    } u;
    u.i = i;
    return u.f;
}

static inline int _fnCastf32Toi32(float f) {
    union {
        float f;
        int32_t i;
    } u;
    u.f = f;
    return u.i;
}

static inline float _fnInvSqrt(float a) {
    float xhalf = 0.5f * a;
    a = _fnCasti32Tof32(0x5f3759df - (_fnCastf32Toi32(a) >> 1));
    a = a * (1.5f - xhalf * a * a);
    return a;
}

// NOTE: If your language does not support this method (seen above), then simply use the native sqrt function.
static inline float _fnFastSqrt(float a) {
    return a * _fnInvSqrt(a);
}

static inline int _fnFastFloor(FNfloat f) { return (f >= 0 ? (int)f : (int)f - 1); }

static inline int _fnFastRound(FNfloat f) { return (f >= 0) ? (int)(f + 0.5f) : (int)(f - 0.5f); }

static inline float _fnLerp(float a, float b, float t) { return a + t * (b - a); }

static inline float _fnInterpHermite(float t) { return t * t * (3 - 2 * t); }

static inline float _fnInterpQuintic(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }

static inline float _fnCubicLerp(float a, float b, float c, float d, float t) {
    float p = (d - c) - (a - b);
    return t * t * t * p + t * t * ((a - b) - p) + t * (c - a) + b;
}

static inline float _fnPingPong(float t) {
    t -= (int)(t * 0.5f) * 2;
    return t < 1 ? t : 2 - t;
}

static float _fnCalculateFractalBounding(fn_state *state) {
    float gain = _fnFastAbs(state->gain);
    float amp = gain;
    float ampFractal = 1.0f;
    for (int i = 1; i < state->octaves; i++) {
        ampFractal += amp;
        amp *= gain;
    }
    return 1.0f / ampFractal;
}

// ====================
// Hashing
// ====================

static const int PRIME_X = 501125321;
static const int PRIME_Y = 1136930381;
static const int PRIME_Z = 1720413743;

static inline int _fnHash2D(int seed, int xPrimed, int yPrimed) {
    int hash = seed ^ xPrimed ^ yPrimed;

    hash *= 0x27d4eb2d;
    return hash;
}

static inline int _fnHash3D(int seed, int xPrimed, int yPrimed, int zPrimed) {
    int hash = seed ^ xPrimed ^ yPrimed ^ zPrimed;

    hash *= 0x27d4eb2d;
    return hash;
}

static inline float _fnValCoord2D(int seed, int xPrimed, int yPrimed) {
    int hash = _fnHash2D(seed, xPrimed, yPrimed);
    hash ^= hash << 19;
    return hash / 2147483648.0f;
}

static inline float _fnValCoord3D(int seed, int xPrimed, int yPrimed, int zPrimed) {
    int hash = _fnHash3D(seed, xPrimed, yPrimed, zPrimed);
    hash ^= hash << 19;
    return hash / 2147483648.0f;
}

static const float ROOT2 = 1.4142135623730950488f;

static const float GRADIENTS_2D[] = {
    1 + ROOT2, 1, -1 - ROOT2, 1, 1 + ROOT2, -1, -1 - ROOT2, -1,
    1, 1 + ROOT2, 1, -1 - ROOT2, -1, 1 + ROOT2, -1, -1 - ROOT2
};

static inline float _fnGradCoord2D(int seed, int xPrimed, int yPrimed, float xd, float yd) {
    int hash = _fnHash2D(seed, xPrimed, yPrimed);
    hash ^= hash >> 15;
    hash &= 7 << 1;
    return xd * GRADIENTS_2D[hash] + yd * GRADIENTS_2D[hash | 1];
}

static const float GRADIENTS_3D[] = {
    1, 1, 0, 0, -1, 1, 0, 0,  1,-1, 0, 0, -1,-1, 0, 0,
    1, 0, 1, 0, -1, 0, 1, 0,  1, 0,-1, 0, -1, 0,-1, 0,
    0, 1, 1, 0,  0,-1, 1, 0,  0, 1,-1, 0,  0,-1,-1, 0,
    1, 1, 0, 0,  0,-1, 1, 0, -1, 1, 0, 0,  0,-1,-1, 0
};

static inline float _fnGradCoord3D(int seed, int xPrimed, int yPrimed, int zPrimed, float xd, float yd, float zd) {
    int hash = _fnHash3D(seed, xPrimed, yPrimed, zPrimed);
    hash ^= hash >> 15;
    hash &= 15 << 2;
    return xd * GRADIENTS_3D[hash] + yd * GRADIENTS_3D[hash | 1] + zd * GRADIENTS_3D[hash | 2];
}

static const float RAND_VECS_2D[] = {
    -0.2700222198f, -0.9628540911f, 0.3863092627f, -0.9223693152f, 0.04444859006f, -0.999011673f, -0.5992523158f, -0.8005602176f, -0.7819280288f, 0.6233687174f, 0.9464672271f, 0.3227999196f, -0.6514146797f, -0.7587218957f, 0.9378472289f, 0.347048376f,
    -0.8497875957f, -0.5271252623f, -0.879042592f, 0.4767432447f, -0.892300288f, -0.4514423508f, -0.379844434f, -0.9250503802f, -0.9951650832f, 0.0982163789f, 0.7724397808f, -0.6350880136f, 0.7573283322f, -0.6530343002f, -0.9928004525f, -0.119780055f,
    -0.0532665713f, 0.9985803285f, 0.9754253726f, -0.2203300762f, -0.7665018163f, 0.6422421394f, 0.991636706f, 0.1290606184f, -0.994696838f, 0.1028503788f, -0.5379205513f, -0.84299554f, 0.5022815471f, -0.8647041387f, 0.4559821461f, -0.8899889226f,
    -0.8659131224f, -0.5001944266f, 0.0879458407f, -0.9961252577f, -0.5051684983f, 0.8630207346f, 0.7753185226f, -0.6315704146f, -0.6921944612f, 0.7217110418f, -0.5191659449f, -0.8546734591f, 0.8978622882f, -0.4402764035f, -0.1706774107f, 0.9853269617f,
    -0.9353430106f, -0.3537420705f, -0.9992404798f, 0.03896746794f, -0.2882064021f, -0.9575683108f, -0.9663811329f, 0.2571137995f, -0.8759714238f, -0.4823630009f, -0.8303123018f, -0.5572983775f, 0.05110133755f, -0.9986934731f, -0.8558373281f, -0.5172450752f,
    0.09887025282f, 0.9951003332f, 0.9189016087f, 0.3944867976f, -0.2439375892f, -0.9697909324f, -0.8121409387f, -0.5834613061f, -0.9910431363f, 0.1335421355f, 0.8492423985f, -0.5280031709f, -0.9717838994f, -0.2358729591f, 0.9949457207f, 0.1004142068f,
    0.6241065508f, -0.7813392434f, 0.662910307f, 0.7486988212f, -0.7197418176f, 0.6942418282f, -0.8143370775f, -0.5803922158f, 0.104521054f, -0.9945226741f, -0.1065926113f, -0.9943027784f, 0.445799684f, -0.8951327509f, 0.105547406f, 0.9944142724f,
    -0.992790267f, 0.1198644477f, -0.8334366408f, 0.552615025f, 0.9115561563f, -0.4111755999f, 0.8285544909f, -0.5599084351f, 0.7217097654f, -0.6921957921f, 0.4940492677f, -0.8694339084f, -0.3652321272f, -0.9309164803f, -0.9696606758f, 0.2444548501f,
    0.08925509731f, -0.996008799f, 0.5354071276f, -0.8445941083f, -0.1053576186f, 0.9944343981f, -0.9890284586f, 0.1477251101f, 0.004856104961f, 0.9999882091f, 0.9885598478f, 0.1508291331f, 0.9286129562f, -0.3710498316f, -0.5832393863f, -0.8123003252f,
    0.3015207509f, 0.9534596146f, -0.9575110528f, 0.2883965738f, 0.9715802154f, -0.2367105511f, 0.229981792f, 0.9731949318f, 0.955763816f, -0.2941352207f, 0.740956116f, 0.6715534485f, -0.9971513787f, -0.07542630764f, 0.6905710663f, -0.7232645452f,
    -0.290713703f, -0.9568100872f, 0.5912777791f, -0.8064679708f, -0.9454592212f, -0.325740481f, 0.6664455681f, 0.74555369f, 0.6236134912f, 0.7817328275f, 0.9126993851f, -0.4086316587f, -0.8191762011f, 0.5735419353f, -0.8812745759f, -0.4726046147f,
    0.9953313627f, 0.09651672651f, 0.9855650846f, -0.1692969699f, -0.8495980887f, 0.5274306472f, 0.6174853946f, -0.7865823463f, 0.8508156371f, 0.52546432f, 0.9985032451f, -0.05469249926f, 0.1971371563f, -0.9803759185f, 0.6607855748f, -0.7505747292f,
    -0.03097494063f, 0.9995201614f, -0.6731660801f, 0.739491331f, -0.7195018362f, -0.6944905383f, 0.9727511689f, 0.2318515979f, 0.9997059088f, -0.0242506907f, 0.4421787429f, -0.8969269532f, 0.9981350961f, -0.061043673f, -0.9173660799f, -0.3980445648f,
    -0.8150056635f, -0.5794529907f, -0.8789331304f, 0.4769450202f, 0.0158605829f, 0.999874213f, -0.8095464474f, 0.5870558317f, -0.9165898907f, -0.3998286786f, -0.8023542565f, 0.5968480938f, -0.5176737917f, 0.8555780767f, -0.8154407307f, -0.5788405779f,
    0.4022010347f, -0.9155513791f, -0.9052556868f, -0.4248672045f, 0.7317445619f, 0.6815789728f, -0.5647632201f, -0.8252529947f, -0.8403276335f, -0.5420788397f, -0.9314281527f, 0.363925262f, 0.5238198472f, 0.8518290719f, 0.7432803869f, -0.6689800195f,
    -0.985371561f, -0.1704197369f, 0.4601468731f, 0.88784281f, 0.825855404f, 0.5638819483f, 0.6182366099f, 0.7859920446f, 0.8331502863f, -0.553046653f, 0.1500307506f, 0.9886813308f, -0.662330369f, -0.7492119075f, -0.668598664f, 0.743623444f,
    0.7025606278f, 0.7116238924f, -0.5419389763f, -0.8404178401f, -0.3388616456f, 0.9408362159f, 0.8331530315f, 0.5530425174f, -0.2989720662f, -0.9542618632f, 0.2638522993f, 0.9645630949f, 0.124108739f, -0.9922686234f, -0.7282649308f, -0.6852956957f,
    0.6962500149f, 0.7177993569f, -0.9183535368f, 0.3957610156f, -0.6326102274f, -0.7744703352f, -0.9331891859f, -0.359385508f, -0.1153779357f, -0.9933216659f, 0.9514974788f, -0.3076565421f, -0.08987977445f, -0.9959526224f, 0.6678496916f, 0.7442961705f,
    0.7952400393f, -0.6062947138f, -0.6462007402f, -0.7631674805f, -0.2733598753f, 0.9619118351f, 0.9669590226f, -0.254931851f, -0.9792894595f, 0.2024651934f, -0.5369502995f, -0.8436138784f, -0.270036471f, -0.9628500944f, -0.6400277131f, 0.7683518247f,
    -0.7854537493f, -0.6189203566f, 0.06005905383f, -0.9981948257f, -0.02455770378f, 0.9996984141f, -0.65983623f, 0.751409442f, -0.6253894466f, -0.7803127835f, -0.6210408851f, -0.7837781695f, 0.8348888491f, 0.5504185768f, -0.1592275245f, 0.9872419133f,
    0.8367622488f, 0.5475663786f, -0.8675753916f, -0.4973056806f, -0.2022662628f, -0.9793305667f, 0.9399189937f, 0.3413975472f, 0.9877404807f, -0.1561049093f, -0.9034455656f, 0.4287028224f, 0.1269804218f, -0.9919052235f, -0.3819600854f, 0.924178821f,
    0.9754625894f, 0.2201652486f, -0.3204015856f, -0.9472818081f, -0.9874760884f, 0.1577687387f, 0.02535348474f, -0.9996785487f, 0.4835130794f, -0.8753371362f, -0.2850799925f, -0.9585037287f, -0.06805516006f, -0.99768156f, -0.7885244045f, -0.6150034663f,
    0.3185392127f, -0.9479096845f, 0.8880043089f, 0.4598351306f, 0.6476921488f, -0.7619021462f, 0.9820241299f, 0.1887554194f, 0.9357275128f, -0.3527237187f, -0.8894895414f, 0.4569555293f, 0.7922791302f, 0.6101588153f, 0.7483818261f, 0.6632681526f,
    -0.7288929755f, -0.6846276581f, 0.8729032783f, -0.4878932944f, 0.8288345784f, 0.5594937369f, 0.08074567077f, 0.9967347374f, 0.9799148216f, -0.1994165048f, -0.580730673f, -0.8140957471f, -0.4700049791f, -0.8826637636f, 0.2409492979f, 0.9705377045f,
    0.9437816757f, -0.3305694308f, -0.8927998638f, -0.4504535528f, -0.8069622304f, 0.5906030467f, 0.06258973166f, 0.9980393407f, -0.9312597469f, 0.3643559849f, 0.5777449785f, 0.8162173362f, -0.3360095855f, -0.941858566f, 0.697932075f, -0.7161639607f,
    -0.002008157227f, -0.9999979837f, -0.1827294312f, -0.9831632392f, -0.6523911722f, 0.7578824173f, -0.4302626911f, -0.9027037258f, -0.9985126289f, -0.05452091251f, -0.01028102172f, -0.9999471489f, -0.4946071129f, 0.8691166802f, -0.2999350194f, 0.9539596344f,
    0.8165471961f, 0.5772786819f, 0.2697460475f, 0.962931498f, -0.7306287391f, -0.6827749597f, -0.7590952064f, -0.6509796216f, -0.907053853f, 0.4210146171f, -0.5104861064f, -0.8598860013f, 0.8613350597f, 0.5080373165f, 0.5007881595f, -0.8655698812f,
    -0.654158152f, 0.7563577938f, -0.8382755311f, -0.545246856f, 0.6940070834f, 0.7199681717f, 0.06950936031f, 0.9975812994f, 0.1702942185f, -0.9853932612f, 0.2695973274f, 0.9629731466f, 0.5519612192f, -0.8338697815f, 0.225657487f, -0.9742067022f,
    0.4215262855f, -0.9068161835f, 0.4881873305f, -0.8727388672f, -0.3683854996f, -0.9296731273f, -0.9825390578f, 0.1860564427f, 0.81256471f, 0.5828709909f, 0.3196460933f, -0.9475370046f, 0.9570913859f, 0.2897862643f, -0.6876655497f, -0.7260276109f,
    -0.9988770922f, -0.047376731f, -0.1250179027f, 0.992154486f, -0.8280133617f, 0.560708367f, 0.9324863769f, -0.3612051451f, 0.6394653183f, 0.7688199442f, -0.01623847064f, -0.9998681473f, -0.9955014666f, -0.09474613458f, -0.81453315f, 0.580117012f,
    0.4037327978f, -0.9148769469f, 0.9944263371f, 0.1054336766f, -0.1624711654f, 0.9867132919f, -0.9949487814f, -0.100383875f, -0.6995302564f, 0.7146029809f, 0.5263414922f, -0.85027327f, -0.5395221479f, 0.841971408f, 0.6579370318f, 0.7530729462f,
    0.01426758847f, -0.9998982128f, -0.6734383991f, 0.7392433447f, 0.639412098f, -0.7688642071f, 0.9211571421f, 0.3891908523f, -0.146637214f, -0.9891903394f, -0.782318098f, 0.6228791163f, -0.5039610839f, -0.8637263605f, -0.7743120191f, -0.6328039957f,
};

static inline void _fnGradCoordOut2D(int seed, int xPrimed, int yPrimed, float *xo, float *yo) {
    int hash = _fnHash2D(seed, xPrimed, yPrimed) & (255 << 1);

    *xo = RAND_VECS_2D[hash];
    *yo = RAND_VECS_2D[hash | 1];
}

static inline void _fnGradCoordDual2D(int seed, int xPrimed, int yPrimed, float xd, float yd, float *xo, float *yo) {
    int hash = _fnHash2D(seed, xPrimed, yPrimed);
    int index1 = hash & (7 << 1);
    int index2 = (hash >> 3) & (255 << 1);

    float xg = GRADIENTS_2D[index1];
    float yg = GRADIENTS_2D[index1 | 1];
    float value = xd * xg + yd * yg;

    float xgo = RAND_VECS_2D[index2];
    float ygo = RAND_VECS_2D[index2 | 1];

    *xo = value * xgo;
    *yo = value * ygo;
}

static const float RAND_VECS_3D[] = {
    -0.7292736885f, -0.6618439697f, 0.1735581948f, 0, 0.790292081f, -0.5480887466f, -0.2739291014f, 0, 0.7217578935f, 0.6226212466f, -0.3023380997f, 0, 0.565683137f, -0.8208298145f, -0.0790000257f, 0, 0.760049034f, -0.5555979497f, -0.3370999617f, 0, 0.3713945616f, 0.5011264475f, 0.7816254623f, 0, -0.1277062463f, -0.4254438999f, -0.8959289049f, 0, -0.2881560924f, -0.5815838982f, 0.7607405838f, 0,
    0.5849561111f, -0.662820239f, -0.4674352136f, 0, 0.3307171178f, 0.0391653737f, 0.94291689f, 0, 0.8712121778f, -0.4113374369f, -0.2679381538f, 0, 0.580981015f, 0.7021915846f, 0.4115677815f, 0, 0.503756873f, 0.6330056931f, -0.5878203852f, 0, 0.4493712205f, 0.601390195f, 0.6606022552f, 0, -0.6878403724f, 0.09018890807f, -0.7202371714f, 0, -0.5958956522f, -0.6469350577f, 0.475797649f, 0,
    -0.5127052122f, 0.1946921978f, -0.8361987284f, 0, -0.9911507142f, -0.05410276466f, -0.1212153153f, 0, -0.2149721042f, 0.9720882117f, -0.09397607749f, 0, -0.7518650936f, -0.5428057603f, 0.3742469607f, 0, 0.5237068895f, 0.8516377189f, -0.02107817834f, 0, 0.6333504779f, 0.1926167129f, -0.7495104896f, 0, -0.06788241606f, 0.3998305789f, 0.9140719259f, 0, -0.5538628599f, -0.4729896695f, -0.6852128902f, 0,
    -0.7261455366f, -0.5911990757f, 0.3509933228f, 0, -0.9229274737f, -0.1782808786f, 0.3412049336f, 0, -0.6968815002f, 0.6511274338f, 0.3006480328f, 0, 0.9608044783f, -0.2098363234f, -0.1811724921f, 0, 0.06817146062f, -0.9743405129f, 0.2145069156f, 0, -0.3577285196f, -0.6697087264f, -0.6507845481f, 0, -0.1868621131f, 0.7648617052f, -0.6164974636f, 0, -0.6541697588f, 0.3967914832f, 0.6439087246f, 0,
    0.6993340405f, -0.6164538506f, 0.3618239211f, 0, -0.1546665739f, 0.6291283928f, 0.7617583057f, 0, -0.6841612949f, -0.2580482182f, -0.6821542638f, 0, 0.5383980957f, 0.4258654885f, 0.7271630328f, 0, -0.5026987823f, -0.7939832935f, -0.3418836993f, 0, 0.3202971715f, 0.2834415347f, 0.9039195862f, 0, 0.8683227101f, -0.0003762656404f, -0.4959995258f, 0, 0.791120031f, -0.08511045745f, 0.6057105799f, 0,
    -0.04011016052f, -0.4397248749f, 0.8972364289f, 0, 0.9145119872f, 0.3579346169f, -0.1885487608f, 0, -0.9612039066f, -0.2756484276f, 0.01024666929f, 0, 0.6510361721f, -0.2877799159f, -0.7023778346f, 0, -0.2041786351f, 0.7365237271f, 0.644859585f, 0, -0.7718263711f, 0.3790626912f, 0.5104855816f, 0, -0.3060082741f, -0.7692987727f, 0.5608371729f, 0, 0.454007341f, -0.5024843065f, 0.7357899537f, 0,
    0.4816795475f, 0.6021208291f, -0.6367380315f, 0, 0.6961980369f, -0.3222197429f, 0.641469197f, 0, -0.6532160499f, -0.6781148932f, 0.3368515753f, 0, 0.5089301236f, -0.6154662304f, -0.6018234363f, 0, -0.1635919754f, -0.9133604627f, -0.372840892f, 0, 0.52408019f, -0.8437664109f, 0.1157505864f, 0, 0.5902587356f, 0.4983817807f, -0.6349883666f, 0, 0.5863227872f, 0.494764745f, 0.6414307729f, 0,
    0.6779335087f, 0.2341345225f, 0.6968408593f, 0, 0.7177054546f, -0.6858979348f, 0.120178631f, 0, -0.5328819713f, -0.5205125012f, 0.6671608058f, 0, -0.8654874251f, -0.0700727088f, -0.4960053754f, 0, -0.2861810166f, 0.7952089234f, 0.5345495242f, 0, -0.04849529634f, 0.9810836427f, -0.1874115585f, 0, -0.6358521667f, 0.6058348682f, 0.4781800233f, 0, 0.6254794696f, -0.2861619734f, 0.7258696564f, 0,
    -0.2585259868f, 0.5061949264f, -0.8227581726f, 0, 0.02136306781f, 0.5064016808f, -0.8620330371f, 0, 0.200111773f, 0.8599263484f, 0.4695550591f, 0, 0.4743561372f, 0.6014985084f, -0.6427953014f, 0, 0.6622993731f, -0.5202474575f, -0.5391679918f, 0, 0.08084972818f, -0.6532720452f, 0.7527940996f, 0, -0.6893687501f, 0.0592860349f, 0.7219805347f, 0, -0.1121887082f, -0.9673185067f, 0.2273952515f, 0,
    0.7344116094f, 0.5979668656f, -0.3210532909f, 0, 0.5789393465f, -0.2488849713f, 0.7764570201f, 0, 0.6988182827f, 0.3557169806f, -0.6205791146f, 0, -0.8636845529f, -0.2748771249f, -0.4224826141f, 0, -0.4247027957f, -0.4640880967f, 0.777335046f, 0, 0.5257722489f, -0.8427017621f, 0.1158329937f, 0, 0.9343830603f, 0.316302472f, -0.1639543925f, 0, -0.1016836419f, -0.8057303073f, -0.5834887393f, 0,
    -0.6529238969f, 0.50602126f, -0.5635892736f, 0, -0.2465286165f, -0.9668205684f, -0.06694497494f, 0, -0.9776897119f, -0.2099250524f, -0.007368825344f, 0, 0.7736893337f, 0.5734244712f, 0.2694238123f, 0, -0.6095087895f, 0.4995678998f, 0.6155736747f, 0, 0.5794535482f, 0.7434546771f, 0.3339292269f, 0, -0.8226211154f, 0.08142581855f, 0.5627293636f, 0, -0.510385483f, 0.4703667658f, 0.7199039967f, 0,
    -0.5764971849f, -0.07231656274f, -0.8138926898f, 0, 0.7250628871f, 0.3949971505f, -0.5641463116f, 0, -0.1525424005f, 0.4860840828f, -0.8604958341f, 0, -0.5550976208f, -0.4957820792f, 0.667882296f, 0, -0.1883614327f, 0.9145869398f, 0.357841725f, 0, 0.7625556724f, -0.5414408243f, -0.3540489801f, 0, -0.5870231946f, -0.3226498013f, -0.7424963803f, 0, 0.3051124198f, 0.2262544068f, -0.9250488391f, 0,
    0.6379576059f, 0.577242424f, -0.5097070502f, 0, -0.5966775796f, 0.1454852398f, -0.7891830656f, 0, -0.658330573f, 0.6555487542f, -0.3699414651f, 0, 0.7434892426f, 0.2351084581f, 0.6260573129f, 0, 0.5562114096f, 0.8264360377f, -0.0873632843f, 0, -0.3028940016f, -0.8251527185f, 0.4768419182f, 0, 0.1129343818f, -0.985888439f, -0.1235710781f, 0, 0.5937652891f, -0.5896813806f, 0.5474656618f, 0,
    0.6757964092f, -0.5835758614f, -0.4502648413f, 0, 0.7242302609f, -0.1152719764f, 0.6798550586f, 0, -0.9511914166f, 0.0753623979f, -0.2992580792f, 0, 0.2539470961f, -0.1886339355f, 0.9486454084f, 0, 0.571433621f, -0.1679450851f, -0.8032795685f, 0, -0.06778234979f, 0.3978269256f, 0.9149531629f, 0, 0.6074972649f, 0.733060024f, -0.3058922593f, 0, -0.5435478392f, 0.1675822484f, 0.8224791405f, 0,
    -0.5876678086f, -0.3380045064f, -0.7351186982f, 0, -0.7967562402f, 0.04097822706f, -0.6029098428f, 0, -0.1996350917f, 0.8706294745f, 0.4496111079f, 0, -0.02787660336f, -0.9106232682f, -0.4122962022f, 0, -0.7797625996f, -0.6257634692f, 0.01975775581f, 0, -0.5211232846f, 0.7401644346f, -0.4249554471f, 0, 0.8575424857f, 0.4053272873f, -0.3167501783f, 0, 0.1045223322f, 0.8390195772f, -0.5339674439f, 0,
    0.3501822831f, 0.9242524096f, -0.1520850155f, 0, 0.1987849858f, 0.07647613266f, 0.9770547224f, 0, 0.7845996363f, 0.6066256811f, -0.1280964233f, 0, 0.09006737436f, -0.9750989929f, -0.2026569073f, 0, -0.8274343547f, -0.542299559f, 0.1458203587f, 0, -0.3485797732f, -0.415802277f, 0.840000362f, 0, -0.2471778936f, -0.7304819962f, -0.6366310879f, 0, -0.3700154943f, 0.8577948156f, 0.3567584454f, 0,
    0.5913394901f, -0.548311967f, -0.5913303597f, 0, 0.1204873514f, -0.7626472379f, -0.6354935001f, 0, 0.616959265f, 0.03079647928f, 0.7863922953f, 0, 0.1258156836f, -0.6640829889f, -0.7369967419f, 0, -0.6477565124f, -0.1740147258f, -0.7417077429f, 0, 0.6217889313f, -0.7804430448f, -0.06547655076f, 0, 0.6589943422f, -0.6096987708f, 0.4404473475f, 0, -0.2689837504f, -0.6732403169f, -0.6887635427f, 0,
    -0.3849775103f, 0.5676542638f, 0.7277093879f, 0, 0.5754444408f, 0.8110471154f, -0.1051963504f, 0, 0.9141593684f, 0.3832947817f, 0.131900567f, 0, -0.107925319f, 0.9245493968f, 0.3654593525f, 0, 0.377977089f, 0.3043148782f, 0.8743716458f, 0, -0.2142885215f, -0.8259286236f, 0.5214617324f, 0, 0.5802544474f, 0.4148098596f, -0.7008834116f, 0, -0.1982660881f, 0.8567161266f, -0.4761596756f, 0,
    -0.03381553704f, 0.3773180787f, -0.9254661404f, 0, -0.6867922841f, -0.6656597827f, 0.2919133642f, 0, 0.7731742607f, -0.2875793547f, -0.5652430251f, 0, -0.09655941928f, 0.9193708367f, -0.3813575004f, 0, 0.2715702457f, -0.9577909544f, -0.09426605581f, 0, 0.2451015704f, -0.6917998565f, -0.6792188003f, 0, 0.977700782f, -0.1753855374f, 0.1155036542f, 0, -0.5224739938f, 0.8521606816f, 0.02903615945f, 0,
    -0.7734880599f, -0.5261292347f, 0.3534179531f, 0, -0.7134492443f, -0.269547243f, 0.6467878011f, 0, 0.1644037271f, 0.5105846203f, -0.8439637196f, 0, 0.6494635788f, 0.05585611296f, 0.7583384168f, 0, -0.4711970882f, 0.5017280509f, -0.7254255765f, 0, -0.6335764307f, -0.2381686273f, -0.7361091029f, 0, -0.9021533097f, -0.270947803f, -0.3357181763f, 0, -0.3793711033f, 0.872258117f, 0.3086152025f, 0,
    -0.6855598966f, -0.3250143309f, 0.6514394162f, 0, 0.2900942212f, -0.7799057743f, -0.5546100667f, 0, -0.2098319339f, 0.85037073f, 0.4825351604f, 0, -0.4592603758f, 0.6598504336f, -0.5947077538f, 0, 0.8715945488f, 0.09616365406f, -0.4807031248f, 0, -0.6776666319f, 0.7118504878f, -0.1844907016f, 0, 0.7044377633f, 0.312427597f, 0.637304036f, 0, -0.7052318886f, -0.2401093292f, -0.6670798253f, 0,
    0.081921007f, -0.7207336136f, -0.6883545647f, 0, -0.6993680906f, -0.5875763221f, -0.4069869034f, 0, -0.1281454481f, 0.6419895885f, 0.7559286424f, 0, -0.6337388239f, -0.6785471501f, -0.3714146849f, 0, 0.5565051903f, -0.2168887573f, -0.8020356851f, 0, -0.5791554484f, 0.7244372011f, -0.3738578718f, 0, 0.1175779076f, -0.7096451073f, 0.6946792478f, 0, -0.6134619607f, 0.1323631078f, 0.7785527795f, 0,
    0.6984635305f, -0.02980516237f, -0.715024719f, 0, 0.8318082963f, -0.3930171956f, 0.3919597455f, 0, 0.1469576422f, 0.05541651717f, -0.9875892167f, 0, 0.708868575f, -0.2690503865f, 0.6520101478f, 0, 0.2726053183f, 0.67369766f, -0.68688995f, 0, -0.6591295371f, 0.3035458599f, -0.6880466294f, 0, 0.4815131379f, -0.7528270071f, 0.4487723203f, 0, 0.9430009463f, 0.1675647412f, -0.2875261255f, 0,
    0.434802957f, 0.7695304522f, -0.4677277752f, 0, 0.3931996188f, 0.594473625f, 0.7014236729f, 0, 0.7254336655f, -0.603925654f, 0.3301814672f, 0, 0.7590235227f, -0.6506083235f, 0.02433313207f, 0, -0.8552768592f, -0.3430042733f, 0.3883935666f, 0, -0.6139746835f, 0.6981725247f, 0.3682257648f, 0, -0.7465905486f, -0.5752009504f, 0.3342849376f, 0, 0.5730065677f, 0.810555537f, -0.1210916791f, 0,
    -0.9225877367f, -0.3475211012f, -0.167514036f, 0, -0.7105816789f, -0.4719692027f, -0.5218416899f, 0, -0.08564609717f, 0.3583001386f, 0.929669703f, 0, -0.8279697606f, -0.2043157126f, 0.5222271202f, 0, 0.427944023f, 0.278165994f, 0.8599346446f, 0, 0.5399079671f, -0.7857120652f, -0.3019204161f, 0, 0.5678404253f, -0.5495413974f, -0.6128307303f, 0, -0.9896071041f, 0.1365639107f, -0.04503418428f, 0,
    -0.6154342638f, -0.6440875597f, 0.4543037336f, 0, 0.1074204368f, -0.7946340692f, 0.5975094525f, 0, -0.3595449969f, -0.8885529948f, 0.28495784f, 0, -0.2180405296f, 0.1529888965f, 0.9638738118f, 0, -0.7277432317f, -0.6164050508f, -0.3007234646f, 0, 0.7249729114f, -0.00669719484f, 0.6887448187f, 0, -0.5553659455f, -0.5336586252f, 0.6377908264f, 0, 0.5137558015f, 0.7976208196f, -0.3160000073f, 0,
    -0.3794024848f, 0.9245608561f, -0.03522751494f, 0, 0.8229248658f, 0.2745365933f, -0.4974176556f, 0, -0.5404114394f, 0.6091141441f, 0.5804613989f, 0, 0.8036581901f, -0.2703029469f, 0.5301601931f, 0, 0.6044318879f, 0.6832968393f, 0.4095943388f, 0, 0.06389988817f, 0.9658208605f, -0.2512108074f, 0, 0.1087113286f, 0.7402471173f, -0.6634877936f, 0, -0.713427712f, -0.6926784018f, 0.1059128479f, 0,
    0.6458897819f, -0.5724548511f, -0.5050958653f, 0, -0.6553931414f, 0.7381471625f, 0.159995615f, 0, 0.3910961323f, 0.9188871375f, -0.05186755998f, 0, -0.4879022471f, -0.5904376907f, 0.6429111375f, 0, 0.6014790094f, 0.7707441366f, -0.2101820095f, 0, -0.5677173047f, 0.7511360995f, 0.3368851762f, 0, 0.7858573506f, 0.226674665f, 0.5753666838f, 0, -0.4520345543f, -0.604222686f, -0.6561857263f, 0,
    0.002272116345f, 0.4132844051f, -0.9105991643f, 0, -0.5815751419f, -0.5162925989f, 0.6286591339f, 0, -0.03703704785f, 0.8273785755f, 0.5604221175f, 0, -0.5119692504f, 0.7953543429f, -0.3244980058f, 0, -0.2682417366f, -0.9572290247f, -0.1084387619f, 0, -0.2322482736f, -0.9679131102f, -0.09594243324f, 0, 0.3554328906f, -0.8881505545f, 0.2913006227f, 0, 0.7346520519f, -0.4371373164f, 0.5188422971f, 0,
    0.9985120116f, 0.04659011161f, -0.02833944577f, 0, -0.3727687496f, -0.9082481361f, 0.1900757285f, 0, 0.91737377f, -0.3483642108f, 0.1925298489f, 0, 0.2714911074f, 0.4147529736f, -0.8684886582f, 0, 0.5131763485f, -0.7116334161f, 0.4798207128f, 0, -0.8737353606f, 0.18886992f, -0.4482350644f, 0, 0.8460043821f, -0.3725217914f, 0.3814499973f, 0, 0.8978727456f, -0.1780209141f, -0.4026575304f, 0,
    0.2178065647f, -0.9698322841f, -0.1094789531f, 0, -0.1518031304f, -0.7788918132f, -0.6085091231f, 0, -0.2600384876f, -0.4755398075f, -0.8403819825f, 0, 0.572313509f, -0.7474340931f, -0.3373418503f, 0, -0.7174141009f, 0.1699017182f, -0.6756111411f, 0, -0.684180784f, 0.02145707593f, -0.7289967412f, 0, -0.2007447902f, 0.06555605789f, -0.9774476623f, 0, -0.1148803697f, -0.8044887315f, 0.5827524187f, 0,
    -0.7870349638f, 0.03447489231f, 0.6159443543f, 0, -0.2015596421f, 0.6859872284f, 0.6991389226f, 0, -0.08581082512f, -0.10920836f, -0.9903080513f, 0, 0.5532693395f, 0.7325250401f, -0.396610771f, 0, -0.1842489331f, -0.9777375055f, -0.1004076743f, 0, 0.0775473789f, -0.9111505856f, 0.4047110257f, 0, 0.1399838409f, 0.7601631212f, -0.6344734459f, 0, 0.4484419361f, -0.845289248f, 0.2904925424f, 0
};

static inline void _fnGradCoordOut3D(int seed, int xPrimed, int yPrimed, int zPrimed, float *xo, float *yo, float *zo) {
    int hash = _fnHash3D(seed, xPrimed, yPrimed, zPrimed) & (255 << 2);

    *xo = RAND_VECS_3D[hash];
    *yo = RAND_VECS_3D[hash | 1];
    *zo = RAND_VECS_3D[hash | 2];
}

static inline void _fnGradCoordDual3D(int seed, int xPrimed, int yPrimed, int zPrimed, float xd, float yd, float zd, float *xo, float *yo, float *zo) {
    int hash = _fnHash3D(seed, xPrimed, yPrimed, zPrimed);
    int index1 = hash & (15 << 2);
    int index2 = (hash >> 6) & (255 << 2);

    float xg = GRADIENTS_3D[index1];
    float yg = GRADIENTS_3D[index1 | 1];
    float zg = GRADIENTS_3D[index1 | 2];
    float value = xd * xg + yd * yg + zd * zg;

    float xgo = RAND_VECS_3D[index2];
    float ygo = RAND_VECS_3D[index2 | 1];
    float zgo = RAND_VECS_3D[index2 | 2];

    *xo = value * xgo;
    *yo = value * ygo;
    *zo = value * zgo;
}

// ====================
// Noise functions
// ====================

// Simplex Noise

static float _fnSingleSimplex2D(int seed, FNfloat x, FNfloat y) {
    const FNfloat SQRT3 = (FNfloat)1.7320508075688772935274463415059;
    const FNfloat F2 = 0.5f * (SQRT3 - 1);
    const FNfloat G2 = (3 - SQRT3) / 6;

    FNfloat t = (x + y) * F2;
    int i = _fnFastFloor(x + t);
    int j = _fnFastFloor(y + t);

    t = (i + j) * G2;
    float x0 = (float)(x - (i - t));
    float y0 = (float)(y - (j - t));

    float y0_1 = y0 - 1;
    int i1 = (int)(y0_1 - x0);
    int j1 = ~i1;

    float x1 = x0 + i1 + (float)G2;
    float y1 = y0 + j1 + (float)G2;
    float x2 = x0 - 1 + 2 * (float)G2;
    float y2 = y0_1 + 2 * (float)G2;

    i *= PRIME_X;
    j *= PRIME_Y;

    i1 &= PRIME_X;
    j1 &= PRIME_Y;

    float n0, n1, n2;

    float a = 0.5f - x0 * x0 - y0 * y0;
    if (a < 0) n0 = 0;
    else {
        a *= a;
        n0 = a * a * _fnGradCoord2D(seed, i, j, x0, y0);
    }

    float b = 0.5f - x1 * x1 - y1 * y1;
    if (b < 0) n1 = 0;
    else {
        b *= b;
        n1 = b * b * _fnGradCoord2D(seed, i + i1, j + j1, x1, y1);
    }

    float c = 0.5f - x2 * x2 - y2 * y2;
    if (c < 0) n2 = 0;
    else {
        c *= c;
        n2 = c * c * _fnGradCoord2D(seed, i + PRIME_X, j + PRIME_Y, x2, y2);
    }
    return (n0 + n1 + n2) * 38.283687591552734375f;
}

static float _fnSingleSimplex3D(int seed, FNfloat x, FNfloat y, FNfloat z) {
    const FNfloat F3 = (FNfloat)(1.0 / 3.0);
    const FNfloat G3 = (FNfloat)(1.0 / 6.0);

    FNfloat t = (x + y + z) * F3;
    int i = _fnFastFloor(x + t);
    int j = _fnFastFloor(y + t);
    int k = _fnFastFloor(z + t);

    t = (i + j + k) * G3;
    float x0 = (float)(x - (i - t));
    float y0 = (float)(y - (j - t));
    float z0 = (float)(z - (k - t));

    int i1, j1, k1;
    int i2, j2, k2;

    if (x0 >= y0) {
        if (y0 >= z0) {
            i1 = -1; j1 = 0; k1 = 0; i2 = -1; j2 = -1; k2 = 0;
        } else if (x0 >= z0) {
            i1 = -1; j1 = 0; k1 = 0; i2 = -1; j2 = 0; k2 = -1;
        } else { // x0 < z0
            i1 = 0; j1 = 0; k1 = -1; i2 = -1; j2 = 0; k2 = -1;
        }
    } else { // x0 < y0
        if (y0 < z0) {
            i1 = 0; j1 = 0; k1 = -1; i2 = 0; j2 = -1; k2 = -1;
        } else if (x0 < z0) {
            i1 = 0; j1 = -1; k1 = 0; i2 = 0; j2 = -1; k2 = -1;
        } else { // x0 >= z0
            i1 = 0; j1 = -1; k1 = 0; i2 = -1; j2 = -1; k2 = 0;
        }
    }

    float x1 = x0 + i1 + (float)G3;
    float y1 = y0 + j1 + (float)G3;
    float z1 = z0 + k1 + (float)G3;
    float x2 = x0 + i2 + (float)F3;
    float y2 = y0 + j2 + (float)F3;
    float z2 = z0 + k2 + (float)F3;
    float x3 = x0 - 0.5f;
    float y3 = y0 - 0.5f;
    float z3 = z0 - 0.5f;

    i  *= PRIME_X;
    j  *= PRIME_Y;
    k  *= PRIME_Z;

    i1 &= PRIME_X;
    j1 &= PRIME_Y;
    k1 &= PRIME_Z;

    i2 &= PRIME_X;
    j2 &= PRIME_Y;
    k2 &= PRIME_Z;

    float n0, n1, n2, n3;

    float a = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;
    if (a < 0) n0 = 0;
    else {
        a *= a;
        n0 = a * a * _fnGradCoord3D(seed, i, j, k, x0, y0, z0);
    }

    float b = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;
    if (b < 0) n1 = 0;
    else {
        b *= b;
        n1 = b * b * _fnGradCoord3D(seed, i + i1, j + j1, k + k1, x1, y1, z1);
    }

    float c = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;
    if (c < 0) n2 = 0;
    else {
        c *= c;
        n2 = c * c * _fnGradCoord3D(seed, i + i2, j + j2, k + k2, x2, y2, z2);
    }

    float d = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;
    if (d < 0) n3 = 0;
    else {
        d *= d;
        n3 = d * d * _fnGradCoord3D(seed, i + PRIME_X, j + PRIME_Y, k + PRIME_Z, x3, y3, z3);
    }

    return (n0 + n1 + n2 + n3) * 32.69588470458984375f;
}

// OpenSimplex 2 Noise

static float _fnSingleOpenSimplex23D(int seed, FNfloat x, FNfloat y, FNfloat z) {
    const FNfloat R3 = (FNfloat)(2.0 / 3.0);

    FNfloat r = (x + y + z) * R3; // Rotation, not skew
    x = r - x; y = r - y; z = r - z;

    int i = _fnFastRound(x);
    int j = _fnFastRound(y);
    int k = _fnFastRound(z);
    float x0 = (float)(x - i);
    float y0 = (float)(y - j);
    float z0 = (float)(z - k);

    int xNSign = (int)(-x0 - 1.0f) | 1;
    int yNSign = (int)(-y0 - 1.0f) | 1;
    int zNSign = (int)(-z0 - 1.0f) | 1;

    float ax0 = xNSign * -x0;
    float ay0 = yNSign * -y0;
    float az0 = zNSign * -z0;

    i *= PRIME_X;
    j *= PRIME_Y;
    k *= PRIME_Z;

    float value = 0;

    float a = (0.6f - x0 * x0) - (y0 * y0 + z0 * z0);
    for (int l = 0; l < 2; l++)
    {
        if (a > 0)
        {
            value += (a * a) * (a * a) * _fnGradCoord3D(seed, i, j, k, x0, y0, z0);
        }

        float b = a + 1;
        int i1 = i;
        int j1 = j;
        int k1 = k;
        float x1 = x0;
        float y1 = y0;
        float z1 = z0;
        if (ax0 >= ay0 && ax0 >= az0) {
            x1 += xNSign;
            b -= xNSign * 2 * x1;
            i1 -= xNSign * PRIME_X;
        } else if (ay0 > ax0 && ay0 >= az0) {
            y1 += yNSign;
            b -= yNSign * 2 * y1;
            j1 -= yNSign * PRIME_Y;
        } else  {
            z1 += zNSign;
            b -= zNSign * 2 * z1;
            k1 -= zNSign * PRIME_Z;
        }

        if (b > 0) {
            value += (b * b) * (b * b) * _fnGradCoord3D(seed, i1, j1, k1, x1, y1, z1);
        }

        if (l == 1) break;

        ax0 = 0.5f - ax0;
        ay0 = 0.5f - ay0;
        az0 = 0.5f - az0;

        x0 = xNSign * ax0;
        y0 = yNSign * ay0;
        z0 = zNSign * az0;

        a += (0.75f - ax0) - (ay0 + az0);

        i += (xNSign >> 1) & PRIME_X;
        j += (yNSign >> 1) & PRIME_Y;
        k += (zNSign >> 1) & PRIME_Z;

        xNSign = -xNSign;
        yNSign = -yNSign;
        zNSign = -zNSign;

        seed += 1293373;
    }

    return value * 32.69428253173828125f;
}

// Cellular Noise

static float _fnSingleCellular2D(fn_state *state, int seed, FNfloat x, FNfloat y) {
    int xr = _fnFastRound(x);
    int yr = _fnFastRound(y);

    float distance0 = FLT_MAX;
    float distance1 = FLT_MAX;
    int closestHash = 0;

    float cellularJitter = 0.5f * state->cellular_jitter_mod;

    int xPrimed = (xr - 1) * PRIME_X;
    int yPrimedBase = (yr - 1) * PRIME_Y;

    switch (state->cellular_distance_func) {
        default:
        case FN_CELLULAR_DIST_EUCLIDEAN:
        case FN_CELLULAR_DIST_EUCLIDEANSQ:
            for (int xi = xr - 1; xi <= xr + 1; xi++) {
                int yPrimed = yPrimedBase;

                for (int yi = yr - 1; yi <= yr + 1; yi++) {
                    int hash = _fnHash2D(seed, xPrimed, yPrimed);
                    int idx = hash & (255 << 1);

                    float vecX = (float)(xi - x) + RAND_VECS_2D[idx] * cellularJitter;
                    float vecY = (float)(yi - y) + RAND_VECS_2D[idx | 1] * cellularJitter;

                    float newDistance = vecX * vecX + vecY * vecY;

                    distance1 = _fnFastMax(_fnFastMin(distance1, newDistance), distance0);
                    if (newDistance < distance0) {
                        distance0 = newDistance;
                        closestHash = hash;
                    }
                    yPrimed += PRIME_Y;
                }
                xPrimed += PRIME_X;
            }
            break;
        case FN_CELLULAR_DIST_MANHATTAN:
            for (int xi = xr - 1; xi <= xr + 1; xi++) {
                int yPrimed = yPrimedBase;

                for (int yi = yr - 1; yi <= yr + 1; yi++) {
                    int hash = _fnHash2D(seed, xPrimed, yPrimed);
                    int idx = hash & (255 << 1);

                    float vecX = (float)(xi - x) + RAND_VECS_2D[idx] * cellularJitter;
                    float vecY = (float)(yi - y) + RAND_VECS_2D[idx | 1] * cellularJitter;

                    float newDistance = _fnFastAbs(vecX) + _fnFastAbs(vecY);

                    distance1 = _fnFastMax(_fnFastMin(distance1, newDistance), distance0);
                    if (newDistance < distance0) {
                        distance0 = newDistance;
                        closestHash = hash;
                    }
                    yPrimed += PRIME_Y;
                }
                xPrimed += PRIME_X;
            }
            break;
        case FN_CELLULAR_DIST_HYBRID:
            for (int xi = xr - 1; xi <= xr + 1; xi++) {
                int yPrimed = yPrimedBase;
                for (int yi = yr - 1; yi <= yr + 1; yi++) {
                    int hash = _fnHash2D(seed, xPrimed, yPrimed);
                    int idx = hash & (255 << 1);

                    float vecX = (float)(xi - x) + RAND_VECS_2D[idx] * cellularJitter;
                    float vecY = (float)(yi - y) + RAND_VECS_2D[idx | 1] * cellularJitter;

                    float newDistance = (_fnFastAbs(vecX) + _fnFastAbs(vecY)) + (vecX * vecX + vecY * vecY);

                    distance1 = _fnFastMax(_fnFastMin(distance1, newDistance), distance0);
                    if (newDistance < distance0) {
                        distance0 = newDistance;
                        closestHash = hash;
                    }
                    yPrimed += PRIME_Y;
                }
                xPrimed += PRIME_X;
            }
            break;
    }

    if (state->cellular_distance_func == FN_CELLULAR_DIST_EUCLIDEAN && state->cellular_return_type >= FN_CELLULAR_RET_DISTANCE) {
        distance0 = _fnFastSqrt(distance0);
        if (state->cellular_return_type >= FN_CELLULAR_RET_DISTANCE2)
            distance1 = _fnFastSqrt(distance1);
    }

    switch (state->cellular_return_type) {
        case FN_CELLULAR_RET_CELLVALUE:
            return closestHash / 2147483648.0f;
        case FN_CELLULAR_RET_DISTANCE:
            return distance0 - 1;
        case FN_CELLULAR_RET_DISTANCE2:
            return distance1 - 1;
        case FN_CELLULAR_RET_DISTANCE2ADD:
            return (distance1 + distance0) * 0.5f - 1;
        case FN_CELLULAR_RET_DISTANCE2SUB:
            return distance1 - distance0 - 1;
        case FN_CELLULAR_RET_DISTANCE2MUL:
            return distance1 * distance0 * 0.5f - 1;
        case FN_CELLULAR_RET_DISTANCE2DIV:
            return distance0 / distance1 - 1;
        default:
            return 0;
    }
}

static float _fnSingleCellular3D(fn_state *state, int seed, FNfloat x, FNfloat y, FNfloat z) {
    int xr = _fnFastRound(x);
    int yr = _fnFastRound(y);
    int zr = _fnFastRound(z);

    float distance0 = FLT_MAX;
    float distance1 = FLT_MAX;
    int closestHash = 0;

    float cellularJitter = 0.45f * state->cellular_jitter_mod;

    int xPrimed = (xr - 1) * PRIME_X;
    int yPrimedBase = (yr - 1) * PRIME_Y;
    int zPrimedBase = (zr - 1) * PRIME_Z;

    switch (state->cellular_distance_func) {
        default:
        case FN_CELLULAR_DIST_EUCLIDEAN:
        case FN_CELLULAR_DIST_EUCLIDEANSQ:
            for (int xi = xr - 1; xi <= xr + 1; xi++) {
                int yPrimed = yPrimedBase;

                for (int yi = yr - 1; yi <= yr + 1; yi++) {
                    int zPrimed = zPrimedBase;

                    for (int zi = zr - 1; zi <= zr + 1; zi++) {
                        int hash = _fnHash3D(seed, xPrimed, yPrimed, zPrimed);
                        int idx = hash & (255 << 2);

                        float vecX = (float)(xi - x) + RAND_VECS_3D[idx] * cellularJitter;
                        float vecY = (float)(yi - y) + RAND_VECS_3D[idx | 1] * cellularJitter;
                        float vecZ = (float)(zi - z) + RAND_VECS_3D[idx | 2] * cellularJitter;

                        float newDistance = vecX * vecX + vecY * vecY + vecZ * vecZ;

                        distance1 = _fnFastMax(_fnFastMin(distance1, newDistance), distance0);
                        if (newDistance < distance0) {
                            distance0 = newDistance;
                            closestHash = hash;
                        }
                        zPrimed += PRIME_Z;
                    }
                    yPrimed += PRIME_Y;
                }
                xPrimed += PRIME_X;
            }
            break;
        case FN_CELLULAR_DIST_MANHATTAN:
            for (int xi = xr - 1; xi <= xr + 1; xi++) {
                int yPrimed = yPrimedBase;

                for (int yi = yr - 1; yi <= yr + 1; yi++) {
                    int zPrimed = zPrimedBase;

                    for (int zi = zr - 1; zi <= zr + 1; zi++) {
                        int hash = _fnHash3D(seed, xPrimed, yPrimed, zPrimed);
                        int idx = hash & (255 << 2);

                        float vecX = (float)(xi - x) + RAND_VECS_3D[idx] * cellularJitter;
                        float vecY = (float)(yi - y) + RAND_VECS_3D[idx | 1] * cellularJitter;
                        float vecZ = (float)(zi - z) + RAND_VECS_3D[idx | 2] * cellularJitter;

                        float newDistance = _fnFastAbs(vecX) + _fnFastAbs(vecY) + _fnFastAbs(vecZ);

                        distance1 = _fnFastMax(_fnFastMin(distance1, newDistance), distance0);
                        if (newDistance < distance0) {
                            distance0 = newDistance;
                            closestHash = hash;
                        }
                        zPrimed += PRIME_Z;
                    }
                    yPrimed += PRIME_Y;
                }
                xPrimed += PRIME_X;
            }
            break;
        case FN_CELLULAR_DIST_HYBRID:
            for (int xi = xr - 1; xi <= xr + 1; xi++) {
                int yPrimed = yPrimedBase;

                for (int yi = yr - 1; yi <= yr + 1; yi++) {
                    int zPrimed = zPrimedBase;

                    for (int zi = zr - 1; zi <= zr + 1; zi++) {
                        int hash = _fnHash3D(seed, xPrimed, yPrimed, zPrimed);
                        int idx = hash & (255 << 2);

                        float vecX = (float)(xi - x) + RAND_VECS_3D[idx] * cellularJitter;
                        float vecY = (float)(yi - y) + RAND_VECS_3D[idx | 1] * cellularJitter;
                        float vecZ = (float)(zi - z) + RAND_VECS_3D[idx | 2] * cellularJitter;
                            
                        float newDistance = (_fnFastAbs(vecX) + _fnFastAbs(vecY) + _fnFastAbs(vecZ)) + (vecX * vecX + vecY * vecY + vecZ * vecZ);

                        distance1 = _fnFastMax(_fnFastMin(distance1, newDistance), distance0);
                        if (newDistance < distance0) {
                            distance0 = newDistance;
                            closestHash = hash;
                        }
                        zPrimed += PRIME_Z;
                    }
                    yPrimed += PRIME_Y;
                }
                xPrimed += PRIME_X;
            }
            break;
    }

    if (state->cellular_distance_func == FN_CELLULAR_DIST_EUCLIDEAN && state->cellular_return_type >= FN_CELLULAR_RET_DISTANCE) {
        distance0 = _fnFastSqrt(distance0);
        if (state->cellular_return_type >= FN_CELLULAR_RET_DISTANCE2)
            distance1 = _fnFastSqrt(distance1);
    }

    switch (state->cellular_return_type) {
        case FN_CELLULAR_RET_CELLVALUE:
            return closestHash / 2147483648.0f;
        case FN_CELLULAR_RET_DISTANCE:
            return distance0 - 1;
        case FN_CELLULAR_RET_DISTANCE2:
            return distance1 - 1;
        case FN_CELLULAR_RET_DISTANCE2ADD:
            return (distance1 + distance0) * 0.5f - 1;
        case FN_CELLULAR_RET_DISTANCE2SUB:
            return distance1 - distance0 - 1;
        case FN_CELLULAR_RET_DISTANCE2MUL:
            return distance1 * distance0 * 0.5f - 1;
        case FN_CELLULAR_RET_DISTANCE2DIV:
            return distance0 / distance1 - 1;
        default:
            return 0;
    }
}

// Perlin Noise

static float _fnSinglePerlin2D(int seed, FNfloat x, FNfloat y) {
    int x0 = _fnFastFloor(x);
    int y0 = _fnFastFloor(y);

    float xd0 = (float) (x - x0);
    float yd0 = (float) (y - y0);
    float xd1 = xd0 - 1;
    float yd1 = yd0 - 1;

    float xs = _fnInterpQuintic(xd0);
    float ys = _fnInterpQuintic(yd0);

    x0 *= PRIME_X;
    y0 *= PRIME_Y;
    int x1 = x0 + PRIME_X;
    int y1 = y0 + PRIME_Y;

    float xf0 = _fnLerp(_fnGradCoord2D(seed, x0, y0, xd0, yd0), _fnGradCoord2D(seed, x1, y0, xd1, yd0), xs);
    float xf1 = _fnLerp(_fnGradCoord2D(seed, x0, y1, xd0, yd1), _fnGradCoord2D(seed, x1, y1, xd1, yd1), xs);

    return _fnLerp(xf0, xf1, ys) * 0.579106986522674560546875f;
}

static float _fnSinglePerlin3D(int seed, FNfloat x, FNfloat y, FNfloat z) {
    int x0 = _fnFastFloor(x);
    int y0 = _fnFastFloor(y);
    int z0 = _fnFastFloor(z);

    float xd0 = (float) (x - x0);
    float yd0 = (float) (y - y0);
    float zd0 = (float) (z - z0);
    float xd1 = xd0 - 1;
    float yd1 = yd0 - 1;
    float zd1 = zd0 - 1;

    float xs = _fnInterpQuintic(xd0);
    float ys = _fnInterpQuintic(yd0);
    float zs = _fnInterpQuintic(zd0);

    x0 *= PRIME_X;
    y0 *= PRIME_Y;
    z0 *= PRIME_Z;
    int x1 = x0 + PRIME_X;
    int y1 = y0 + PRIME_Y;
    int z1 = z0 + PRIME_Z;

    float xf00 = _fnLerp(_fnGradCoord3D(seed, x0, y0, z0, xd0, yd0, zd0), _fnGradCoord3D(seed, x1, y0, z0, xd1, yd0, zd0), xs);
    float xf10 = _fnLerp(_fnGradCoord3D(seed, x0, y1, z0, xd0, yd1, zd0), _fnGradCoord3D(seed, x1, y1, z0, xd1, yd1, zd0), xs);
    float xf01 = _fnLerp(_fnGradCoord3D(seed, x0, y0, z1, xd0, yd0, zd1), _fnGradCoord3D(seed, x1, y0, z1, xd1, yd0, zd1), xs);
    float xf11 = _fnLerp(_fnGradCoord3D(seed, x0, y1, z1, xd0, yd1, zd1), _fnGradCoord3D(seed, x1, y1, z1, xd1, yd1, zd1), xs);
         
    float yf0 = _fnLerp(xf00, xf10, ys);
    float yf1 = _fnLerp(xf01, xf11, ys);

    return _fnLerp(yf0, yf1, zs) * 0.964921414852142333984375f;
}

// Value Cubic

static float _fnSingleValueCubic2D(int seed, FNfloat x, FNfloat y) {
    int x1 = _fnFastFloor(x);
    int y1 = _fnFastFloor(y);

    float xs = x - (float) x1;
    float ys = y - (float) y1;

    x1 *= PRIME_X;
    y1 *= PRIME_Y;

    int x0 = x1 - PRIME_X;
    int y0 = y1 - PRIME_Y;
    int x2 = x1 + PRIME_X;
    int y2 = y1 + PRIME_Y;
    int x3 = x1 + PRIME_X * 2;
    int y3 = y1 + PRIME_Y * 2;

    return _fnCubicLerp(
               _fnCubicLerp(_fnValCoord2D(seed, x0, y0), _fnValCoord2D(seed, x1, y0), _fnValCoord2D(seed, x2, y0), _fnValCoord2D(seed, x3, y0),
                   xs),
               _fnCubicLerp(_fnValCoord2D(seed, x0, y1), _fnValCoord2D(seed, x1, y1), _fnValCoord2D(seed, x2, y1), _fnValCoord2D(seed, x3, y1),
                   xs),
               _fnCubicLerp(_fnValCoord2D(seed, x0, y2), _fnValCoord2D(seed, x1, y2), _fnValCoord2D(seed, x2, y2), _fnValCoord2D(seed, x3, y2),
                   xs),
               _fnCubicLerp(_fnValCoord2D(seed, x0, y3), _fnValCoord2D(seed, x1, y3), _fnValCoord2D(seed, x2, y3), _fnValCoord2D(seed, x3, y3),
                   xs),
               ys) * (1 / (1.5f * 1.5f));
}

static float _fnSingleValueCubic3D(int seed, FNfloat x, FNfloat y, FNfloat z) {
    int x1 = _fnFastFloor(x);
    int y1 = _fnFastFloor(y);
    int z1 = _fnFastFloor(z);

    float xs = x - (float)x1;
    float ys = y - (float)y1;
    float zs = z - (float)z1;

    x1 *= PRIME_X;
    y1 *= PRIME_Y;
    z1 *= PRIME_Z;

    int x0 = x1 - PRIME_X;
    int y0 = y1 - PRIME_Y;
    int z0 = z1 - PRIME_Z;
    int x2 = x1 + PRIME_X;
    int y2 = y1 + PRIME_Y;
    int z2 = z1 + PRIME_Z;
    int x3 = x1 + PRIME_X * 2;
    int y3 = y1 + PRIME_Y * 2;
    int z3 = z1 + PRIME_Z * 2;

    return _fnCubicLerp(
            _fnCubicLerp(
            _fnCubicLerp(_fnValCoord3D(seed, x0, y0, z0), _fnValCoord3D(seed, x1, y0, z0), _fnValCoord3D(seed, x2, y0, z0), _fnValCoord3D(seed, x3, y0, z0), xs),
            _fnCubicLerp(_fnValCoord3D(seed, x0, y1, z0), _fnValCoord3D(seed, x1, y1, z0), _fnValCoord3D(seed, x2, y1, z0), _fnValCoord3D(seed, x3, y1, z0), xs),
            _fnCubicLerp(_fnValCoord3D(seed, x0, y2, z0), _fnValCoord3D(seed, x1, y2, z0), _fnValCoord3D(seed, x2, y2, z0), _fnValCoord3D(seed, x3, y2, z0), xs),
            _fnCubicLerp(_fnValCoord3D(seed, x0, y3, z0), _fnValCoord3D(seed, x1, y3, z0), _fnValCoord3D(seed, x2, y3, z0), _fnValCoord3D(seed, x3, y3, z0), xs),
                ys),
            _fnCubicLerp(
            _fnCubicLerp(_fnValCoord3D(seed, x0, y0, z1), _fnValCoord3D(seed, x1, y0, z1), _fnValCoord3D(seed, x2, y0, z1), _fnValCoord3D(seed, x3, y0, z1), xs),
            _fnCubicLerp(_fnValCoord3D(seed, x0, y1, z1), _fnValCoord3D(seed, x1, y1, z1), _fnValCoord3D(seed, x2, y1, z1), _fnValCoord3D(seed, x3, y1, z1), xs),
            _fnCubicLerp(_fnValCoord3D(seed, x0, y2, z1), _fnValCoord3D(seed, x1, y2, z1), _fnValCoord3D(seed, x2, y2, z1), _fnValCoord3D(seed, x3, y2, z1), xs),
            _fnCubicLerp(_fnValCoord3D(seed, x0, y3, z1), _fnValCoord3D(seed, x1, y3, z1), _fnValCoord3D(seed, x2, y3, z1), _fnValCoord3D(seed, x3, y3, z1), xs),
                ys),
            _fnCubicLerp(
            _fnCubicLerp(_fnValCoord3D(seed, x0, y0, z2), _fnValCoord3D(seed, x1, y0, z2), _fnValCoord3D(seed, x2, y0, z2), _fnValCoord3D(seed, x3, y0, z2), xs),
            _fnCubicLerp(_fnValCoord3D(seed, x0, y1, z2), _fnValCoord3D(seed, x1, y1, z2), _fnValCoord3D(seed, x2, y1, z2), _fnValCoord3D(seed, x3, y1, z2), xs),
            _fnCubicLerp(_fnValCoord3D(seed, x0, y2, z2), _fnValCoord3D(seed, x1, y2, z2), _fnValCoord3D(seed, x2, y2, z2), _fnValCoord3D(seed, x3, y2, z2), xs),
            _fnCubicLerp(_fnValCoord3D(seed, x0, y3, z2), _fnValCoord3D(seed, x1, y3, z2), _fnValCoord3D(seed, x2, y3, z2), _fnValCoord3D(seed, x3, y3, z2), xs),
                ys),
            _fnCubicLerp(
            _fnCubicLerp(_fnValCoord3D(seed, x0, y0, z3), _fnValCoord3D(seed, x1, y0, z3), _fnValCoord3D(seed, x2, y0, z3), _fnValCoord3D(seed, x3, y0, z3), xs),
            _fnCubicLerp(_fnValCoord3D(seed, x0, y1, z3), _fnValCoord3D(seed, x1, y1, z3), _fnValCoord3D(seed, x2, y1, z3), _fnValCoord3D(seed, x3, y1, z3), xs),
            _fnCubicLerp(_fnValCoord3D(seed, x0, y2, z3), _fnValCoord3D(seed, x1, y2, z3), _fnValCoord3D(seed, x2, y2, z3), _fnValCoord3D(seed, x3, y2, z3), xs),
            _fnCubicLerp(_fnValCoord3D(seed, x0, y3, z3), _fnValCoord3D(seed, x1, y3, z3), _fnValCoord3D(seed, x2, y3, z3), _fnValCoord3D(seed, x3, y3, z3), xs),
                ys),
            zs) * (1 / 1.5f * 1.5f * 1.5f);
}

// Value noise

static FNfloat _fnSingleValue2D(int seed, FNfloat x, FNfloat y) {
    int x0 = _fnFastFloor(x);
    int y0 = _fnFastFloor(y);
    
    float xs = _fnInterpHermite((float)(x - x0));
    float ys = _fnInterpHermite((float)(y - y0));

    x0 *= PRIME_X;
    y0 *= PRIME_Y;
    int x1 = x0 + PRIME_X;
    int y1 = y0 + PRIME_Y;

    float xf0 = _fnLerp(_fnValCoord2D(seed, x0, y0), _fnValCoord2D(seed, x1, y0), xs);
    float xf1 = _fnLerp(_fnValCoord2D(seed, x0, y1), _fnValCoord2D(seed, x1, y1), xs);

    return _fnLerp(xf0, xf1, ys);
}

static float _fnSingleValue3D(int seed, FNfloat x, FNfloat y, FNfloat z) {
    int x0 = _fnFastFloor(x);
    int y0 = _fnFastFloor(y);
    int z0 = _fnFastFloor(z);

    float xs = _fnInterpHermite((float)(x - x0));
    float ys = _fnInterpHermite((float)(y - y0));
    float zs = _fnInterpHermite((float)(z - z0));

    x0 *= PRIME_X;
    y0 *= PRIME_Y;
    z0 *= PRIME_Z;
    int x1 = x0 + PRIME_X;
    int y1 = y0 + PRIME_Y;
    int z1 = z0 + PRIME_Z;

    float xf00 = _fnLerp(_fnValCoord3D(seed, x0, y0, z0), _fnValCoord3D(seed, x1, y0, z0), xs);
    float xf10 = _fnLerp(_fnValCoord3D(seed, x0, y1, z0), _fnValCoord3D(seed, x1, y1, z0), xs);
    float xf01 = _fnLerp(_fnValCoord3D(seed, x0, y0, z1), _fnValCoord3D(seed, x1, y0, z1), xs);
    float xf11 = _fnLerp(_fnValCoord3D(seed, x0, y1, z1), _fnValCoord3D(seed, x1, y1, z1), xs);
        
    float yf0 = _fnLerp(xf00, xf10, ys);
    float yf1 = _fnLerp(xf01, xf11, ys);

    return _fnLerp(yf0, yf1, zs);
}

// ====================
// 2D Gen functions
// ====================

static inline float _fnGenNoiseSingle2D(fn_state *state, int seed, FNfloat x, FNfloat y) {
    switch (state->noise_type) {
        case FN_NOISE_SIMPLEX:
            return _fnSingleSimplex2D(seed, x, y);
        case FN_NOISE_OPENSIMPLEX2:
            // 2D case doesn't need a different algorithm.
            // TODO improve Simplex 2D gradient table, or use improved table for OpenSimplex2 case like in FastNoise2.
            return _fnSingleSimplex2D(seed, x, y);
        case FN_NOISE_CELLULAR:
            return _fnSingleCellular2D(state, seed, x, y);
        case FN_NOISE_PERLIN:
            return _fnSinglePerlin2D(seed, x, y);
        case FN_NOISE_VALUE_CUBIC:
            return _fnSingleValueCubic2D(seed, x, y);
        case FN_NOISE_VALUE:
            return _fnSingleValue2D(seed, x, y);
        default:
            return 0;
    }
}

static float _fnGenFractalFBM2D(fn_state *state, FNfloat x, FNfloat y) {
    int seed = state->seed;
    float sum = 0;
    float amp = _fnCalculateFractalBounding(state);

    for (int i = 0; i < state->octaves; i++) {
        float noise = _fnGenNoiseSingle2D(state, seed++, x, y);
        sum += noise * amp;
        amp *= _fnLerp(1.0f, _fnFastMin(noise + 1, 2) * 0.5f, state->weighted_strength);

        x *= state->lacunarity;
        y *= state->lacunarity;
        amp *= state->gain;
    }

    return sum;
}

static float _fnGenFractalRidged2D(fn_state *state, FNfloat x, FNfloat y) {
    int seed = state->seed;
    float sum = 0;
    float amp = _fnCalculateFractalBounding(state);

    for (int i = 0; i < state->octaves; i++) {
        float noise = _fnFastAbs(_fnGenNoiseSingle2D(state, seed++, x, y));
        sum += (noise * -2 + 1) * amp;
        amp *= _fnLerp(1.0f, 1 - noise, state->weighted_strength);

        x *= state->lacunarity;
        y *= state->lacunarity;
        amp *= state->gain;
    }

    return sum;
}

static float _fnGenFractalPingPong2D(fn_state *state, FNfloat x, FNfloat y) {
    int seed = state->seed;
    float sum = 0;
    float amp = _fnCalculateFractalBounding(state);

    for (int i = 0; i < state->octaves; i++) {
        float noise = _fnPingPong((_fnGenNoiseSingle2D(state, seed++, x, y) + 1) * state->ping_pong_strength);
        sum += (noise - 0.5f) * 2 * amp;
        amp *= _fnLerp(1.0f, noise, state->weighted_strength);

        x *= state->lacunarity;
        y *= state->lacunarity;
        amp *= state->gain;
    }

    return sum;
}

// ====================
// 3D Gen functions
// ====================

static inline float _fnGenNoiseSingle3D(fn_state *state, int seed, FNfloat x, FNfloat y, FNfloat z) {
    switch (state->noise_type) {
        case FN_NOISE_SIMPLEX:
            return _fnSingleSimplex3D(seed, x, y, z);
        case FN_NOISE_OPENSIMPLEX2:
            return _fnSingleOpenSimplex23D(seed, x, y, z);
        case FN_NOISE_CELLULAR:
            return _fnSingleCellular3D(state, seed, x, y, z);
        case FN_NOISE_PERLIN:
            return _fnSinglePerlin3D(seed, x, y, z);
        case FN_NOISE_VALUE_CUBIC:
            return _fnSingleValueCubic3D(seed, x, y, z);
        case FN_NOISE_VALUE:
            return _fnSingleValue3D(seed, x, y, z);
        default:
            return 0;
    }
}

static float _fnGenFractalFBM3D(fn_state *state, FNfloat x, FNfloat y, FNfloat z) {
    int seed = state->seed;
    float sum = 0;
    float amp = _fnCalculateFractalBounding(state);

    for (int i = 0; i < state->octaves; i++) {
        float noise = _fnGenNoiseSingle3D(state, seed++, x, y, z);
        sum += noise * amp;
        amp *= _fnLerp(1.0f, (noise + 1) * 0.5f, state->weighted_strength);

        x *= state->lacunarity;
        y *= state->lacunarity;
        z *= state->lacunarity;
        amp *= state->gain;
    }

    return sum;
}

static float _fnGenFractalRidged3D(fn_state *state, FNfloat x, FNfloat y, FNfloat z) {
    int seed = state->seed;
    float sum = 0;
    float amp = _fnCalculateFractalBounding(state);

    for (int i = 0; i < state->octaves; i++) {
        float noise = _fnFastAbs(_fnGenNoiseSingle3D(state, seed++, x, y, z));
        sum += (noise * -2 + 1) * amp;
        amp *= _fnLerp(1.0f, 1 - noise, state->weighted_strength);

        x *= state->lacunarity;
        y *= state->lacunarity;
        z *= state->lacunarity;
        amp *= state->gain;
    }

    return sum;
}

static float _fnGenFractalPingPong3D(fn_state *state, FNfloat x, FNfloat y, FNfloat z) {
    int seed = state->seed;
    float sum = 0;
    float amp = _fnCalculateFractalBounding(state);

    for (int i = 0; i < state->octaves; i++) {
        float noise = _fnPingPong((_fnGenNoiseSingle3D(state, seed++, x, y, z) + 1) * state->ping_pong_strength);
        sum += (noise - 0.5f) * 2 * amp;
        amp *= _fnLerp(1.0f, noise, state->weighted_strength);

        x *= state->lacunarity;
        y *= state->lacunarity;
        z *= state->lacunarity;
        amp *= state->gain;
    }

    return sum;
}

// ====================
// Domain Warp
// ====================

// Domain Warp Basic Grid

static void _fnSingleDomainWarpBasicGrid2D(int seed, float warpAmp, float frequency, FNfloat x, FNfloat y, FNfloat *xp, FNfloat *yp) {
    FNfloat xf = x * frequency;
    FNfloat yf = y * frequency;

    int x0 = _fnFastFloor(xf);
    int y0 = _fnFastFloor(yf);

    float xs = _fnInterpHermite((float)(xf - x0));
    float ys = _fnInterpHermite((float)(yf - y0));

    x0 *= PRIME_X;
    y0 *= PRIME_Y;
    int x1 = x0 + PRIME_X;
    int y1 = y0 + PRIME_Y;

    int idx0 = _fnHash2D(seed, x0, y0) & (255 << 1);
    int idx1 = _fnHash2D(seed, x1, y0) & (255 << 1);

    float lx0x = _fnLerp(RAND_VECS_2D[idx0], RAND_VECS_2D[idx1], xs);
    float ly0x = _fnLerp(RAND_VECS_2D[idx0 | 1], RAND_VECS_2D[idx1 | 1], xs);

    idx0 = _fnHash2D(seed, x0, y1) & (255 << 1);
    idx1 = _fnHash2D(seed, x1, y1) & (255 << 1);

    float lx1x = _fnLerp(RAND_VECS_2D[idx0], RAND_VECS_2D[idx1], xs);
    float ly1x = _fnLerp(RAND_VECS_2D[idx0 | 1], RAND_VECS_2D[idx1 | 1], xs);

    *xp += _fnLerp(lx0x, lx1x, ys) * warpAmp;
    *yp += _fnLerp(ly0x, ly1x, ys) * warpAmp;
}

static void _fnSingleDomainWarpBasicGrid3D(int seed, float warpAmp, float frequency, FNfloat x, FNfloat y, FNfloat z, FNfloat *xp, FNfloat *yp, FNfloat *zp) {
    FNfloat xf = x * frequency;
    FNfloat yf = y * frequency;
    FNfloat zf = z * frequency;

    int x0 = _fnFastFloor(xf);
    int y0 = _fnFastFloor(yf);
    int z0 = _fnFastFloor(zf);

    float xs = _fnInterpHermite((float)(xf - x0));
    float ys = _fnInterpHermite((float)(yf - y0));
    float zs = _fnInterpHermite((float)(zf - z0));

    x0 *= PRIME_X;
    y0 *= PRIME_Y;
    z0 *= PRIME_Z;
    int x1 = x0 + PRIME_X;
    int y1 = y0 + PRIME_Y;
    int z1 = z0 + PRIME_Z;

    int idx0 = _fnHash3D(seed, x0, y0, z0) & (255 << 2);
    int idx1 = _fnHash3D(seed, x1, y0, z0) & (255 << 2);

    float lx0x = _fnLerp(RAND_VECS_3D[idx0], RAND_VECS_3D[idx1], xs);
    float ly0x = _fnLerp(RAND_VECS_3D[idx0 | 1], RAND_VECS_3D[idx1 | 1], xs);
    float lz0x = _fnLerp(RAND_VECS_3D[idx0 | 2], RAND_VECS_3D[idx1 | 2], xs);

    idx0 = _fnHash3D(seed, x0, y1, z0) & (255 << 2);
    idx1 = _fnHash3D(seed, x1, y1, z0) & (255 << 2);

    float lx1x = _fnLerp(RAND_VECS_3D[idx0], RAND_VECS_3D[idx1], xs);
    float ly1x = _fnLerp(RAND_VECS_3D[idx0 | 1], RAND_VECS_3D[idx1 | 1], xs);
    float lz1x = _fnLerp(RAND_VECS_3D[idx0 | 2], RAND_VECS_3D[idx1 | 2], xs);
        
    float lx0y = _fnLerp(lx0x, lx1x, ys);
    float ly0y = _fnLerp(ly0x, ly1x, ys);
    float lz0y = _fnLerp(lz0x, lz1x, ys);

    idx0 = _fnHash3D(seed, x0, y0, z1) & (255 << 2);
    idx1 = _fnHash3D(seed, x1, y0, z1) & (255 << 2);

    lx0x = _fnLerp(RAND_VECS_3D[idx0], RAND_VECS_3D[idx1], xs);
    ly0x = _fnLerp(RAND_VECS_3D[idx0 | 1], RAND_VECS_3D[idx1 | 1], xs);
    lz0x = _fnLerp(RAND_VECS_3D[idx0 | 2], RAND_VECS_3D[idx1 | 2], xs);

    idx0 = _fnHash3D(seed, x0, y1, z1) & (255 << 2);
    idx1 = _fnHash3D(seed, x1, y1, z1) & (255 << 2);

    lx1x = _fnLerp(RAND_VECS_3D[idx0], RAND_VECS_3D[idx1], xs);
    ly1x = _fnLerp(RAND_VECS_3D[idx0 | 1], RAND_VECS_3D[idx1 | 1], xs);
    lz1x = _fnLerp(RAND_VECS_3D[idx0 | 2], RAND_VECS_3D[idx1 | 2], xs);

    *xp += _fnLerp(lx0y, _fnLerp(lx0x, lx1x, ys), zs) * warpAmp;
    *yp += _fnLerp(ly0y, _fnLerp(ly0x, ly1x, ys), zs) * warpAmp;
    *zp += _fnLerp(lz0y, _fnLerp(lz0x, lz1x, ys), zs) * warpAmp;
}

// Domain Warp Simplex

static void _fnSingleDomainWarpSimplexGradient(int seed, float warpAmp, float frequency, FNfloat x, FNfloat y, FNfloat *xr, FNfloat *yr, bool outGradOnly) {
    const FNfloat SQRT3 = (FNfloat)1.7320508075688772935274463415059;
    const FNfloat F2 = 0.5f * (SQRT3 - 1);
    const FNfloat G2 = (3 - SQRT3) / 6;

    x *= frequency;
    y *= frequency;

    FNfloat t = (x + y) * F2;
    int i = _fnFastFloor(x + t);
    int j = _fnFastFloor(y + t);

    t = (i + j) * G2;
    float x0 = (float)(x - (i - t));
    float y0 = (float)(y - (j - t));

    float y0_1 = y0 - 1;
    int i1 = (int)(y0_1 - x0);
    int j1 = ~i1;

    float x1 = x0 + i1 + (float)G2;
    float y1 = y0 + j1 + (float)G2;
    float x2 = x0 - 1 + 2 * (float)G2;
    float y2 = y0_1 + 2 * (float)G2;

    i *= PRIME_X;
    j *= PRIME_Y;

    i1 &= PRIME_X;
    j1 &= PRIME_Y;

    float vx, vy;
    vx = vy = 0;

    float a = 0.5f - x0 * x0 - y0 * y0;
    if (a > 0)
    {
        a *= a; a *= a;
        float xo, yo;
        if (outGradOnly)
            _fnGradCoordOut2D(seed, i, j, &xo, &yo);
        else
            _fnGradCoordDual2D(seed, i, j, x0, y0, &xo, &yo);
        vx += a * xo;
        vy += a * yo;
    }

    float b = 0.5f - x1 * x1 - y1 * y1;
    if (b > 0)
    {
        b *= b; b *= b;
        float xo, yo;
        if (outGradOnly)
            _fnGradCoordOut2D(seed, i + i1, j + j1, &xo, &yo);
        else
            _fnGradCoordDual2D(seed, i + i1, j + j1, x1, y1, &xo, &yo);
        vx += b * xo;
        vy += b * yo;
    }

    float c = 0.5f - x2 * x2 - y2 * y2;
    if (c > 0)
    {
        c *= c; c *= c;
        float xo, yo;
        if (outGradOnly)
            _fnGradCoordOut2D(seed, i + PRIME_X, j + PRIME_Y, &xo, &yo);
        else
            _fnGradCoordDual2D(seed, i + PRIME_X, j + PRIME_Y, x2, y2, &xo, &yo);
        vx += c * xo;
        vy += c * yo;
    }

    *xr += vx * warpAmp;
    *yr += vy * warpAmp;
}

static void _fnSingleDomainWarpOpenSimplex2Gradient(int seed, float warpAmp, float frequency, FNfloat x, FNfloat y, FNfloat z, FNfloat *xr, FNfloat *yr, FNfloat *zr, bool outGradOnly) {
    const FNfloat R3 = (FNfloat)(2.0 / 3.0);

    x *= frequency;
    y *= frequency;
    z *= frequency;

    FNfloat r = (x + y + z) * R3; // Rotation, not skew
    x = r - x; y = r - y; z = r - z;

    int i = _fnFastRound(x);
    int j = _fnFastRound(y);
    int k = _fnFastRound(z);
    float x0 = (float)x - i;
    float y0 = (float)y - j;
    float z0 = (float)z - k;

    int xNSign = (int)(-x0 - 1.0f) | 1;
    int yNSign = (int)(-y0 - 1.0f) | 1;
    int zNSign = (int)(-z0 - 1.0f) | 1;

    float ax0 = xNSign * -x0;
    float ay0 = yNSign * -y0;
    float az0 = zNSign * -z0;

    i *= PRIME_X;
    j *= PRIME_Y;
    k *= PRIME_Z;

    float vx, vy, vz;
    vx = vy = vz = 0;

    float a = (0.6f - x0 * x0) - (y0 * y0 + z0 * z0);
    for (int l = 0; l < 2; l++) {
        if (a > 0) {
            float aaaa = (a * a) * (a * a);
            float xo, yo, zo;
            if (outGradOnly)
                _fnGradCoordOut3D(seed, i, j, k, &xo, &yo, &zo);
            else
                _fnGradCoordDual3D(seed, i, j, k, x0, y0, z0, &xo, &yo, &zo);
            vx += aaaa * xo;
            vy += aaaa * yo;
            vz += aaaa * zo;
        }

        float b = a + 1;
        int i1 = i;
        int j1 = j;
        int k1 = k;
        float x1 = x0;
        float y1 = y0;
        float z1 = z0;
        if (ax0 >= ay0 && ax0 >= az0) {
            x1 += xNSign;
            b -= xNSign * 2 * x1;
            i1 -= xNSign * PRIME_X;
        } else if (ay0 > ax0 && ay0 >= az0) {
            y1 += yNSign;
            b -= yNSign * 2 * y1;
            j1 -= yNSign * PRIME_Y;
        } else {
            z1 += zNSign;
            b -= zNSign * 2 * z1;
            k1 -= zNSign * PRIME_Z;
        }

        if (b > 0) {
            float bbbb = (b * b) * (b * b);
            float xo, yo, zo;
            if (outGradOnly)
                _fnGradCoordOut3D(seed, i1, j1, k1, &xo, &yo, &zo);
            else
                _fnGradCoordDual3D(seed, i1, j1, k1, x1, y1, z1, &xo, &yo, &zo);
            vx += bbbb * xo;
            vy += bbbb * yo;
            vz += bbbb * zo;
        }

        if (l == 1) break;

        ax0 = 0.5f - ax0;
        ay0 = 0.5f - ay0;
        az0 = 0.5f - az0;

        x0 = xNSign * ax0;
        y0 = yNSign * ay0;
        z0 = zNSign * az0;

        a += (0.75f - ax0) - (ay0 + az0);

        i += (xNSign >> 1) & PRIME_X;
        j += (yNSign >> 1) & PRIME_Y;
        k += (zNSign >> 1) & PRIME_Z;

        xNSign = -xNSign;
        yNSign = -yNSign;
        zNSign = -zNSign;

        seed += 1293373;
    }

    *xr += vx * warpAmp;
    *yr += vy * warpAmp;
    *zr += vz * warpAmp;
}

// Perform domain warp

static inline void _fnDoSingleDomainWarp2D(fn_state *state, int seed, float amp, float freq, FNfloat x, FNfloat y, FNfloat *xp, FNfloat *yp) {
    switch (state->domain_warp_type) {
        case FN_DOMAIN_WARP_OPENSIMPLEX2:
            _fnSingleDomainWarpSimplexGradient(seed, amp * 38.283687591552734375f, freq, x, y, xp, yp, false);
            break;
        case FN_DOMAIN_WARP_OPENSIMPLEX2_REDUCED:
            _fnSingleDomainWarpSimplexGradient(seed, amp * 16.0f, freq, x, y, xp, yp, true);
            break;
        case FN_DOMAIN_WARP_BASICGRID:
            _fnSingleDomainWarpBasicGrid2D(seed, amp, freq, x, y, xp, yp);
            break;
    }
}

static inline void _fnDoSingleDomainWarp3D(fn_state *state, int seed, float amp, float freq, FNfloat x, FNfloat y, FNfloat z, FNfloat *xp, FNfloat *yp, FNfloat *zp) {
    switch (state->domain_warp_type) {
        case FN_DOMAIN_WARP_OPENSIMPLEX2:
            _fnSingleDomainWarpOpenSimplex2Gradient(seed, amp * 32.69428253173828125f, freq, x, y, z, xp, yp, zp, false);
            break;
        case FN_DOMAIN_WARP_OPENSIMPLEX2_REDUCED:
            _fnSingleDomainWarpOpenSimplex2Gradient(seed, amp * 7.71604938271605f, freq, x, y, z, xp, yp, zp, true);
            break;
        case FN_DOMAIN_WARP_BASICGRID:
            _fnSingleDomainWarpBasicGrid3D(seed, amp, freq, x, y, z, xp, yp, zp);
            break;
    }
}

// Domain Warp Fractal Progressive

static void _fnDomainWarpFractalProgressive2D(fn_state *state, FNfloat *x, FNfloat *y) {
    int seed = state->seed;
    float amp = state->domain_warp_amp * _fnCalculateFractalBounding(state);
    float freq = state->frequency;

    for (int i = 0; i < state->octaves; i++) {
        _fnDoSingleDomainWarp2D(state, seed, amp, freq, *x, *y, x, y);

        seed++;
        amp *= state->gain;
        freq *= state->lacunarity;
    }
}

static void _fnDomainWarpFractalProgressive3D(fn_state *state, FNfloat *x, FNfloat *y, FNfloat *z) {
    int seed = state->seed;
    float amp = state->domain_warp_amp * _fnCalculateFractalBounding(state);
    float freq = state->frequency;

    for (int i = 0; i < state->octaves; i++) {
        _fnDoSingleDomainWarp3D(state, seed, amp, freq, *x, *y, *z, x, y, z);

        seed++;
        amp *= state->gain;
        freq *= state->lacunarity;
    }
}

// Domain Warp Fractal Independent

static void _fnDomainWarpFractalIndependent2D(fn_state *state, FNfloat *x, FNfloat *y) {
    FNfloat xs = *x;
    FNfloat ys = *y;

    int seed = state->seed;
    float amp = state->domain_warp_amp * _fnCalculateFractalBounding(state);
    float freq = state->frequency;

    for (int i = 0; i < state->octaves; i++) {
        _fnDoSingleDomainWarp2D(state, seed, amp, freq, xs, ys, x, y);

        seed++;
        amp *= state->gain;
        freq *= state->lacunarity;
    }
}

static void _fnDomainWarpFractalIndependent3D(fn_state *state, FNfloat *x, FNfloat *y, FNfloat *z) {
    FNfloat xs = *x;
    FNfloat ys = *y;
    FNfloat zs = *z;

    int seed = state->seed;
    float amp = state->domain_warp_amp * _fnCalculateFractalBounding(state);
    float freq = state->frequency;

    for (int i = 0; i < state->octaves; i++) {
        _fnDoSingleDomainWarp3D(state, seed, amp, freq, xs, ys, zs, x, y, z);

        seed++;
        amp *= state->gain;
        freq *= state->lacunarity;
    }
}

// ====================
// Public API
// ====================

fn_state fnCreateState(int seed) {
    fn_state newState;
    newState.seed = seed;
    newState.frequency = 0.01f;
    newState.noise_type = FN_NOISE_OPENSIMPLEX2;
    newState.fractal_type = FN_FRACTAL_NONE;
    newState.octaves = 3;
    newState.lacunarity = 2.0f;
    newState.gain = 0.5f;
    newState.weighted_strength = 0.0f;
    newState.ping_pong_strength = 2.0f;
    newState.cellular_distance_func = FN_CELLULAR_DIST_EUCLIDEANSQ;
    newState.cellular_return_type = FN_CELLULAR_RET_DISTANCE;
    newState.cellular_jitter_mod = 1.0f;
    newState.domain_warp_amp = 30.0f;
    newState.domain_warp_type = FN_DOMAIN_WARP_OPENSIMPLEX2;
    return newState;
}

float fnGetNoise2D(fn_state *state, FNfloat x, FNfloat y) {
    x *= state->frequency;
    y *= state->frequency;

    switch (state->fractal_type) {
        default:
            return _fnGenNoiseSingle2D(state, state->seed, x, y);
        case FN_FRACTAL_FBM:
            return _fnGenFractalFBM2D(state, x, y);
        case FN_FRACTAL_RIDGED:
            return _fnGenFractalRidged2D(state, x, y);
        case FN_FRACTAL_PINGPONG:
            return _fnGenFractalPingPong2D(state, x, y);
    }
}

float fnGetNoise3D(fn_state *state, FNfloat x, FNfloat y, FNfloat z) {
    x *= state->frequency;
    y *= state->frequency;
    z *= state->frequency;

    // Select a noise type
    switch (state->fractal_type) {
        default:
            return _fnGenNoiseSingle3D(state, state->seed, x, y, z);
        case FN_FRACTAL_FBM:
            return _fnGenFractalFBM3D(state, x, y, z);
        case FN_FRACTAL_RIDGED:
            return _fnGenFractalRidged3D(state, x, y, z);
        case FN_FRACTAL_PINGPONG:
            return _fnGenFractalPingPong3D(state, x, y, z);
    }
}

void fnDomainWarp2D(fn_state *state, FNfloat *x, FNfloat *y) {
    switch (state->fractal_type) {
        default:
            _fnDoSingleDomainWarp2D(state, state->seed, state->domain_warp_amp, state->frequency, *x, *y, x, y);
            break;
        case FN_FRACTAL_DOMAIN_WARP_PROGRESSIVE:
            _fnDomainWarpFractalProgressive2D(state, x, y);
            break;
        case FN_FRACTAL_DOMAIN_WARP_INDEPENDENT:
            _fnDomainWarpFractalIndependent2D(state, x, y);
            break;
    }
}

void fnDomainWarp3D(fn_state *state, FNfloat *x, FNfloat *y, FNfloat *z) {
    switch (state->fractal_type) {
        default:
            _fnDoSingleDomainWarp3D(state, state->seed, state->domain_warp_amp, state->frequency, *x, *y, *z, x, y, z);
            break;
        case FN_FRACTAL_DOMAIN_WARP_PROGRESSIVE:
            _fnDomainWarpFractalProgressive3D(state, x, y, z);
            break;
        case FN_FRACTAL_DOMAIN_WARP_INDEPENDENT:
            _fnDomainWarpFractalIndependent3D(state, x, y, z);
            break;
    }
}

#endif

#if defined(__cplusplus)
}
#endif
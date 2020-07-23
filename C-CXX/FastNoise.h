/*
 * C Header-Only FastNoise Lite Implementation.
 */

// Config
//#define FN_USE_DOUBLE

#if defined(__cplusplus)
extern "C" {
#endif

#include <math.h>
#include <stdint.h>
#include <float.h>

// So that VS code doesnt hide my work :(
#define FN_IMPL

// Typedefs
#if defined(FN_USE_DOUBLE)
typedef double FNfloat;
#define FN_FLOAT_MAX DBL_MAX
#else
typedef float FNfloat;
#define FN_FLOAT_MAX FLT_MAX
#endif

// Enums
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
    FN_FRACTAL_RIDGED,
    FN_FRACTAL_DOMAIN_WARP_PROGRESSIVE,
    FN_FRACTAL_DOMAIN_WARP_INDEPENDANT
} fn_fractal_type;

typedef enum {
    FN_CELLULAR_DIST_EUCLIDEAN,
    FN_CELLULAR_DIST_MANHATTAN,
    FN_CELLULAR_DIST_NATURAL
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
    FN_DOMAIN_WARP_GRADIENT,
    FN_DOMAIN_WARP_SIMPLEX
} fn_domain_warp_type;

// State struct
typedef struct fn_state {
    int seed;
    FNfloat frequency;
    fn_noise_type noise_type;

    int octaves;
    FNfloat lacunarity;
    FNfloat gain;
    fn_fractal_type fractal_type;
    
    fn_cellular_distance_func cellular_distance_func;
    fn_cellular_return_type cellular_return_type;
    FNfloat cellular_jitter_mod;

    fn_domain_warp_type domain_warp_type;
    FNfloat domain_warp_amp;
} fn_state;

// Creates a state with default values set
fn_state fnCreateState(int seed = 1337);

// Simple function for getting the noise
FNfloat fnGetNoise2D(fn_state *state, FNfloat x, FNfloat y);
FNfloat fnGetNoise3D(fn_state *state, FNfloat x, FNfloat y, FNfloat z);

// Domain warp
void fnDomainWarp2D(fn_state *state, FNfloat *x, FNfloat *y);
void fnDomainWarp3D(fn_state *state, FNfloat *x, FNfloat *y, FNfloat *z);

#if defined(FN_IMPL)

// ====================
// Private API
// ====================

static FNfloat _fnCalculateFractalBounding(fn_state *state) {
    FNfloat amp = state->gain;
    FNfloat ampFractal = 1;
    for (int i = 0; i < state->octaves; i++) {
        ampFractal += amp;
        amp *= state->gain;
    }
    return 1 / ampFractal;
}

// ====================
// Utilities
// ====================

//#define FAST_FLOOR(f) (f >= 0 ? (int) f : (int) (f) - 1;)

static inline int _fnFastFloor(FNfloat f) { return (f >= 0 ? (int)f : (int)f - 1); }

static inline int _fnFastRound(FNfloat f) { return (f >= 0) ? (int)(f + 0.5f) : (int)(f - 0.5f); }

static inline FNfloat _fnLerp(FNfloat a, FNfloat b, FNfloat t) { return a + t * (b - a); }

static inline FNfloat _fnInterpHermite(FNfloat t) { return t * t * (3 - 2 * t); }

static inline FNfloat _fnInterpQuintic(FNfloat t) { return t * t * t * (t * (t * 6 - 15) + 10); }

static inline FNfloat _fnCubicLerp(FNfloat a, FNfloat b, FNfloat c, FNfloat d, FNfloat t) {
    FNfloat p = (d - c) - (a - b);
    return t * t * t * p + t * t * ((a - b) - p) + t * (c - a) + b;
}

static inline FNfloat _fnFastMin(FNfloat x, FNfloat y) { return x < y ? x : y; }

static inline FNfloat _fnFastMax(FNfloat x, FNfloat y) { return x > y ? x : y; }

// ====================
// ABS Optimization
// ====================

#if defined(FN_USE_DOUBLE)
#define NF_ABS absf
#else
#define FN_ABS fabsf
#endif

// ====================
// Hashing
// ====================

static const int PRIME_X = 1619;
static const int PRIME_Y = 31337;
static const int PRIME_Z = 6971;

static inline int _fnHash2D(int seed, int xPrimed, int yPrimed) {
    int hash = seed ^ xPrimed ^ yPrimed;

    hash *= 0x27d4eb2d;
    hash ^= hash >> 15;
    return hash;
}

static inline int _fnHash3D(int seed, int xPrimed, int yPrimed, int zPrimed) {
    int hash = seed ^ xPrimed ^ yPrimed ^ zPrimed;

    hash *= 0x27d4eb2d;
    hash ^= hash >> 15;
    return hash;
}

static inline FNfloat _fnValCoord2D(int seed, int xPrimed, int yPrimed) {
    int hash = seed ^ xPrimed ^ yPrimed;

    hash *= hash * 0x27d4eb2d;
    return hash / (FNfloat) 2147483648.0;
}

static inline FNfloat _fnValCoord3D(int seed, int xPrimed, int yPrimed, int zPrimed) {
    int hash = seed ^ xPrimed ^ yPrimed ^ zPrimed;

    hash *= hash * 0x27d4eb2d;
    return hash / (FNfloat) 2147483648.0;
}

static const FNfloat ROOT2 = (FNfloat) 1.4142135623730950488f;

static const FNfloat GRAD_2D_X[] = {
    1 + ROOT2, -1-ROOT2,   1+ROOT2, -1-ROOT2,
    1,          1,        -1,       -1
};

static const FNfloat GRAD_2D_Y[] = {
    1,          1,        -1,       -1,
    1 + ROOT2, -1-ROOT2,   1+ROOT2, -1-ROOT2
};

static inline FNfloat _fnGradCoord2D(int seed, int xPrimed, int yPrimed, FNfloat xd, FNfloat yd) {
    int idx = _fnHash2D(seed, xPrimed, yPrimed) & 7;
    FNfloat gx = GRAD_2D_X[idx];
    FNfloat gy = GRAD_2D_Y[idx];

    return xd * gx + yd * gy;
}

static const FNfloat GRAD_3D_X[] = {
    1, -1, 1, -1,
    1, -1, 1, -1,
    0, 0, 0, 0
};

static const FNfloat GRAD_3D_Y[] = {
	1, 1, -1, -1,
	0, 0, 0, 0,
	1, -1, 1, -1
};

static const FNfloat GRAD_3D_Z[] = {
	0, 0, 0, 0,
	1, 1, -1, -1,
	1, 1, -1, -1
};

static inline FNfloat _fnGradCoord3D(int seed, int xPrimed, int yPrimed, int zPrimed, FNfloat xd, FNfloat yd, FNfloat zd) {
    int idx = _fnHash3D(seed, xPrimed, yPrimed, zPrimed) & 15;
    FNfloat gx = GRAD_3D_X[idx];
    FNfloat gy = GRAD_3D_Y[idx];
    FNfloat gz = GRAD_3D_Z[idx];

    return xd * gx + yd * gy + zd * gz;
}

// 2D cellular offsets

static const FNfloat CELLULAR_OFFSETS_2D_X[] = {
    -0.2700222198,0.3863092627,0.04444859006,-0.5992523158,-0.7819280288,0.9464672271,-0.6514146797,0.9378472289,
    -0.8497875957,-0.879042592,-0.892300288,-0.379844434,-0.9951650832,0.7724397808,0.7573283322,-0.9928004525,
    -0.0532665713,0.9754253726,-0.7665018163,0.991636706,-0.994696838,-0.5379205513,0.5022815471,0.4559821461,
    -0.8659131224,0.0879458407,-0.5051684983,0.7753185226,-0.6921944612,-0.5191659449,0.8978622882,-0.1706774107,
    -0.9353430106,-0.9992404798,-0.2882064021,-0.9663811329,-0.8759714238,-0.8303123018,0.05110133755,-0.8558373281,
    0.09887025282,0.9189016087,-0.2439375892,-0.8121409387,-0.9910431363,0.8492423985,-0.9717838994,0.9949457207,
    0.6241065508,0.662910307,-0.7197418176,-0.8143370775,0.104521054,-0.1065926113,0.445799684,0.105547406,
    -0.992790267,-0.8334366408,0.9115561563,0.8285544909,0.7217097654,0.4940492677,-0.3652321272,-0.9696606758,
    0.08925509731,0.5354071276,-0.1053576186,-0.9890284586,0.004856104961,0.9885598478,0.9286129562,-0.5832393863,
    0.3015207509,-0.9575110528,0.9715802154,0.229981792,0.955763816,0.740956116,-0.9971513787,0.6905710663,
    -0.290713703,0.5912777791,-0.9454592212,0.6664455681,0.6236134912,0.9126993851,-0.8191762011,-0.8812745759,
    0.9953313627,0.9855650846,-0.8495980887,0.6174853946,0.8508156371,0.9985032451,0.1971371563,0.6607855748,
    -0.03097494063,-0.6731660801,-0.7195018362,0.9727511689,0.9997059088,0.4421787429,0.9981350961,-0.9173660799,
    -0.8150056635,-0.8789331304,0.0158605829,-0.8095464474,-0.9165898907,-0.8023542565,-0.5176737917,-0.8154407307,
    0.4022010347,-0.9052556868,0.7317445619,-0.5647632201,-0.8403276335,-0.9314281527,0.5238198472,0.7432803869,
    -0.985371561,0.4601468731,0.825855404,0.6182366099,0.8331502863,0.1500307506,-0.662330369,-0.668598664,
    0.7025606278,-0.5419389763,-0.3388616456,0.8331530315,-0.2989720662,0.2638522993,0.124108739,-0.7282649308,
    0.6962500149,-0.9183535368,-0.6326102274,-0.9331891859,-0.1153779357,0.9514974788,-0.08987977445,0.6678496916,
    0.7952400393,-0.6462007402,-0.2733598753,0.9669590226,-0.9792894595,-0.5369502995,-0.270036471,-0.6400277131,
    -0.7854537493,0.06005905383,-0.02455770378,-0.65983623,-0.6253894466,-0.6210408851,0.8348888491,-0.1592275245,
    0.8367622488,-0.8675753916,-0.2022662628,0.9399189937,0.9877404807,-0.9034455656,0.1269804218,-0.3819600854,
    0.9754625894,-0.3204015856,-0.9874760884,0.02535348474,0.4835130794,-0.2850799925,-0.06805516006,-0.7885244045,
    0.3185392127,0.8880043089,0.6476921488,0.9820241299,0.9357275128,-0.8894895414,0.7922791302,0.7483818261,
    -0.7288929755,0.8729032783,0.8288345784,0.08074567077,0.9799148216,-0.580730673,-0.4700049791,0.2409492979,
    0.9437816757,-0.8927998638,-0.8069622304,0.06258973166,-0.9312597469,0.5777449785,-0.3360095855,0.697932075,
    -0.002008157227,-0.1827294312,-0.6523911722,-0.4302626911,-0.9985126289,-0.01028102172,-0.4946071129,-0.2999350194,
    0.8165471961,0.2697460475,-0.7306287391,-0.7590952064,-0.907053853,-0.5104861064,0.8613350597,0.5007881595,
    -0.654158152,-0.8382755311,0.6940070834,0.06950936031,0.1702942185,0.2695973274,0.5519612192,0.225657487,
    0.4215262855,0.4881873305,-0.3683854996,-0.9825390578,0.81256471,0.3196460933,0.9570913859,-0.6876655497,
    -0.9988770922,-0.1250179027,-0.8280133617,0.9324863769,0.6394653183,-0.01623847064,-0.9955014666,-0.81453315,
    0.4037327978,0.9944263371,-0.1624711654,-0.9949487814,-0.6995302564,0.5263414922,-0.5395221479,0.6579370318,
    0.01426758847,-0.6734383991,0.639412098,0.9211571421,-0.146637214,-0.782318098,-0.5039610839,-0.7743120191
};

static const FNfloat CELLULAR_OFFSETS_2D_Y[] = {
    -0.9628540911,-0.9223693152,-0.999011673,-0.8005602176,0.6233687174,0.3227999196,-0.7587218957,0.347048376,
    -0.5271252623,0.4767432447,-0.4514423508,-0.9250503802,0.0982163789,-0.6350880136,-0.6530343002,-0.119780055,
    0.9985803285,-0.2203300762,0.6422421394,0.1290606184,0.1028503788,-0.84299554,-0.8647041387,-0.8899889226,
    -0.5001944266,-0.9961252577,0.8630207346,-0.6315704146,0.7217110418,-0.8546734591,-0.4402764035,0.9853269617,
    -0.3537420705,0.03896746794,-0.9575683108,0.2571137995,-0.4823630009,-0.5572983775,-0.9986934731,-0.5172450752,
    0.9951003332,0.3944867976,-0.9697909324,-0.5834613061,0.1335421355,-0.5280031709,-0.2358729591,0.1004142068,
    -0.7813392434,0.7486988212,0.6942418282,-0.5803922158,-0.9945226741,-0.9943027784,-0.8951327509,0.9944142724,
    0.1198644477,0.552615025,-0.4111755999,-0.5599084351,-0.6921957921,-0.8694339084,-0.9309164803,0.2444548501,
    -0.996008799,-0.8445941083,0.9944343981,0.1477251101,0.9999882091,0.1508291331,-0.3710498316,-0.8123003252,
    0.9534596146,0.2883965738,-0.2367105511,0.9731949318,-0.2941352207,0.6715534485,-0.07542630764,-0.7232645452,
    -0.9568100872,-0.8064679708,-0.325740481,0.74555369,0.7817328275,-0.4086316587,0.5735419353,-0.4726046147,
    0.09651672651,-0.1692969699,0.5274306472,-0.7865823463,0.52546432,-0.05469249926,-0.9803759185,-0.7505747292,
    0.9995201614,0.739491331,-0.6944905383,0.2318515979,-0.0242506907,-0.8969269532,-0.061043673,-0.3980445648,
    -0.5794529907,0.4769450202,0.999874213,0.5870558317,-0.3998286786,0.5968480938,0.8555780767,-0.5788405779,
    -0.9155513791,-0.4248672045,0.6815789728,-0.8252529947,-0.5420788397,0.363925262,0.8518290719,-0.6689800195,
    -0.1704197369,0.88784281,0.5638819483,0.7859920446,-0.553046653,0.9886813308,-0.7492119075,0.743623444,
    0.7116238924,-0.8404178401,0.9408362159,0.5530425174,-0.9542618632,0.9645630949,-0.9922686234,-0.6852956957,
    0.7177993569,0.3957610156,-0.7744703352,-0.359385508,-0.9933216659,-0.3076565421,-0.9959526224,0.7442961705,
    -0.6062947138,-0.7631674805,0.9619118351,-0.254931851,0.2024651934,-0.8436138784,-0.9628500944,0.7683518247,
    -0.6189203566,-0.9981948257,0.9996984141,0.751409442,-0.7803127835,-0.7837781695,0.5504185768,0.9872419133,
    0.5475663786,-0.4973056806,-0.9793305667,0.3413975472,-0.1561049093,0.4287028224,-0.9919052235,0.924178821,
    0.2201652486,-0.9472818081,0.1577687387,-0.9996785487,-0.8753371362,-0.9585037287,-0.99768156,-0.6150034663,
    -0.9479096845,0.4598351306,-0.7619021462,0.1887554194,-0.3527237187,0.4569555293,0.6101588153,0.6632681526,
    -0.6846276581,-0.4878932944,0.5594937369,0.9967347374,-0.1994165048,-0.8140957471,-0.8826637636,0.9705377045,
    -0.3305694308,-0.4504535528,0.5906030467,0.9980393407,0.3643559849,0.8162173362,-0.941858566,-0.7161639607,
    -0.9999979837,-0.9831632392,0.7578824173,-0.9027037258,-0.05452091251,-0.9999471489,0.8691166802,0.9539596344,
    0.5772786819,0.962931498,-0.6827749597,-0.6509796216,0.4210146171,-0.8598860013,0.5080373165,-0.8655698812,
    0.7563577938,-0.545246856,0.7199681717,0.9975812994,-0.9853932612,0.9629731466,-0.8338697815,-0.9742067022,
    -0.9068161835,-0.8727388672,-0.9296731273,0.1860564427,0.5828709909,-0.9475370046,0.2897862643,-0.7260276109,
    -0.047376731,0.992154486,0.560708367,-0.3612051451,0.7688199442,-0.9998681473,-0.09474613458,0.580117012,
    -0.9148769469,0.1054336766,0.9867132919,-0.100383875,0.7146029809,-0.85027327,0.841971408,0.7530729462,
    -0.9998982128,0.7392433447,-0.7688642071,0.3891908523,-0.9891903394,0.6228791163,-0.8637263605,-0.6328039957
};

// 3D cellular offsets

static const FNfloat CELLULAR_OFFSETS_3D_X[] = {
    -0.7292736885,0.790292081,0.7217578935,0.565683137,0.760049034,0.3713945616,-0.1277062463,-0.2881560924,
    0.5849561111,0.3307171178,0.8712121778,0.580981015,0.503756873,0.4493712205,-0.6878403724,-0.5958956522,
    -0.5127052122,-0.9911507142,-0.2149721042,-0.7518650936,0.5237068895,0.6333504779,-0.06788241606,-0.5538628599,
    -0.7261455366,-0.9229274737,-0.6968815002,0.9608044783,0.06817146062,-0.3577285196,-0.1868621131,-0.6541697588,
    0.6993340405,-0.1546665739,-0.6841612949,0.5383980957,-0.5026987823,0.3202971715,0.8683227101,0.791120031,
    -0.04011016052,0.9145119872,-0.9612039066,0.6510361721,-0.2041786351,-0.7718263711,-0.3060082741,0.454007341,
    0.4816795475,0.6961980369,-0.6532160499,0.5089301236,-0.1635919754,0.52408019,0.5902587356,0.5863227872,
    0.6779335087,0.7177054546,-0.5328819713,-0.8654874251,-0.2861810166,-0.04849529634,-0.6358521667,0.6254794696,
    -0.2585259868,0.02136306781,0.200111773,0.4743561372,0.6622993731,0.08084972818,-0.6893687501,-0.1121887082,
    0.7344116094,0.5789393465,0.6988182827,-0.8636845529,-0.4247027957,0.5257722489,0.9343830603,-0.1016836419,
    -0.6529238969,-0.2465286165,-0.9776897119,0.7736893337,-0.6095087895,0.5794535482,-0.8226211154,-0.510385483,
    -0.5764971849,0.7250628871,-0.1525424005,-0.5550976208,-0.1883614327,0.7625556724,-0.5870231946,0.3051124198,
    0.6379576059,-0.5966775796,-0.658330573,0.7434892426,0.5562114096,-0.3028940016,0.1129343818,0.5937652891,
    0.6757964092,0.7242302609,-0.9511914166,0.2539470961,0.571433621,-0.06778234979,0.6074972649,-0.5435478392,
    -0.5876678086,-0.7967562402,-0.1996350917,-0.02787660336,-0.7797625996,-0.5211232846,0.8575424857,0.1045223322,
    0.3501822831,0.1987849858,0.7845996363,0.09006737436,-0.8274343547,-0.3485797732,-0.2471778936,-0.3700154943,
    0.5913394901,0.1204873514,0.616959265,0.1258156836,-0.6477565124,0.6217889313,0.6589943422,-0.2689837504,
    -0.3849775103,0.5754444408,0.9141593684,-0.107925319,0.377977089,-0.2142885215,0.5802544474,-0.1982660881,
    -0.03381553704,-0.6867922841,0.7731742607,-0.09655941928,0.2715702457,0.2451015704,0.977700782,-0.5224739938,
    -0.7734880599,-0.7134492443,0.1644037271,0.6494635788,-0.4711970882,-0.6335764307,-0.9021533097,-0.3793711033,
    -0.6855598966,0.2900942212,-0.2098319339,-0.4592603758,0.8715945488,-0.6776666319,0.7044377633,-0.7052318886,
    0.081921007,-0.6993680906,-0.1281454481,-0.6337388239,0.5565051903,-0.5791554484,0.1175779076,-0.6134619607,
    0.6984635305,0.8318082963,0.1469576422,0.708868575,0.2726053183,-0.6591295371,0.4815131379,0.9430009463,
    0.434802957,0.3931996188,0.7254336655,0.7590235227,-0.8552768592,-0.6139746835,-0.7465905486,0.5730065677,
    -0.9225877367,-0.7105816789,-0.08564609717,-0.8279697606,0.427944023,0.5399079671,0.5678404253,-0.9896071041,
    -0.6154342638,0.1074204368,-0.3595449969,-0.2180405296,-0.7277432317,0.7249729114,-0.5553659455,0.5137558015,
    -0.3794024848,0.8229248658,-0.5404114394,0.8036581901,0.6044318879,0.06389988817,0.1087113286,-0.713427712,
    0.6458897819,-0.6553931414,0.3910961323,-0.4879022471,0.6014790094,-0.5677173047,0.7858573506,-0.4520345543,
    0.002272116345,-0.5815751419,-0.03703704785,-0.5119692504,-0.2682417366,-0.2322482736,0.3554328906,0.7346520519,
    0.9985120116,-0.3727687496,0.91737377,0.2714911074,0.5131763485,-0.8737353606,0.8460043821,0.8978727456,
    0.2178065647,-0.1518031304,-0.2600384876,0.572313509,-0.7174141009,-0.684180784,-0.2007447902,-0.1148803697,
    -0.7870349638,-0.2015596421,-0.08581082512,0.5532693395,-0.1842489331,0.0775473789,0.1399838409,0.4484419361
};

static const FNfloat CELLULAR_OFFSETS_3D_Y[] = {
    -0.6618439697,-0.5480887466,0.6226212466,-0.8208298145,-0.5555979497,0.5011264475,-0.4254438999,-0.5815838982,
    -0.662820239,0.0391653737,-0.4113374369,0.7021915846,0.6330056931,0.601390195,0.09018890807,-0.6469350577,
    0.1946921978,-0.05410276466,0.9720882117,-0.5428057603,0.8516377189,0.1926167129,0.3998305789,-0.4729896695,
    -0.5911990757,-0.1782808786,0.6511274338,-0.2098363234,-0.9743405129,-0.6697087264,0.7648617052,0.3967914832,
    -0.6164538506,0.6291283928,-0.2580482182,0.4258654885,-0.7939832935,0.2834415347,-0.0003762656404,-0.08511045745,
    -0.4397248749,0.3579346169,-0.2756484276,-0.2877799159,0.7365237271,0.3790626912,-0.7692987727,-0.5024843065,
    0.6021208291,-0.3222197429,-0.6781148932,-0.6154662304,-0.9133604627,-0.8437664109,0.4983817807,0.494764745,
    0.2341345225,-0.6858979348,-0.5205125012,-0.0700727088,0.7952089234,0.9810836427,0.6058348682,-0.2861619734,
    0.5061949264,0.5064016808,0.8599263484,0.6014985084,-0.5202474575,-0.6532720452,0.0592860349,-0.9673185067,
    0.5979668656,-0.2488849713,0.3557169806,-0.2748771249,-0.4640880967,-0.8427017621,0.316302472,-0.8057303073,
    0.50602126,-0.9668205684,-0.2099250524,0.5734244712,0.4995678998,0.7434546771,0.08142581855,0.4703667658,
    -0.07231656274,0.3949971505,0.4860840828,-0.4957820792,0.9145869398,-0.5414408243,-0.3226498013,0.2262544068,
    0.577242424,0.1454852398,0.6555487542,0.2351084581,0.8264360377,-0.8251527185,-0.985888439,-0.5896813806,
    -0.5835758614,-0.1152719764,0.0753623979,-0.1886339355,-0.1679450851,0.3978269256,0.733060024,0.1675822484,
    -0.3380045064,0.04097822706,0.8706294745,-0.9106232682,-0.6257634692,0.7401644346,0.4053272873,0.8390195772,
    0.9242524096,0.07647613266,0.6066256811,-0.9750989929,-0.542299559,-0.415802277,-0.7304819962,0.8577948156,
    -0.548311967,-0.7626472379,0.03079647928,-0.6640829889,-0.1740147258,-0.7804430448,-0.6096987708,-0.6732403169,
    0.5676542638,0.8110471154,0.3832947817,0.9245493968,0.3043148782,-0.8259286236,0.4148098596,0.8567161266,
    0.3773180787,-0.6656597827,-0.2875793547,0.9193708367,-0.9577909544,-0.6917998565,-0.1753855374,0.8521606816,
    -0.5261292347,-0.269547243,0.5105846203,0.05585611296,0.5017280509,-0.2381686273,-0.270947803,0.872258117,
    -0.3250143309,-0.7799057743,0.85037073,0.6598504336,0.09616365406,0.7118504878,0.312427597,-0.2401093292,
    -0.7207336136,-0.5875763221,0.6419895885,-0.6785471501,-0.2168887573,0.7244372011,-0.7096451073,0.1323631078,
    -0.02980516237,-0.3930171956,0.05541651717,-0.2690503865,0.67369766,0.3035458599,-0.7528270071,0.1675647412,
    0.7695304522,0.594473625,-0.603925654,-0.6506083235,-0.3430042733,0.6981725247,-0.5752009504,0.810555537,
    -0.3475211012,-0.4719692027,0.3583001386,-0.2043157126,0.278165994,-0.7857120652,-0.5495413974,0.1365639107,
    -0.6440875597,-0.7946340692,-0.8885529948,0.1529888965,-0.6164050508,-0.00669719484,-0.5336586252,0.7976208196,
    0.9245608561,0.2745365933,0.6091141441,-0.2703029469,0.6832968393,0.9658208605,0.7402471173,-0.6926784018,
    -0.5724548511,0.7381471625,0.9188871375,-0.5904376907,0.7707441366,0.7511360995,0.226674665,-0.604222686,
    0.4132844051,-0.5162925989,0.8273785755,0.7953543429,-0.9572290247,-0.9679131102,-0.8881505545,-0.4371373164,
    0.04659011161,-0.9082481361,-0.3483642108,0.4147529736,-0.7116334161,0.18886992,-0.3725217914,-0.1780209141,
    -0.9698322841,-0.7788918132,-0.4755398075,-0.7474340931,0.1699017182,0.02145707593,0.06555605789,-0.8044887315,
    0.03447489231,0.6859872284,-0.10920836,0.7325250401,-0.9777375055,-0.9111505856,0.7601631212,-0.845289248
};

static const FNfloat CELLULAR_OFFSETS_3D_Z[] = {
    0.1735581948,-0.2739291014,-0.3023380997,-0.0790000257,-0.3370999617,0.7816254623,-0.8959289049,0.7607405838,
    -0.4674352136,0.94291689,-0.2679381538,0.4115677815,-0.5878203852,0.6606022552,-0.7202371714,0.475797649,
    -0.8361987284,-0.1212153153,-0.09397607749,0.3742469607,-0.02107817834,-0.7495104896,0.9140719259,-0.6852128902,
    0.3509933228,0.3412049336,0.3006480328,-0.1811724921,0.2145069156,-0.6507845481,-0.6164974636,0.6439087246,
    0.3618239211,0.7617583057,-0.6821542638,0.7271630328,-0.3418836993,0.9039195862,-0.4959995258,0.6057105799,
    0.8972364289,-0.1885487608,0.01024666929,-0.7023778346,0.644859585,0.5104855816,0.5608371729,0.7357899537,
    -0.6367380315,0.641469197,0.3368515753,-0.6018234363,-0.372840892,0.1157505864,-0.6349883666,0.6414307729,
    0.6968408593,0.120178631,0.6671608058,-0.4960053754,0.5345495242,-0.1874115585,0.4781800233,0.7258696564,
    -0.8227581726,-0.8620330371,0.4695550591,-0.6427953014,-0.5391679918,0.7527940996,0.7219805347,0.2273952515,
    -0.3210532909,0.7764570201,-0.6205791146,-0.4224826141,0.777335046,0.1158329937,-0.1639543925,-0.5834887393,
    -0.5635892736,-0.06694497494,-0.007368825344,0.2694238123,0.6155736747,0.3339292269,0.5627293636,0.7199039967,
    -0.8138926898,-0.5641463116,-0.8604958341,0.667882296,0.357841725,-0.3540489801,-0.7424963803,-0.9250488391,
    -0.5097070502,-0.7891830656,-0.3699414651,0.6260573129,-0.0873632843,0.4768419182,-0.1235710781,0.5474656618,
    -0.4502648413,0.6798550586,-0.2992580792,0.9486454084,-0.8032795685,0.9149531629,-0.3058922593,0.8224791405,
    -0.7351186982,-0.6029098428,0.4496111079,-0.4122962022,0.01975775581,-0.4249554471,-0.3167501783,-0.5339674439,
    -0.1520850155,0.9770547224,-0.1280964233,-0.2026569073,0.1458203587,0.840000362,-0.6366310879,0.3567584454,
    -0.5913303597,-0.6354935001,0.7863922953,-0.7369967419,-0.7417077429,-0.06547655076,0.4404473475,-0.6887635427,
    0.7277093879,-0.1051963504,0.131900567,0.3654593525,0.8743716458,0.5214617324,-0.7008834116,-0.4761596756,
    -0.9254661404,0.2919133642,-0.5652430251,-0.3813575004,-0.09426605581,-0.6792188003,0.1155036542,0.02903615945,
    0.3534179531,0.6467878011,-0.8439637196,0.7583384168,-0.7254255765,-0.7361091029,-0.3357181763,0.3086152025,
    0.6514394162,-0.5546100667,0.4825351604,-0.5947077538,-0.4807031248,-0.1844907016,0.637304036,-0.6670798253,
    -0.6883545647,-0.4069869034,0.7559286424,-0.3714146849,-0.8020356851,-0.3738578718,0.6946792478,0.7785527795,
    -0.715024719,0.3919597455,-0.9875892167,0.6520101478,-0.68688995,-0.6880466294,0.4487723203,-0.2875261255,
    -0.4677277752,0.7014236729,0.3301814672,0.02433313207,0.3883935666,0.3682257648,0.3342849376,-0.1210916791,
    -0.167514036,-0.5218416899,0.929669703,0.5222271202,0.8599346446,-0.3019204161,-0.6128307303,-0.04503418428,
    0.4543037336,0.5975094525,0.28495784,0.9638738118,-0.3007234646,0.6887448187,0.6377908264,-0.3160000073,
    -0.03522751494,-0.4974176556,0.5804613989,0.5301601931,0.4095943388,-0.2512108074,-0.6634877936,0.1059128479,
    -0.5050958653,0.159995615,-0.05186755998,0.6429111375,-0.2101820095,0.3368851762,0.5753666838,-0.6561857263,
    -0.9105991643,0.6286591339,0.5604221175,-0.3244980058,-0.1084387619,-0.09594243324,0.2913006227,0.5188422971,
    -0.02833944577,0.1900757285,0.1925298489,-0.8684886582,0.4798207128,-0.4482350644,0.3814499973,-0.4026575304,
    -0.1094789531,-0.6085091231,-0.8403819825,-0.3373418503,-0.6756111411,-0.7289967412,-0.9774476623,0.5827524187,
    0.6159443543,0.6991389226,-0.9903080513,-0.396610771,-0.1004076743,0.4047110257,-0.6344734459,0.2904925424,
};

// ====================
// Noise functions
// ====================

// White Noise

static inline int _fnFloatCast2Int(FNfloat f) {
    typedef union {
        uint64_t i;
        FNfloat f;
    } u;
    u t;
    t.f = f;
    return (int)(t.i ^ (t.i >> 32));
}

static FNfloat _fnSingleWhiteNoise2D(int seed, FNfloat x, FNfloat y) {
    int xi = _fnFloatCast2Int(x) * PRIME_X;
    int yi = _fnFloatCast2Int(y) * PRIME_Y;
    return _fnValCoord2D(seed, xi, yi);
}

static FNfloat _fnSingleWhiteNoise3D(int seed, FNfloat x, FNfloat y, FNfloat z) {
    int xi = _fnFloatCast2Int(x) * PRIME_X;
    int yi = _fnFloatCast2Int(y) * PRIME_Y;
    int zi = _fnFloatCast2Int(z) * PRIME_Z;
    return _fnValCoord3D(seed, xi, yi, zi);
}

// Value noise

static FNfloat _fnSingleValue2D(int seed, FNfloat x, FNfloat y) {
    int x0 = _fnFastFloor(x);
    int y0 = _fnFastFloor(y);
    
    FNfloat xs = _fnInterpQuintic((FNfloat)(x - x0));
    FNfloat ys = _fnInterpQuintic((FNfloat)(y - y0));

    x0 *= PRIME_X;
    y0 *= PRIME_Y;
    int x1 = x0 + PRIME_X;
    int y1 = y0 + PRIME_Y;

    FNfloat xf0 = _fnLerp(_fnValCoord2D(seed, x0, y0), _fnValCoord2D(seed, x1, y0), xs);
    FNfloat xf1 = _fnLerp(_fnValCoord2D(seed, x0, y1), _fnValCoord2D(seed, x1, y1), xs);

    return _fnLerp(xf0, xf1, ys);
}

static FNfloat _fnSingleValue3D(int seed, FNfloat x, FNfloat y, FNfloat z) {
    int x0 = _fnFastFloor(x);
    int y0 = _fnFastFloor(y);
    int z0 = _fnFastFloor(z);

    FNfloat xs = _fnInterpQuintic((FNfloat)(x - x0));
    FNfloat ys = _fnInterpQuintic((FNfloat)(y - y0));
    FNfloat zs = _fnInterpQuintic((FNfloat)(z - z0));

    x0 *= PRIME_X;
    y0 *= PRIME_Y;
    z0 *= PRIME_Z;
    int x1 = x0 + PRIME_X;
    int y1 = y0 + PRIME_Y;
    int z1 = z0 + PRIME_Z;

    FNfloat xf00 = _fnLerp(_fnValCoord3D(seed, x0, y0, z0), _fnValCoord3D(seed, x1, y0, z0), xs);
    FNfloat xf10 = _fnLerp(_fnValCoord3D(seed, x0, y1, z0), _fnValCoord3D(seed, x1, y1, z0), xs);
    FNfloat xf01 = _fnLerp(_fnValCoord3D(seed, x0, y0, z1), _fnValCoord3D(seed, x1, y0, z1), xs);
    FNfloat xf11 = _fnLerp(_fnValCoord3D(seed, x0, y1, z1), _fnValCoord3D(seed, x1, y1, z1), xs);
        
    FNfloat yf0 = _fnLerp(xf00, xf10, ys);
    FNfloat yf1 = _fnLerp(xf01, xf11, ys);

    return _fnLerp(yf0, yf1, zs);
}

// Value Cubic

static const FNfloat CUBIC_2D_BOUNDING = 1 / (FNfloat)(1.5 * 1.5);

static FNfloat _fnSingleValueCubic2D(int seed, FNfloat x, FNfloat y) {
    int x1 = _fnFastFloor(x);
    int y1 = _fnFastFloor(y);

    FNfloat xs = x - (FNfloat) x1;
    FNfloat ys = y - (FNfloat) y1;

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
               ys) * CUBIC_2D_BOUNDING;
}

static const FNfloat CUBIC_3D_BOUNDING = 1 / (FNfloat) (1.5 * 1.5 * 1.5);

static FNfloat _fnSingleValueCubic3D(int seed, FNfloat x, FNfloat y, FNfloat z) {
    int x1 = _fnFastFloor(x);
    int y1 = _fnFastFloor(y);
    int z1 = _fnFastFloor(z);

    FNfloat xs = x - (FNfloat)x1;
    FNfloat ys = y - (FNfloat)y1;
    FNfloat zs = z - (FNfloat)z1;

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
            zs) * CUBIC_3D_BOUNDING;
}

// Perlin Noise

static FNfloat _fnSinglePerlin2D(int seed, FNfloat x, FNfloat y) {
    int x0 = _fnFastFloor(x);
    int y0 = _fnFastFloor(y);

    FNfloat xd0 = (FNfloat) (x - x0);
    FNfloat yd0 = (FNfloat) (y - y0);
    FNfloat xd1 = xd0 - 1;
    FNfloat yd1 = yd0 - 1;

    FNfloat xs = _fnInterpQuintic(xd0);
    FNfloat ys = _fnInterpQuintic(yd0);

    x0 *= PRIME_X;
    y0 *= PRIME_Y;
    int x1 = x0 + PRIME_X;
    int y1 = y0 + PRIME_Y;

    FNfloat xf0 = _fnLerp(_fnGradCoord2D(seed, x0, y0, xd0, yd0), _fnGradCoord2D(seed, x1, y0, xd1, yd0), xs);
    FNfloat xf1 = _fnLerp(_fnGradCoord2D(seed, x0, y1, xd0, yd1), _fnGradCoord2D(seed, x1, y1, xd1, yd1), xs);

    return _fnLerp(xf0, xf1, ys) * (FNfloat) 0.579106986522674560546875;
}

static FNfloat _fnSinglePerlin3D(int seed, FNfloat x, FNfloat y, FNfloat z) {
    int x0 = _fnFastFloor(x);
    int y0 = _fnFastFloor(y);
    int z0 = _fnFastFloor(z);

    FNfloat xd0 = (FNfloat) (x - x0);
    FNfloat yd0 = (FNfloat) (y - y0);
    FNfloat zd0 = (FNfloat) (z - z0);
    FNfloat xd1 = xd0 - 1;
    FNfloat yd1 = yd0 - 1;
    FNfloat zd1 = zd0 - 1;

    FNfloat xs = _fnInterpQuintic(xd0);
    FNfloat ys = _fnInterpQuintic(yd0);
    FNfloat zs = _fnInterpQuintic(zd0);

    x0 *= PRIME_X;
    y0 *= PRIME_Y;
    z0 *= PRIME_Z;
    int x1 = x0 + PRIME_X;
    int y1 = y0 + PRIME_Y;
    int z1 = z0 + PRIME_Z;

    FNfloat xf00 = _fnLerp(_fnGradCoord3D(seed, x0, y0, z0, xd0, yd0, zd0), _fnGradCoord3D(seed, x1, y0, z0, xd1, yd0, zd0), xs);
    FNfloat xf10 = _fnLerp(_fnGradCoord3D(seed, x0, y1, z0, xd0, yd1, zd0), _fnGradCoord3D(seed, x1, y1, z0, xd1, yd1, zd0), xs);
    FNfloat xf01 = _fnLerp(_fnGradCoord3D(seed, x0, y0, z1, xd0, yd0, zd1), _fnGradCoord3D(seed, x1, y0, z1, xd1, yd0, zd1), xs);
    FNfloat xf11 = _fnLerp(_fnGradCoord3D(seed, x0, y1, z1, xd0, yd1, zd1), _fnGradCoord3D(seed, x1, y1, z1, xd1, yd1, zd1), xs);
         
    FNfloat yf0 = _fnLerp(xf00, xf10, ys);
    FNfloat yf1 = _fnLerp(xf01, xf11, ys);

    return _fnLerp(yf0, yf1, zs) * (FNfloat) 0.964921414852142333984375;
}

// Simplex Noise

static FNfloat _fnSingleSimplex2D(int seed, FNfloat x, FNfloat y) {
    const FNfloat SQRT3 = (FNfloat)1.7320508075688772935274463415059;
    const FNfloat F2 = (FNfloat)0.5 * (SQRT3 - 1);
    const FNfloat G2 = (3 - SQRT3) / (FNfloat)6.0;

    FNfloat t = (x + y) * F2;
    int i = _fnFastFloor(x + t);
    int j = _fnFastFloor(y + t);

    t = (i + j) * G2;
    FNfloat x0 = (FNfloat)(x - (i - t));
    FNfloat y0 = (FNfloat)(y - (j - t));

    int i1, j1;
    if (x0 > y0)
    {
        i1 = -1; j1 = 0;
    }
    else
    {
        i1 = 0; j1 = -1;
    }

    FNfloat x1 = x0 + i1 + (FNfloat)G2;
    FNfloat y1 = y0 + j1 + (FNfloat)G2;
    FNfloat x2 = x0 - 1 + 2 * (FNfloat)G2;
    FNfloat y2 = y0 - 1 + 2 * (FNfloat)G2;

    i *= PRIME_X;
    j *= PRIME_Y;

    i1 &= PRIME_X;
    j1 &= PRIME_Y;

    FNfloat n0, n1, n2;

    FNfloat a = 0.5f - x0 * x0 - y0 * y0;
    if (a < 0) n0 = 0;
    else
    {
        a *= a;
        n0 = a * a * _fnGradCoord2D(seed, i, j, x0, y0);
    }

    FNfloat b = 0.5f - x1 * x1 - y1 * y1;
    if (b < 0) n1 = 0;
    else
    {
        b *= b;
        n1 = b * b * _fnGradCoord2D(seed, i + i1, j + j1, x1, y1);
    }

    FNfloat c = 0.5f - x2 * x2 - y2 * y2;
    if (c < 0) n2 = 0;
    else
    {
        c *= c;
        n2 = c * c * _fnGradCoord2D(seed, i + PRIME_X, j + PRIME_Y, x2, y2);
    }
    return (n0 + n1 + n2) * 38.283687591552734375f;
}

static FNfloat _fnSingleSimplex3D(int seed, FNfloat x, FNfloat y, FNfloat z) {
    const FNfloat F3 = (FNfloat)(1.0 / 3.0);
    const FNfloat G3 = (FNfloat)(1.0 / 6.0);

    FNfloat t = (x + y + z) * F3;
    int i = _fnFastFloor(x + t);
    int j = _fnFastFloor(y + t);
    int k = _fnFastFloor(z + t);

    t = (i + j + k) * G3;
    FNfloat x0 = (FNfloat)(x - (i - t));
    FNfloat y0 = (FNfloat)(y - (j - t));
    FNfloat z0 = (FNfloat)(z - (k - t));

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

    FNfloat x1 = x0 + i1 + (FNfloat)G3;
    FNfloat y1 = y0 + j1 + (FNfloat)G3;
    FNfloat z1 = z0 + k1 + (FNfloat)G3;
    FNfloat x2 = x0 + i2 + (FNfloat)F3;
    FNfloat y2 = y0 + j2 + (FNfloat)F3;
    FNfloat z2 = z0 + k2 + (FNfloat)F3;
    FNfloat x3 = x0 - (FNfloat) 0.5;
    FNfloat y3 = y0 - (FNfloat) 0.5;
    FNfloat z3 = z0 - (FNfloat) 0.5;

    i  *= PRIME_X;
    j  *= PRIME_Y;
    k  *= PRIME_Z;

    i1 &= PRIME_X;
    j1 &= PRIME_Y;
    k1 &= PRIME_Z;

    i2 &= PRIME_X;
    j2 &= PRIME_Y;
    k2 &= PRIME_Z;

    FNfloat n0, n1, n2, n3;

    FNfloat a = (FNfloat) 0.6 - x0 * x0 - y0 * y0 - z0 * z0;
    if (a < 0) n0 = 0;
    else {
        a *= a;
        n0 = a * a * _fnGradCoord3D(seed, i, j, k, x0, y0, z0);
    }

    FNfloat b = (FNfloat) 0.6 - x1 * x1 - y1 * y1 - z1 * z1;
    if (b < 0) n1 = 0;
    else {
        b *= b;
        n1 = b * b * _fnGradCoord3D(seed, i + i1, j + j1, k + k1, x1, y1, z1);
    }

    FNfloat c = (FNfloat) 0.6 - x2 * x2 - y2 * y2 - z2 * z2;
    if (c < 0) n2 = 0;
    else {
        c *= c;
        n2 = c * c * _fnGradCoord3D(seed, i + i2, j + j2, k + k2, x2, y2, z2);
    }

    FNfloat d = (FNfloat) 0.6 - x3 * x3 - y3 * y3 - z3 * z3;
    if (d < 0) n3 = 0;
    else {
        d *= d;
        n3 = d * d * _fnGradCoord3D(seed, i + PRIME_X, j + PRIME_Y, k + PRIME_Z, x3, y3, z3);
    }

    return (n0 + n1 + n2 + n3) * 32.69588470458984375f;
}

// Cellular Noise

static FNfloat _fnSingleCellular2D(fn_state *state, int seed, FNfloat x, FNfloat y) {
    int xr = _fnFastRound(x);
    int yr = _fnFastRound(y);

    FNfloat distance0 = FN_FLOAT_MAX;
    FNfloat distance1 = FN_FLOAT_MAX;
    int closestHash = 0;

    FNfloat cellularJitter = 0.5f * state->cellular_jitter_mod;

    switch (state->cellular_distance_func) {
        case FN_CELLULAR_DIST_EUCLIDEAN:
            for (int xi = xr - 1; xi <= xr + 1; xi++) {
                for (int yi = yr - 1; yi <= yr + 1; yi++) {
                    int hash = _fnHash2D(seed, xi * PRIME_X, yi * PRIME_Y);
                    int idx = hash & 255;
                    FNfloat vx = CELLULAR_OFFSETS_2D_X[idx];
                    FNfloat vy = CELLULAR_OFFSETS_2D_Y[idx];
                    FNfloat vecX = (FNfloat)(xi - x) + vx * cellularJitter;
                    FNfloat vecY = (FNfloat)(yi - y) + vy * cellularJitter;

                    FNfloat newDistance = vecX * vecX + vecY * vecY;

                    distance1 = _fnFastMax(_fnFastMin(distance1, newDistance), distance0);
                    if (newDistance < distance0) {
                        distance0 = newDistance;
                        closestHash = hash;
                    }
                }
            }
            break;
        case FN_CELLULAR_DIST_MANHATTAN:
            for (int xi = xr - 1; xi <= xr + 1; xi++) {
                for (int yi = yr - 1; yi <= yr + 1; yi++) {
                    int hash = _fnHash2D(seed, xi * PRIME_X, yi * PRIME_Y);
                    int idx = hash & 255;
                    FNfloat vx = CELLULAR_OFFSETS_2D_X[idx];
                    FNfloat vy = CELLULAR_OFFSETS_2D_Y[idx];

                    FNfloat vecX = (FNfloat)(xi - x) + vx * cellularJitter;
                    FNfloat vecY = (FNfloat)(yi - y) + vy * cellularJitter;

                    FNfloat newDistance = FN_ABS(vecX) + FN_ABS(vecY);

                    distance1 = _fnFastMax(_fnFastMin(distance1, newDistance), distance0);
                    if (newDistance < distance0) {
                        distance0 = newDistance;
                        closestHash = hash;
                    }
                }
            }
            break;
        case FN_CELLULAR_DIST_NATURAL:
            for (int xi = xr - 1; xi <= xr + 1; xi++) {
                for (int yi = yr - 1; yi <= yr + 1; yi++) {
                    int hash = _fnHash2D(seed, xi * PRIME_X, yi * PRIME_Y);
                    int idx = hash & 255;
                    FNfloat vx = CELLULAR_OFFSETS_2D_X[idx];
                    FNfloat vy = CELLULAR_OFFSETS_2D_Y[idx];

                    FNfloat vecX = (FNfloat)(xi - x) + vx * cellularJitter;
                    FNfloat vecY = (FNfloat)(yi - y) + vy * cellularJitter;

                    FNfloat newDistance = (FN_ABS(vecX) + FN_ABS(vecY)) + (vecX * vecX + vecY * vecY);

                    distance1 = _fnFastMax(_fnFastMin(distance1, newDistance), distance0);
                    if (newDistance < distance0) {
                        distance0 = newDistance;
                        closestHash = hash;
                    }
                }
            }
            break;
        default:
            break;
    }

    switch (state->cellular_return_type)
    {
        case FN_CELLULAR_RET_CELLVALUE:
            return closestHash / (FNfloat) 2147483648.0;
        case FN_CELLULAR_RET_DISTANCE:
            return distance0;
        case FN_CELLULAR_RET_DISTANCE2:
            return distance1;
        case FN_CELLULAR_RET_DISTANCE2ADD:
            return distance1 + distance0;
        case FN_CELLULAR_RET_DISTANCE2SUB:
            return distance1 - distance0;
        case FN_CELLULAR_RET_DISTANCE2MUL:
            return distance1 * distance0;
        case FN_CELLULAR_RET_DISTANCE2DIV:
            return distance0 / distance1;
        default:
            return 0;
    }
}

// ====================
// 2D Gen functions
// ====================

// DEV NOTE (if noiselookup stays): When porting to HLSL, have an overload for an array of states, when cellular lookup is required, we look for an adjacent state
static FNfloat _fnGenNoiseSingle2D(fn_state *state, int seed, FNfloat x, FNfloat y) {
    switch (state->noise_type) {
        case FN_NOISE_VALUE:
            return _fnSingleValue2D(seed, x, y);
        case FN_NOISE_VALUE_CUBIC:
            return _fnSingleValueCubic2D(seed, x, y);
        case FN_NOISE_PERLIN:
            return _fnSinglePerlin2D(seed, x, y);
        case FN_NOISE_SIMPLEX:
            return _fnSingleSimplex2D(seed, x, y);
        // case FN_NOISE_OPENSIMPLEX2F:
        //     return 0; // TODO
        case FN_NOISE_CELLULAR:
            return _fnSingleCellular2D(state, seed, x, y);
        case FN_NOISE_WHITENOISE:
            return _fnSingleWhiteNoise2D(seed, x, y);
        default:
            return 0;
    }
}

static FNfloat _fnGenFractalFBM2D(fn_state *state, FNfloat x, FNfloat y) {
    int seed = state->seed;
    FNfloat sum = _fnGenNoiseSingle2D(state, seed, x, y);
    FNfloat amp = (FNfloat) 1.0;

    for (int i = 1; i < state->octaves; i++) {
        x *= state->lacunarity;
        y *= state->lacunarity;

        amp *= state->gain;
        sum += _fnGenNoiseSingle2D(state, ++seed, x, y) * amp;
    }

    return sum * _fnCalculateFractalBounding(state);
}

static FNfloat _fnGenFractalBillow2D(fn_state *state, FNfloat x, FNfloat y) {
    int seed = state->seed;
    FNfloat sum = FN_ABS(_fnGenNoiseSingle2D(state, seed, x, y)) * 2 - 1;
    FNfloat amp = (FNfloat) 1.0;

    for (int i = 1; i < state->octaves; i++) {
        x *= state->lacunarity;
        y *= state->lacunarity;

        amp *= state->gain;
        sum += (FN_ABS(_fnGenNoiseSingle2D(state, ++seed, x, y)) * 2 - 1) * amp;
    }

    return sum * _fnCalculateFractalBounding(state);
}

static FNfloat _fnGenFractalRidged2D(fn_state *state, FNfloat x, FNfloat y) {
    int seed = state->seed;
    FNfloat sum = 1 - FN_ABS(_fnGenNoiseSingle2D(state, seed, x, y));
    FNfloat amp = (FNfloat) 1.0;

    for (int i = 1; i < state->octaves; i++) {
        x *= state->lacunarity;
        y *= state->lacunarity;

        amp *= state->gain;
        sum -= (1 - FN_ABS(_fnGenNoiseSingle2D(state, ++seed, x, y))) * amp;
    }

    return sum;
}

// ====================
// 3D Gen functions
// ====================

static FNfloat _fnGenNoiseSingle3D(fn_state *state, int seed, FNfloat x, FNfloat y, FNfloat z) {
    switch (state->noise_type) {
        case FN_NOISE_VALUE:
            return _fnSingleValue3D(seed, x, y, z);
        case FN_NOISE_VALUE_CUBIC:
            return _fnSingleValueCubic3D(seed, x, y, z);
        case FN_NOISE_PERLIN:
            return _fnSinglePerlin3D(seed, x, y, z);
        case FN_NOISE_SIMPLEX:
            return _fnSingleSimplex3D(seed, x, y, z);
        // case FN_NOISE_OPENSIMPLEX2F:
        //     return 0; // TODO
        // case FN_NOISE_CELLULAR:
        //     return _fnSingleCellular3D(state, seed, x, y, z);
        // case FN_NOISE_WHITENOISE:
        //     return _fnSingleWhiteNoise3D(seed, x, y, z);
        default:
            return 0;
    }
}

static FNfloat _fnGenFractalFBM3D(fn_state *state, FNfloat x, FNfloat y, FNfloat z) {
    int seed = state->seed;
    FNfloat sum = _fnGenNoiseSingle3D(state, seed, x, y, z);
    FNfloat amp = (FNfloat) 1.0;

    for (int i = 1; i < state->octaves; i++) {
        x *= state->lacunarity;
        y *= state->lacunarity;
        z *= state->lacunarity;

        amp *= state->gain;
        sum += _fnGenNoiseSingle3D(state, ++seed, x, y, z) * amp;
    }

    return sum * _fnCalculateFractalBounding(state);
}

static FNfloat _fnGenFractalBillow3D(fn_state *state, FNfloat x, FNfloat y, FNfloat z) {
    int seed = state->seed;
    FNfloat sum = FN_ABS(_fnGenNoiseSingle3D(state, seed, x, y, z)) * 2 - 1;
    FNfloat amp = (FNfloat) 1.0;

    for (int i = 1; i < state->octaves; i++) {
        x *= state->lacunarity;
        y *= state->lacunarity;
        z *= state->lacunarity;

        amp *= state->gain;
        sum += (FN_ABS(_fnGenNoiseSingle3D(state, ++seed, x, y, z)) * 2 - 1) * amp;
    }

    return sum * _fnCalculateFractalBounding(state);
}

static FNfloat _fnGenFractalRidged3D(fn_state *state, FNfloat x, FNfloat y, FNfloat z) {
    int seed = state->seed;
    FNfloat sum = 1 - FN_ABS(_fnGenNoiseSingle3D(state, seed, x, y, z));
    FNfloat amp = (FNfloat) 1.0;

    for (int i = 1; i < state->octaves; i++) {
        x *= state->lacunarity;
        y *= state->lacunarity;
        z *= state->lacunarity;

        amp *= state->gain;
        sum -= (1 - FN_ABS(_fnGenNoiseSingle3D(state, ++seed, x, y, z))) * amp;
    }

    return sum;
}

// Domain Warp Gradient

static void _fnSingleDomainWarpGradient2D(int seed, FNfloat perturbAmp, FNfloat frequency, FNfloat *x, FNfloat *y) {
    FNfloat xf = *x * frequency;
    FNfloat yf = *y * frequency;

    int x0 = _fnFastFloor(xf);
    int y0 = _fnFastFloor(yf);

    FNfloat xs = _fnInterpHermite((FNfloat)(xf - x0));
    FNfloat ys = _fnInterpHermite((FNfloat)(yf - y0));

    x0 *= PRIME_X;
    y0 *= PRIME_Y;
    int x1 = x0 + PRIME_X;
    int y1 = y0 + PRIME_Y;

    int hash0 = _fnHash2D(seed, x0, y0) & 255;
    int hash1 = _fnHash2D(seed, x1, y0) & 255;

    FNfloat vec0x = CELLULAR_OFFSETS_2D_X[hash0];
    FNfloat vec0y = CELLULAR_OFFSETS_2D_Y[hash0];
    FNfloat vec1x = CELLULAR_OFFSETS_2D_X[hash1];
    FNfloat vec1y = CELLULAR_OFFSETS_2D_Y[hash1];

    FNfloat lx0x = _fnLerp(vec0x, vec1x, xs);
    FNfloat ly0x = _fnLerp(vec0y, vec1y, xs);

    hash0 = _fnHash2D(seed, x0, y1) & 255;
    hash1 = _fnHash2D(seed, x1, y1) & 255;

    vec0x = CELLULAR_OFFSETS_2D_X[hash0];
    vec0y = CELLULAR_OFFSETS_2D_Y[hash0];
    vec1x = CELLULAR_OFFSETS_2D_X[hash1];
    vec1y = CELLULAR_OFFSETS_2D_Y[hash1];

    FNfloat lx1x = _fnLerp(vec0x, vec1x, xs);
    FNfloat ly1x = _fnLerp(vec0y, vec1y, xs);

    *x += _fnLerp(lx0x, lx1x, ys) * perturbAmp;
    *y += _fnLerp(ly0x, ly1x, ys) * perturbAmp;
}

static void _fnSingleDomainWarpGradient3D(int seed, FNfloat perturbAmp, FNfloat frequency, FNfloat *x, FNfloat *y, FNfloat *z) {
    FNfloat xf = *x * frequency;
    FNfloat yf = *y * frequency;
    FNfloat zf = *z * frequency;

    int x0 = _fnFastFloor(xf);
    int y0 = _fnFastFloor(yf);
    int z0 = _fnFastFloor(zf);

    FNfloat xs = _fnInterpHermite((FNfloat)(xf - x0));
    FNfloat ys = _fnInterpHermite((FNfloat)(yf - y0));
    FNfloat zs = _fnInterpHermite((FNfloat)(zf - z0));

    x0 *= PRIME_X;
    y0 *= PRIME_Y;
    z0 *= PRIME_Z;
    int x1 = x0 + PRIME_X;
    int y1 = y0 + PRIME_Y;
    int z1 = z0 + PRIME_Z;

    int hash0 = _fnHash3D(seed, x0, y0, z0) & 255;
    int hash1 = _fnHash3D(seed, x1, y0, z0) & 255;

    FNfloat vec0x = CELLULAR_OFFSETS_3D_X[hash0];
    FNfloat vec0y = CELLULAR_OFFSETS_3D_Y[hash0];
    FNfloat vec0z = CELLULAR_OFFSETS_3D_Z[hash0];
    FNfloat vec1x = CELLULAR_OFFSETS_3D_X[hash1];
    FNfloat vec1y = CELLULAR_OFFSETS_3D_Y[hash1];
    FNfloat vec1z = CELLULAR_OFFSETS_3D_Z[hash1];

    FNfloat lx0x = _fnLerp(vec0x, vec1x, xs);
    FNfloat ly0x = _fnLerp(vec0y, vec1y, xs);
    FNfloat lz0x = _fnLerp(vec0z, vec1z, xs);

    hash0 = _fnHash3D(seed, x0, y1, z0) & 255;
    hash1 = _fnHash3D(seed, x1, y1, z0) & 255;

    vec0x = CELLULAR_OFFSETS_3D_X[hash0];
    vec0y = CELLULAR_OFFSETS_3D_Y[hash0];
    vec0z = CELLULAR_OFFSETS_3D_Z[hash0];
    vec1x = CELLULAR_OFFSETS_3D_X[hash1];
    vec1y = CELLULAR_OFFSETS_3D_Y[hash1];
    vec1z = CELLULAR_OFFSETS_3D_Z[hash1];

    FNfloat lx1x = _fnLerp(vec0x, vec1x, xs);
    FNfloat ly1x = _fnLerp(vec0y, vec1y, xs);
    FNfloat lz1x = _fnLerp(vec0z, vec1z, xs);
        
    FNfloat lx0y = _fnLerp(lx0x, lx1x, ys);
    FNfloat ly0y = _fnLerp(ly0x, ly1x, ys);
    FNfloat lz0y = _fnLerp(lz0x, lz1x, ys);

    hash0 = _fnHash3D(seed, x0, y0, z1) & 255;
    hash1 = _fnHash3D(seed, x1, y0, z1) & 255;

    vec0x = CELLULAR_OFFSETS_3D_X[hash0];
    vec0y = CELLULAR_OFFSETS_3D_Y[hash0];
    vec0z = CELLULAR_OFFSETS_3D_Z[hash0];
    vec1x = CELLULAR_OFFSETS_3D_X[hash1];
    vec1y = CELLULAR_OFFSETS_3D_Y[hash1];
    vec1z = CELLULAR_OFFSETS_3D_Z[hash1];

    lx0x = _fnLerp(vec0x, vec1x, xs);
    ly0x = _fnLerp(vec0y, vec1y, xs);
    lz0x = _fnLerp(vec0z, vec1z, xs);

    hash0 = _fnHash3D(seed, x0, y1, z1) & 255;
    hash1 = _fnHash3D(seed, x1, y1, z1) & 255;

    vec0x = CELLULAR_OFFSETS_3D_X[hash0];
    vec0y = CELLULAR_OFFSETS_3D_Y[hash0];
    vec0z = CELLULAR_OFFSETS_3D_Z[hash0];
    vec1x = CELLULAR_OFFSETS_3D_X[hash1];
    vec1y = CELLULAR_OFFSETS_3D_Y[hash1];
    vec1z = CELLULAR_OFFSETS_3D_Z[hash1];

    lx1x = _fnLerp(vec0x, vec1x, xs);
    ly1x = _fnLerp(vec0y, vec1y, xs);
    lz1x = _fnLerp(vec0z, vec1z, xs);

    *x += _fnLerp(lx0y, _fnLerp(lx0x, lx1x, ys), zs) * perturbAmp;
    *y += _fnLerp(ly0y, _fnLerp(ly0x, ly1x, ys), zs) * perturbAmp;
    *z += _fnLerp(lz0y, _fnLerp(lz0x, lz1x, ys), zs) * perturbAmp;
}

// Domain warp get single

static void _fnDoSingleDomainWarp2D(fn_state *state, int seed, FNfloat amp, FNfloat freq, FNfloat *x, FNfloat *y) {
    switch (state->domain_warp_type) {
        case FN_DOMAIN_WARP_GRADIENT:
            _fnSingleDomainWarpGradient2D(seed, amp, freq, x, y);
            break;
        case FN_DOMAIN_WARP_SIMPLEX:
            break;
    }
}

static void _fnDoSingleDomainWarp3D(fn_state *state, int seed, FNfloat amp, FNfloat freq, FNfloat *x, FNfloat *y, FNfloat *z) {
    switch (state->domain_warp_type) {
        case FN_DOMAIN_WARP_GRADIENT:
            _fnSingleDomainWarpGradient3D(seed, amp, freq, x, y, z);
            break;
        case FN_DOMAIN_WARP_SIMPLEX:
            break;
    }
}

// Domain Warp Fractal Progressive

static void _fnDomainWarpFractalProgressive2D(fn_state *state, FNfloat *x, FNfloat *y) {
    int seed = state->seed;
    float amp = state->domain_warp_amp * _fnCalculateFractalBounding(state);
    float freq = state->frequency;

    _fnDoSingleDomainWarp2D(state, seed, amp, freq, x, y);

    for (int i = 1; i < state->octaves; i++)
    {
        freq *= state->lacunarity;
        amp *= state->gain;
        _fnDoSingleDomainWarp2D(state, ++seed, amp, freq, x, y);
    }
}

static void _fnDomainWarpFractalProgressive3D(fn_state *state, FNfloat *x, FNfloat *y, FNfloat *z) {
    int seed = state->seed;
    float amp = state->domain_warp_amp * _fnCalculateFractalBounding(state);
    float freq = state->frequency;

    _fnDoSingleDomainWarp3D(state, seed, amp, freq, x, y, z);

    for (int i = 1; i < state->octaves; i++)
    {
        freq *= state->lacunarity;
        amp *= state->gain;
        _fnDoSingleDomainWarp3D(state, ++seed, amp, freq, x, y, z);
    }
}

// ====================
// Public API
// ====================

fn_state fnCreateState(int seed) {
    fn_state newState;
    newState.seed = seed;
    newState.frequency = (FNfloat) 0.01;
    newState.noise_type = FN_NOISE_SIMPLEX;
    newState.fractal_type = FN_FRACTAL_NONE;
    newState.octaves = 3;
    newState.lacunarity = (FNfloat) 2.0;
    newState.gain = (FNfloat) 0.5;
    newState.cellular_distance_func = FN_CELLULAR_DIST_EUCLIDEAN;
    newState.cellular_return_type = FN_CELLULAR_RET_CELLVALUE;
    newState.cellular_jitter_mod = (FNfloat) 1.0;
    newState.domain_warp_amp = 30.0;
    newState.domain_warp_type = FN_DOMAIN_WARP_GRADIENT;
    return newState;
}

void fnValidateState(fn_state *state) { _fnCalculateFractalBounding(state); }

FNfloat fnGetNoise2D(fn_state *state, FNfloat x, FNfloat y) {
    x *= state->frequency;
    y *= state->frequency;

    switch (state->fractal_type) {
        default:
            return _fnGenNoiseSingle2D(state, state->seed, x, y);
        case FN_FRACTAL_FBM:
            return _fnGenFractalFBM2D(state, x, y);
        case FN_FRACTAL_BILLOW:
            return _fnGenFractalBillow2D(state, x, y);
        case FN_FRACTAL_RIDGED:
            return _fnGenFractalRidged2D(state, x, y);
    }
}

FNfloat fnGetNoise3D(fn_state *state, FNfloat x, FNfloat y, FNfloat z) {
    x *= state->frequency;
    y *= state->frequency;
    z *= state->frequency;

    // Select a noise type
    switch (state->fractal_type) {
        default:
            return _fnGenNoiseSingle3D(state, state->seed, x, y, z);
        case FN_FRACTAL_FBM:
            return _fnGenFractalFBM3D(state, x, y, z);
        case FN_FRACTAL_BILLOW:
            return _fnGenFractalBillow3D(state, x, y, z);
        case FN_FRACTAL_RIDGED:
            return _fnGenFractalRidged3D(state, x, y, z);
    }
}

void fnDomainWarp2D(fn_state *state, FNfloat *x, FNfloat *y) {
    switch (state->fractal_type) {
        default:
            _fnDoSingleDomainWarp2D(state, state->seed, state->domain_warp_amp, state->frequency, x, y);
            break;
        case FN_FRACTAL_DOMAIN_WARP_PROGRESSIVE:
            _fnDomainWarpFractalProgressive2D(state, x, y);
            break;
        case FN_FRACTAL_DOMAIN_WARP_INDEPENDANT:
            break;
    }
}

void fnDomainWarp3D(fn_state *state, FNfloat *x, FNfloat *y, FNfloat *z) {
    switch (state->fractal_type) {
        default:
            _fnDoSingleDomainWarp3D(state, state->seed, state->domain_warp_amp, state->frequency, x, y, z);
            break;
        case FN_FRACTAL_DOMAIN_WARP_PROGRESSIVE:
            _fnDomainWarpFractalProgressive3D(state, x, y, z);
            break;
        case FN_FRACTAL_DOMAIN_WARP_INDEPENDANT:
            break;
    }
}

#endif

#if defined(__cplusplus)
}
#endif
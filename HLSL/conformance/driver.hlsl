// FastNoiseLite conformance kernel (HLSL). See conformance/README.md; driver.py runs it.
// One invocation per (point, instance): instance 0 is fnlCreateState(), instance c + 1 is config c.
// build.sh puts the port's folder (or a double-precision copy of it) on the include path.
#include "FastNoiseLite.hlsl"

struct Config
{
    int seed;
    float frequency;
    int noise_type, rotation_type_3d, fractal_type, octaves;
    float lacunarity, gain, weighted_strength, ping_pong_strength;
    int cellular_distance_func, cellular_return_type;
    float cellular_jitter_mod;
    int domain_warp_type;
    float domain_warp_amp;
};

[[vk::binding(0)]] StructuredBuffer<Config> Configs;
[[vk::binding(1)]] StructuredBuffer<FNLfloat> Points;  // per point: x y (2D), x y z (3D)
[[vk::binding(2)]] RWStructuredBuffer<FNLfloat> Out;   // per (instance, point): N2, N3, W2 x y, W3 x y z

[numthreads(1, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    fnl_state s = fnlCreateState();
    if (id.y > 0)
    {
        Config c = Configs[id.y - 1];
        s.seed = c.seed;
        s.frequency = c.frequency;
        s.noise_type = c.noise_type;
        s.rotation_type_3d = c.rotation_type_3d;
        s.fractal_type = c.fractal_type;
        s.octaves = c.octaves;
        s.lacunarity = c.lacunarity;
        s.gain = c.gain;
        s.weighted_strength = c.weighted_strength;
        s.ping_pong_strength = c.ping_pong_strength;
        s.cellular_distance_func = c.cellular_distance_func;
        s.cellular_return_type = c.cellular_return_type;
        s.cellular_jitter_mod = c.cellular_jitter_mod;
        s.domain_warp_type = c.domain_warp_type;
        s.domain_warp_amp = c.domain_warp_amp;
    }
    uint n, stride;
    Points.GetDimensions(n, stride);
    uint p = id.x * 5, o = (id.y * (n / 5) + id.x) * 7;
    FNLfloat x = Points[p], y = Points[p + 1];
    Out[o] = fnlGetNoise2D(s, x, y);
    fnlDomainWarp2D(s, x, y);
    Out[o + 2] = x;
    Out[o + 3] = y;
    FNLfloat X = Points[p + 2], Y = Points[p + 3], Z = Points[p + 4];
    Out[o + 1] = fnlGetNoise3D(s, X, Y, Z);
    fnlDomainWarp3D(s, X, Y, Z);
    Out[o + 4] = X;
    Out[o + 5] = Y;
    Out[o + 6] = Z;
}

// FastNoiseLite conformance kernel (GLSL). See conformance/README.md.
// build.sh appends this to FastNoiseLite.glsl and conformance/gpu.py runs the result; gpu.py describes the
// buffers. One invocation per (point, instance): instance 0 is the default state, instance c + 1 is config c.
// The port's FNL_* enum #defines have the C++ ordinals, so enum fields are copied unchanged.
layout(local_size_x = 1) in;

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

layout(std430, binding = 0) readonly buffer Configs { Config configs[]; };
layout(std430, binding = 1) readonly buffer Points { FNLfloat points[]; };   // per point: x y (2D), x y z (3D)
layout(std430, binding = 2) writeonly buffer Results { FNLfloat results[]; }; // per (instance, point): N2, N3, W2 x y, W3 x y z

void main()
{
    // The port has no zero-argument fnlCreateState(); 1337 is its documented default seed.
    fnl_state s = fnlCreateState(1337);
    int i = int(gl_GlobalInvocationID.x), c = int(gl_GlobalInvocationID.y);
    if (c > 0)
    {
        Config f = configs[c - 1];
        s.seed = f.seed;
        s.frequency = f.frequency;
        s.noise_type = f.noise_type;
        s.rotation_type_3d = f.rotation_type_3d;
        s.fractal_type = f.fractal_type;
        s.octaves = f.octaves;
        s.lacunarity = f.lacunarity;
        s.gain = f.gain;
        s.weighted_strength = f.weighted_strength;
        s.ping_pong_strength = f.ping_pong_strength;
        s.cellular_distance_func = f.cellular_distance_func;
        s.cellular_return_type = f.cellular_return_type;
        s.cellular_jitter_mod = f.cellular_jitter_mod;
        s.domain_warp_type = f.domain_warp_type;
        s.domain_warp_amp = f.domain_warp_amp;
    }
    int p = i * 5, o = (c * (points.length() / 5) + i) * 7;
    FNLfloat x = points[p], y = points[p + 1];
    results[o] = fnlGetNoise2D(s, x, y);
    fnlDomainWarp2D(s, x, y);
    results[o + 2] = x;
    results[o + 3] = y;
    FNLfloat X = points[p + 2], Y = points[p + 3], Z = points[p + 4];
    results[o + 1] = fnlGetNoise3D(s, X, Y, Z);
    fnlDomainWarp3D(s, X, Y, Z);
    results[o + 4] = X;
    results[o + 5] = Y;
    results[o + 6] = Z;
}

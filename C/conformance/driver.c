/* FastNoiseLite conformance driver (C). See conformance/README.md.
 * Usage: driver VECTORS_DIR > output.txt
 * FNL_HEADER selects the header (build.sh passes a double-precision copy for the double variant). */
#define FNL_IMPL
#ifndef FNL_HEADER
#define FNL_HEADER "../FastNoiseLite.h"
#endif
#include FNL_HEADER
#include <stdio.h>
#include <stdlib.h>

#define MAX_POINTS 256

static double p2[MAX_POINTS][2], p3[MAX_POINTS][3];
static int n2, n3;

static FILE *open_in(const char *dir, const char *name)
{
    char path[4096];
    FILE *f;
    snprintf(path, sizeof path, "%s/%s", dir, name);
    f = fopen(path, "r");
    if (!f)
    {
        fprintf(stderr, "cannot open %s\n", path);
        exit(2);
    }
    return f;
}

static void run(const fnl_state *s, int c)
{
    int i;
    for (i = 0; i < n2; i++)
        printf("N2 %d %d %.17g\n", c, i, (double)fnlGetNoise2D(s, (FNLfloat)p2[i][0], (FNLfloat)p2[i][1]));
    for (i = 0; i < n3; i++)
        printf("N3 %d %d %.17g\n", c, i, (double)fnlGetNoise3D(s, (FNLfloat)p3[i][0], (FNLfloat)p3[i][1], (FNLfloat)p3[i][2]));
    for (i = 0; i < n2; i++)
    {
        FNLfloat x = (FNLfloat)p2[i][0], y = (FNLfloat)p2[i][1];
        fnlDomainWarp2D(s, &x, &y);
        printf("W2 %d %d %.17g %.17g\n", c, i, (double)x, (double)y);
    }
    for (i = 0; i < n3; i++)
    {
        FNLfloat x = (FNLfloat)p3[i][0], y = (FNLfloat)p3[i][1], z = (FNLfloat)p3[i][2];
        fnlDomainWarp3D(s, &x, &y, &z);
        printf("W3 %d %d %.17g %.17g %.17g\n", c, i, (double)x, (double)y, (double)z);
    }
}

int main(int argc, char **argv)
{
    FILE *f;
    int c = 0, seed, nt, rot, ft, octaves, cdist, cret, wt;
    float freq, lac, gain, ws, pp, jitter, amp;
    fnl_state s;

    if (argc < 2)
    {
        fprintf(stderr, "usage: driver VECTORS_DIR\n");
        return 2;
    }
    f = open_in(argv[1], "points2d.txt");
    while (n2 < MAX_POINTS && fscanf(f, "%lf %lf", &p2[n2][0], &p2[n2][1]) == 2) n2++;
    fclose(f);
    f = open_in(argv[1], "points3d.txt");
    while (n3 < MAX_POINTS && fscanf(f, "%lf %lf %lf", &p3[n3][0], &p3[n3][1], &p3[n3][2]) == 3) n3++;
    fclose(f);

    s = fnlCreateState();
    run(&s, -1);

    f = open_in(argv[1], "configs.txt");
    while (fscanf(f, "%d %f %d %d %d %d %f %f %f %f %d %d %f %d %f", &seed, &freq, &nt, &rot, &ft, &octaves,
                  &lac, &gain, &ws, &pp, &cdist, &cret, &jitter, &wt, &amp) == 15)
    {
        s = fnlCreateState();
        s.seed = seed;
        s.frequency = freq;
        s.noise_type = (fnl_noise_type)nt;
        s.rotation_type_3d = (fnl_rotation_type_3d)rot;
        s.fractal_type = (fnl_fractal_type)ft;
        s.octaves = octaves;
        s.lacunarity = lac;
        s.gain = gain;
        s.weighted_strength = ws;
        s.ping_pong_strength = pp;
        s.cellular_distance_func = (fnl_cellular_distance_func)cdist;
        s.cellular_return_type = (fnl_cellular_return_type)cret;
        s.cellular_jitter_mod = jitter;
        s.domain_warp_type = (fnl_domain_warp_type)wt;
        s.domain_warp_amp = amp;
        run(&s, c++);
    }
    fclose(f);
    return 0;
}

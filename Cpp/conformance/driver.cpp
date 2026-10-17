// FastNoiseLite conformance driver (C++ reference). See conformance/README.md.
// Build with -DFNL_COORD=float or -DFNL_COORD=double.
// Usage: driver VECTORS_DIR > output.txt
#include "../FastNoiseLite.h"
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#ifndef FNL_COORD
#define FNL_COORD float
#endif
typedef FNL_COORD C;
typedef std::vector<std::vector<double>> Points;

static Points ReadPoints(const std::string& path)
{
    Points r;
    std::ifstream f(path);
    std::string line;
    while (std::getline(f, line))
    {
        std::istringstream ss(line);
        std::vector<double> v;
        double d;
        while (ss >> d) v.push_back(d);
        if (!v.empty()) r.push_back(v);
    }
    return r;
}

static void Run(const FastNoiseLite& n, int c, const Points& p2, const Points& p3)
{
    for (size_t i = 0; i < p2.size(); i++)
        printf("N2 %d %zu %.17g\n", c, i, (double)n.GetNoise((C)p2[i][0], (C)p2[i][1]));
    for (size_t i = 0; i < p3.size(); i++)
        printf("N3 %d %zu %.17g\n", c, i, (double)n.GetNoise((C)p3[i][0], (C)p3[i][1], (C)p3[i][2]));
    for (size_t i = 0; i < p2.size(); i++)
    {
        C x = (C)p2[i][0], y = (C)p2[i][1];
        n.DomainWarp(x, y);
        printf("W2 %d %zu %.17g %.17g\n", c, i, (double)x, (double)y);
    }
    for (size_t i = 0; i < p3.size(); i++)
    {
        C x = (C)p3[i][0], y = (C)p3[i][1], z = (C)p3[i][2];
        n.DomainWarp(x, y, z);
        printf("W3 %d %zu %.17g %.17g %.17g\n", c, i, (double)x, (double)y, (double)z);
    }
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "usage: driver VECTORS_DIR\n");
        return 2;
    }
    std::string dir = argv[1];
    Points p2 = ReadPoints(dir + "/points2d.txt");
    Points p3 = ReadPoints(dir + "/points3d.txt");

    Run(FastNoiseLite(), -1, p2, p3);

    std::ifstream f(dir + "/configs.txt");
    std::string line;
    int c = 0;
    while (std::getline(f, line))
    {
        if (line.empty()) continue;
        std::istringstream ss(line);
        int seed, nt, rot, ft, octaves, cdist, cret, wt;
        float freq, lac, gain, ws, pp, jitter, amp;
        ss >> seed >> freq >> nt >> rot >> ft >> octaves >> lac >> gain >> ws >> pp >> cdist >> cret >> jitter >> wt >> amp;

        FastNoiseLite n;
        n.SetSeed(seed);
        n.SetFrequency(freq);
        n.SetNoiseType((FastNoiseLite::NoiseType)nt);
        n.SetRotationType3D((FastNoiseLite::RotationType3D)rot);
        n.SetFractalType((FastNoiseLite::FractalType)ft);
        n.SetFractalOctaves(octaves);
        n.SetFractalLacunarity(lac);
        n.SetFractalGain(gain);
        n.SetFractalWeightedStrength(ws);
        n.SetFractalPingPongStrength(pp);
        n.SetCellularDistanceFunction((FastNoiseLite::CellularDistanceFunction)cdist);
        n.SetCellularReturnType((FastNoiseLite::CellularReturnType)cret);
        n.SetCellularJitter(jitter);
        n.SetDomainWarpType((FastNoiseLite::DomainWarpType)wt);
        n.SetDomainWarpAmp(amp);
        Run(n, c++, p2, p3);
    }
    return 0;
}

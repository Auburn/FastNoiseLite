// FastNoiseLite conformance driver (C#). See conformance/README.md.
// Usage: dotnet driver.dll VECTORS_DIR > output.txt
// The port picks its coordinate type with a file-local `using FNLfloat` alias, so build.sh builds the
// double variant from a copy with that alias switched and defines FNL_DOUBLE to match it here.
#if FNL_DOUBLE
using FNLfloat = System.Double;
#else
using FNLfloat = System.Single;
#endif
using System;
using System.Globalization;
using System.IO;
using System.Linq;

static class Driver
{
    static readonly TextWriter Out = new StreamWriter(Console.OpenStandardOutput());
    static double[][] p2, p3;

    static string[][] Rows(string dir, string name) =>
        File.ReadAllLines(Path.Combine(dir, name))
            .Select(l => l.Split((char[])null, StringSplitOptions.RemoveEmptyEntries))
            .Where(f => f.Length > 0).ToArray();

    static double[][] Points(string dir, string name) =>
        Rows(dir, name).Select(f => f.Select(double.Parse).ToArray()).ToArray();

    // "R" is the shortest string that round-trips; floats are widened to double first, like the C driver.
    static string G(double v) => v.ToString("R");

    // A call that throws skips its line (counted as missing) instead of ending the run.
    static void Emit(Func<string> line)
    {
        try { Out.WriteLine(line()); }
        catch (Exception) { }
    }

    static void Run(FastNoiseLite n, int c)
    {
        for (int i = 0; i < p2.Length; i++)
            Emit(() => $"N2 {c} {i} {G(n.GetNoise((FNLfloat)p2[i][0], (FNLfloat)p2[i][1]))}");
        for (int i = 0; i < p3.Length; i++)
            Emit(() => $"N3 {c} {i} {G(n.GetNoise((FNLfloat)p3[i][0], (FNLfloat)p3[i][1], (FNLfloat)p3[i][2]))}");
        for (int i = 0; i < p2.Length; i++)
            Emit(() =>
            {
                FNLfloat x = (FNLfloat)p2[i][0], y = (FNLfloat)p2[i][1];
                n.DomainWarp(ref x, ref y);
                return $"W2 {c} {i} {G(x)} {G(y)}";
            });
        for (int i = 0; i < p3.Length; i++)
            Emit(() =>
            {
                FNLfloat x = (FNLfloat)p3[i][0], y = (FNLfloat)p3[i][1], z = (FNLfloat)p3[i][2];
                n.DomainWarp(ref x, ref y, ref z);
                return $"W3 {c} {i} {G(x)} {G(y)} {G(z)}";
            });
    }

    static int Main(string[] args)
    {
        if (args.Length < 1)
        {
            Console.Error.WriteLine("usage: dotnet driver.dll VECTORS_DIR");
            return 2;
        }
        CultureInfo.CurrentCulture = CultureInfo.InvariantCulture;
        p2 = Points(args[0], "points2d.txt");
        p3 = Points(args[0], "points3d.txt");

        Run(new FastNoiseLite(), -1);

        int c = 0;
        foreach (var f in Rows(args[0], "configs.txt"))
        {
            // The C# enums have the same ordinals as the C++ ones.
            var n = new FastNoiseLite();
            n.SetSeed(int.Parse(f[0]));
            n.SetFrequency(float.Parse(f[1]));
            n.SetNoiseType((FastNoiseLite.NoiseType)int.Parse(f[2]));
            n.SetRotationType3D((FastNoiseLite.RotationType3D)int.Parse(f[3]));
            n.SetFractalType((FastNoiseLite.FractalType)int.Parse(f[4]));
            n.SetFractalOctaves(int.Parse(f[5]));
            n.SetFractalLacunarity(float.Parse(f[6]));
            n.SetFractalGain(float.Parse(f[7]));
            n.SetFractalWeightedStrength(float.Parse(f[8]));
            n.SetFractalPingPongStrength(float.Parse(f[9]));
            n.SetCellularDistanceFunction((FastNoiseLite.CellularDistanceFunction)int.Parse(f[10]));
            n.SetCellularReturnType((FastNoiseLite.CellularReturnType)int.Parse(f[11]));
            n.SetCellularJitter(float.Parse(f[12]));
            n.SetDomainWarpType((FastNoiseLite.DomainWarpType)int.Parse(f[13]));
            n.SetDomainWarpAmp(float.Parse(f[14]));
            Run(n, c++);
        }
        Out.Flush();
        return 0;
    }
}

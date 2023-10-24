// USAGE: Run with `dotnet run --configuration Release` from the CSharp directory

using System.Drawing;
using static FastNoiseLite;

class Program
{
    private const int SKIP_WINDOW = 7919;

    private const int WIDTH = 512;
    private const int HEIGHT = 512;

    private static readonly int[] SEED = { -10, 1337 };
    private static readonly float[] FREQUENCY = { -0.001f, 0.01f, 0.1f, 10f };
    private static readonly NoiseType[] NOISE_TYPE = {
        NoiseType.OpenSimplex2,
        NoiseType.OpenSimplex2S,
        NoiseType.Cellular,
        NoiseType.Perlin,
        NoiseType.ValueCubic,
        NoiseType.Value,
    };
    private static readonly RotationType3D[] ROTATION_TYPE_3D = {
        RotationType3D.None,
        RotationType3D.ImproveXYPlanes,
        RotationType3D.ImproveXZPlanes,
    };

    private static readonly FractalType[] FRACTAL_TYPE = {
        FractalType.None,
        FractalType.FBm,
        FractalType.Ridged,
        FractalType.PingPong,
        FractalType.DomainWarpProgressive,
        FractalType.DomainWarpIndependent,
    };
    private static readonly int[] OCTAVES = { 1, 2, 5 };
    // Only when octaves != 1
    private static readonly float[] LACUNARITY = { -0.1f, 0f, 1f };
    // Only when octaves != 1
    private static readonly float[] GAIN = { -1.5f, 0f, 0.1f };
    // Only when octaves != 1
    private static readonly float[] WEIGHTED_STRENGTH = { -1.5f, 0f, 0.1f };
    // Only when fractal_type = FractalType.PingPong
    private static readonly float[] PING_PONG_STRENGTH = { -1.5f, 0f, 0.1f };

    // Only when noise_type = NoiseType.Cellular
    private static readonly CellularDistanceFunction[] CELLULAR_DISTANCE_FUNCTION = {
        CellularDistanceFunction.Euclidean,
        CellularDistanceFunction.EuclideanSq,
        CellularDistanceFunction.Manhattan,
        CellularDistanceFunction.Hybrid,
    };
    // Only when noise_type = NoiseType.Cellular
    private static readonly CellularReturnType[] CELLULAR_RETURN_TYPE = {
        CellularReturnType.CellValue,
        CellularReturnType.Distance,
        CellularReturnType.Distance2,
        CellularReturnType.Distance2Add,
        CellularReturnType.Distance2Sub,
        CellularReturnType.Distance2Mul,
        CellularReturnType.Distance2Div,
    };
    // Only when noise_type = NoiseType.Cellular
    private static readonly float[] CELLULAR_JITTER_MODIFIER = { -1.5f, 0f, 0.1f };

    // Only when noise_type = NoiseType.Cellular
    private static readonly DomainWarpType[] DOMAIN_WARP_TYPE = {
        DomainWarpType.OpenSimplex2,
        DomainWarpType.OpenSimplex2Reduced,
        DomainWarpType.BasicGrid,
    };
    // Only when noise_type = NoiseType.Cellular
    private static readonly float[] DOMAIN_WARP_AMP = { -100f, 0f, 1000f };

    static void Main(string[] args)
    {
        var baseOptions = new List<(int, float, RotationType3D)>();
        foreach (var a in SEED)
        {
            foreach (var b in FREQUENCY)
            {
                foreach (var c in ROTATION_TYPE_3D)
                {
                    baseOptions.Add((a, b, c));
                }
            }
        }

        var octavesOptions = new List<(int, float, float, float)>();
        foreach (var a in OCTAVES)
        {
            if (a != 1)
            {
                foreach (var b in LACUNARITY)
                {
                    foreach (var c in GAIN)
                    {
                        foreach (var d in WEIGHTED_STRENGTH)
                        {
                            octavesOptions.Add((a, b, c, d));
                        }
                    }
                }
            }
            else
            {
                octavesOptions.Add((a, 2f, 0.5f, 0f));
            }
        }

        var fractalTypeOptions = new List<(FractalType, float)>();
        foreach (var a in FRACTAL_TYPE)
        {
            if (a == FractalType.PingPong)
            {
                foreach (var b in PING_PONG_STRENGTH)
                {
                    fractalTypeOptions.Add((a, b));
                }
            }
            else
            {
                fractalTypeOptions.Add((a, 2f));
            }
        }

        var noiseTypeOptions = new List<(NoiseType, CellularDistanceFunction, CellularReturnType, float, DomainWarpType, float)>();
        foreach (var a in NOISE_TYPE)
        {
            if (a == NoiseType.Cellular)
            {
                foreach (var b in CELLULAR_DISTANCE_FUNCTION)
                {
                    foreach (var c in CELLULAR_RETURN_TYPE)
                    {
                        foreach (var d in CELLULAR_JITTER_MODIFIER)
                        {
                            foreach (var e in DOMAIN_WARP_TYPE)
                            {
                                foreach (var f in DOMAIN_WARP_AMP)
                                {
                                    noiseTypeOptions.Add((a, b, c, d, e, f));
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                noiseTypeOptions.Add((a, CellularDistanceFunction.EuclideanSq, CellularReturnType.Distance, 1f, DomainWarpType.OpenSimplex2, 1f));
            }
        }

        var total = baseOptions.Count * octavesOptions.Count * fractalTypeOptions.Count * noiseTypeOptions.Count;
        var options = new List<(int, float, RotationType3D, int, float, float, float, FractalType, float, NoiseType, CellularDistanceFunction, CellularReturnType, float, DomainWarpType, float)>(total);

        foreach (var (b1, b2, b3) in baseOptions)
        {
            foreach (var (o1, o2, o3, o4) in octavesOptions)
            {
                foreach (var (f1, f2) in fractalTypeOptions)
                {
                    foreach (var (n1, n2, n3, n4, n5, n6) in noiseTypeOptions)
                    {
                        options.Add((b1, b2, b3, o1, o2, o3, o4, f1, f2, n1, n2, n3, n4, n5, n6));
                    }
                }
            }
        }

        Console.WriteLine($"Writing {total / SKIP_WINDOW} images, a subset of the total {total}");

        Parallel.For(0, options.Count / SKIP_WINDOW, i =>
        {
            i *= SKIP_WINDOW;
            // if (i != 3072572) return;

            var (seed, frequency, rotationType3D, octaves, lacunarity, gain, weightedStrength,
                fractalType, pingPongStrength, noiseType, cellularDistanceFunction, cellularReturnType,
                cellularJitterModifier, domainWarpType, domainWarpAmp) = options[i];

            var noise = new FastNoiseLite(seed);
            noise.SetFrequency(frequency);
            noise.SetRotationType3D(rotationType3D);

            noise.SetFractalOctaves(octaves);
            noise.SetFractalLacunarity(lacunarity);
            noise.SetFractalGain(gain);
            noise.SetFractalWeightedStrength(weightedStrength);

            noise.SetFractalType(fractalType);
            noise.SetFractalPingPongStrength(pingPongStrength);

            noise.SetNoiseType(noiseType);
            noise.SetCellularDistanceFunction(cellularDistanceFunction);
            noise.SetCellularReturnType(cellularReturnType);
            noise.SetCellularJitter(cellularJitterModifier);
            noise.SetDomainWarpType(domainWarpType);
            noise.SetDomainWarpAmp(domainWarpAmp);

            // var filePath = $"../../images-csharp/{i}.bytes";
            // using var fileStream = new FileStream(filePath, FileMode.Create);
            // using var binaryWriter = new BinaryWriter(fileStream);
            // for (var x = 0; x < WIDTH; x++)
            // {
            //     for (var y = 0; y < HEIGHT; y++)
            //     {
            //         var value = noise.GetNoise(x, y);
            //         value = Math.Clamp((value + 1) / 2 * 255, 0f, 255f);
            //         var byteValue = (byte)(int)value;
            //         binaryWriter.Write(byteValue);
            //     }
            // }
            // binaryWriter.Close();
            // fileStream.Close();

            // ///
            // float value = noise.GetNoise(184 - 1, 109 - 1);
            // value = Math.Clamp((value + 1) / 2 * 255, 0f, 255f);
            // var byteValue = (byte)(int)value;
            // Console.WriteLine($"C# value: {byteValue}");
            // ///

            var bitmap = new Bitmap(WIDTH, HEIGHT);
            for (var x = 0; x < WIDTH; x++)
            {
                for (var y = 0; y < HEIGHT; y++)
                {
                    float value = noise.GetNoise(4194280 + (float)x, 4194280 + (float)y);
                    value = Math.Clamp((value + 1) / 2 * 255, 0f, 255f);
                    var byteValue = (byte)(int)value;
                    bitmap.SetPixel(x, y, Color.FromArgb(byteValue, byteValue, byteValue));
                }
            }
            // After the PNGs are generated, use this command from the Windows terminal to convert them to BMPs:
            // for %i in (*.png) do ffmpeg -i "%i" -c:v bmp -pix_fmt rgb24 "%~ni.bmp"
            bitmap.Save($"../../images-csharp/{i}.png");
        });
    }
}

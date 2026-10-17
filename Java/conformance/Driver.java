// FastNoiseLite conformance driver (Java). See conformance/README.md.
// Usage: java -cp CLASSES Driver VECTORS_DIR > output.txt
// The double variant is built by applying the port's documented file-wide replace
// ("/*FNLfloat*/ float" -> "/*FNLfloat*/ double") to the port and to this file, so the
// coordinates below follow the variant's precision.
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public class Driver
{
    static final StringBuilder out = new StringBuilder();

    static List<String[]> rows(String dir, String name) throws IOException
    {
        List<String[]> r = new ArrayList<>();
        for (String line : Files.readAllLines(Paths.get(dir, name)))
            if (!line.trim().isEmpty()) r.add(line.trim().split("\\s+"));
        return r;
    }

    static List<double[]> points(String dir, String name) throws IOException
    {
        List<double[]> r = new ArrayList<>();
        for (String[] f : rows(dir, name))
        {
            double[] p = new double[f.length];
            for (int i = 0; i < f.length; i++) p[i] = Double.parseDouble(f[i]);
            r.add(p);
        }
        return r;
    }

    static /*FNLfloat*/ float coord(double v) { return (/*FNLfloat*/ float)v; }

    // Widening to double makes Double.toString print the exact value with up to 17 digits.
    static void line(String head, double... v)
    {
        out.append(head);
        for (double d : v) out.append(' ').append(d);
        out.append('\n');
    }

    static void run(FastNoiseLite n, int c, List<double[]> p2, List<double[]> p3)
    {
        // A call that throws is skipped (it then counts as missing) instead of ending the run.
        for (int i = 0; i < p2.size(); i++)
        {
            double[] p = p2.get(i);
            try { line("N2 " + c + " " + i, n.GetNoise(coord(p[0]), coord(p[1]))); } catch (RuntimeException e) { }
        }
        for (int i = 0; i < p3.size(); i++)
        {
            double[] p = p3.get(i);
            try { line("N3 " + c + " " + i, n.GetNoise(coord(p[0]), coord(p[1]), coord(p[2]))); } catch (RuntimeException e) { }
        }
        for (int i = 0; i < p2.size(); i++)
        {
            double[] p = p2.get(i);
            FastNoiseLite.Vector2 v = new FastNoiseLite.Vector2(coord(p[0]), coord(p[1]));
            try { n.DomainWarp(v); line("W2 " + c + " " + i, v.x, v.y); } catch (RuntimeException e) { }
        }
        for (int i = 0; i < p3.size(); i++)
        {
            double[] p = p3.get(i);
            FastNoiseLite.Vector3 v = new FastNoiseLite.Vector3(coord(p[0]), coord(p[1]), coord(p[2]));
            try { n.DomainWarp(v); line("W3 " + c + " " + i, v.x, v.y, v.z); } catch (RuntimeException e) { }
        }
    }

    public static void main(String[] args) throws IOException
    {
        if (args.length < 1)
        {
            System.err.println("usage: java Driver VECTORS_DIR");
            System.exit(2);
        }
        List<double[]> p2 = points(args[0], "points2d.txt");
        List<double[]> p3 = points(args[0], "points3d.txt");

        run(new FastNoiseLite(), -1, p2, p3);

        int c = 0;
        // Java's enums are declared in the same order as the C++ ones, so ordinals map directly.
        for (String[] f : rows(args[0], "configs.txt"))
        {
            FastNoiseLite n = new FastNoiseLite();
            n.SetSeed(Integer.parseInt(f[0]));
            n.SetFrequency(Float.parseFloat(f[1]));
            n.SetNoiseType(FastNoiseLite.NoiseType.values()[Integer.parseInt(f[2])]);
            n.SetRotationType3D(FastNoiseLite.RotationType3D.values()[Integer.parseInt(f[3])]);
            n.SetFractalType(FastNoiseLite.FractalType.values()[Integer.parseInt(f[4])]);
            n.SetFractalOctaves(Integer.parseInt(f[5]));
            n.SetFractalLacunarity(Float.parseFloat(f[6]));
            n.SetFractalGain(Float.parseFloat(f[7]));
            n.SetFractalWeightedStrength(Float.parseFloat(f[8]));
            n.SetFractalPingPongStrength(Float.parseFloat(f[9]));
            n.SetCellularDistanceFunction(FastNoiseLite.CellularDistanceFunction.values()[Integer.parseInt(f[10])]);
            n.SetCellularReturnType(FastNoiseLite.CellularReturnType.values()[Integer.parseInt(f[11])]);
            n.SetCellularJitter(Float.parseFloat(f[12]));
            n.SetDomainWarpType(FastNoiseLite.DomainWarpType.values()[Integer.parseInt(f[13])]);
            n.SetDomainWarpAmp(Float.parseFloat(f[14]));
            run(n, c++, p2, p3);
        }
        System.out.print(out);
    }
}

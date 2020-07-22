// Adapted from https://stackoverflow.com/a/34801225
// This fixes the SetPixel speed issue.
using System;
using Eto.Drawing;

public class FastBitmap
{
    public Int32[] Bits { get; private set; }
    public int Height { get; private set; }
    public int Width { get; private set; }

    public FastBitmap(int width, int height)
    {
        Width = width;
        Height = height;
        Bits = new Int32[width * height];
    }

    public void SetPixel(int x, int y, Color colour)
    {
        int index = x + (y * Width);
        int col = colour.ToArgb();

        Bits[index] = col;
    }

    public Color GetPixel(int x, int y)
    {
        int index = x + (y * Width);
        int col = Bits[index];
        Color result = Color.FromArgb(col);

        return result;
    }

    public Bitmap CreateBitmap() {
        return new Bitmap(Width, Height, PixelFormat.Format32bppRgba, Bits);
    }
}
using System;
using System.Diagnostics;
using System.Text.RegularExpressions;
using Eto.Drawing;
using Eto.Forms;

namespace FastNoiseLite
{
    public class MainForm : Form
    {
        private Button GenerateButton;
        private Button UpButton;
        private Button DownButton;

        private DropDown PreviewSize;
        private CheckBox Invert;
        private CheckBox Is3D;
        private DropDown GradientPerturb;

        private DropDown NoiseType;
        private NumericStepper Seed;
        private NumericStepper Frequency;
        private DropDown Interpolation;

        private DropDown FractalType;
        private NumericStepper FractalOctaves;
        private NumericStepper FractalLacunarity;
        private NumericStepper FractalGain;

        private DropDown CellularDistanceFunction;
        private DropDown CellularReturnType;
        private NumericStepper CellularDistance2Index0;
        private NumericStepper CellularDistance2Index1;
        private NumericStepper CellularJitter;

        private NumericStepper GPAmplitude;
        private NumericStepper GPFrequency;

        public Bitmap Bitmap;
        private ImageView Image;

        private float zPos = 0;

        private Label Time;
        private Label Min;
        private Label Max;
        private Label Mean;
        private Label ExtraInfo;

        public MainForm()
        {
            // Sets the client (inner) size of the window for your content
            ClientSize = new Size(1050, 820);

            Title = "FastNoise Lite GUI";

            // Create main layout, makes controls and result appear side by side
            var mainLayout = new StackLayout();
            mainLayout.Orientation = Orientation.Horizontal;
            mainLayout.Spacing = 5;
            mainLayout.Padding = new Padding(10);
            mainLayout.HorizontalContentAlignment = HorizontalAlignment.Left;

            // Control panel
            var controlPanel = new StackLayout();
            controlPanel.Orientation = Orientation.Vertical;
            controlPanel.Spacing = 5;

            {
                // Generate, Up, Down
                {
                    var layout = new StackLayout()
                    {
                        Orientation = Orientation.Horizontal,
                        Spacing = 3
                    };

                    // TODO: Events

                    GenerateButton = new Button();
                    GenerateButton.Text = "Generate";
                    GenerateButton.Click += Generate;
                    layout.Items.Add(GenerateButton);

                    UpButton = new Button();
                    UpButton.Text = "Up";
                    UpButton.Enabled = false;
                    UpButton.Click += OnUp;
                    layout.Items.Add(UpButton);

                    DownButton = new Button();
                    DownButton.Text = "Down";
                    DownButton.Enabled = false;
                    DownButton.Click += OnDown;
                    layout.Items.Add(DownButton);

                    controlPanel.Items.Add(layout);
                }

                // Control table
                var table = new TableLayout();
                table.Spacing = new Size(5, 5);

                // Preview controls
                {
                    // Preview size dropdown
                    {
                        PreviewSize = new DropDown();
                        PreviewSize.Items.Add("256");
                        PreviewSize.Items.Add("512");
                        PreviewSize.Items.Add("768");
                        PreviewSize.Items.Add("1024");
                        PreviewSize.SelectedIndex = 2;
                        PreviewSize.SelectedIndexChanged += Generate;
                        AddToTableWithLabel(table, PreviewSize, "Preview Size:");
                    }

                    // Invert
                    Invert = new CheckBox();
                    Invert.CheckedChanged += Generate;
                    AddToTableWithLabel(table, Invert, "Invert:");

                    // 3D
                    Is3D = new CheckBox();
                    Is3D.CheckedChanged += OnUIUpdate;
                    AddToTableWithLabel(table, Is3D, "3D:");

                    // Gradient Perturb Dropdown
                    {
                        GradientPerturb = new DropDown();
                        GradientPerturb.Items.Add("False");
                        GradientPerturb.Items.Add("True");
                        GradientPerturb.Items.Add("True - Fractal");
                        GradientPerturb.SelectedIndex = 0;
                        GradientPerturb.SelectedIndexChanged += Generate;
                        AddToTableWithLabel(table, GradientPerturb, "Gradient Perturb:");
                    }
                }

                // TODO: Work out how to make this bold
                table.Rows.Add(new TableRow { Cells = { new Label { Text = "General:" } } });

                // General properties
                {
                    // Noise Type
                    {
                        NoiseType = new DropDown();
                        foreach (var name in Enum.GetNames(typeof(Old.FastNoise.NoiseType))) {
                            NoiseType.Items.Add(FormatReadable(name));
                        }
                        NoiseType.Items.Add("Gradient Perturb");
                        NoiseType.Items.Add("Gradient Perturb Fractal");
                        NoiseType.SelectedIndex = (int)Old.FastNoise.NoiseType.SimplexFractal;
                        NoiseType.SelectedIndexChanged += OnUIUpdate;
                        AddToTableWithLabel(table, NoiseType, "Noise Type:");
                    }

                    // Seed
                    Seed = new NumericStepper { Value = 1337 };
                    Seed.ValueChanged += Generate; // TODO: This is super laggy. Need to maybe look for enter key, or leave focus?
                    AddToTableWithLabel(table, Seed, "Seed:");

                    // Frequency
                    Frequency = new NumericStepper { Value = 0.02, DecimalPlaces = 2 };
                    Frequency.ValueChanged += Generate;
                    AddToTableWithLabel(table, Frequency, "Frequency:");

                    // Interpolation
                    {
                        Interpolation = new DropDown();
                        foreach (var name in Enum.GetNames(typeof(Old.FastNoise.Interp))) {
                            Interpolation.Items.Add(FormatReadable(name));
                        }
                        Interpolation.SelectedIndex = (int)Old.FastNoise.Interp.Quintic;
                        Interpolation.SelectedIndexChanged += Generate;
                        AddToTableWithLabel(table, Interpolation, "Interpolation:");
                    }
                }

                // Add fractal label
                table.Rows.Add(new TableRow { Cells = { new Label { Text = "Fractal:" } } });

                {
                    // Fractal type
                    {
                        FractalType = new DropDown();
                        foreach (var name in Enum.GetNames(typeof(Old.FastNoise.FractalType))) {
                            FractalType.Items.Add(FormatReadable(name));
                        }
                        FractalType.SelectedIndex = (int)Old.FastNoise.FractalType.FBM;
                        FractalType.SelectedIndexChanged += Generate;
                        AddToTableWithLabel(table, FractalType, "Type:");
                    }

                    // Octaves
                    FractalOctaves = new NumericStepper { Value = 5 };
                    FractalOctaves.ValueChanged += Generate;
                    AddToTableWithLabel(table, FractalOctaves, "Octaves:");

                    // Lacunarity
                    FractalLacunarity = new NumericStepper { Value = 2.0, DecimalPlaces = 2 };
                    FractalLacunarity.ValueChanged += Generate;
                    AddToTableWithLabel(table, FractalLacunarity, "Lacunarity:");

                    // Gain
                    FractalGain = new NumericStepper { Value = 0.5, DecimalPlaces = 2 };
                    FractalGain.ValueChanged += Generate;
                    AddToTableWithLabel(table, FractalGain, "Gain:");
                }

                // Add fractal label
                table.Rows.Add(new TableRow { Cells = { new Label { Text = "Cellular:" } } });

                {
                    // Distance Function
                    {
                        CellularDistanceFunction = new DropDown();
                        foreach (var name in Enum.GetNames(typeof(Old.FastNoise.CellularDistanceFunction))) {
                            CellularDistanceFunction.Items.Add(FormatReadable(name));
                        }
                        CellularDistanceFunction.SelectedIndex = (int)Old.FastNoise.CellularDistanceFunction.Euclidean;
                        CellularDistanceFunction.SelectedIndexChanged += Generate;
                        AddToTableWithLabel(table, CellularDistanceFunction, "Distance Function:");
                    }

                    // Return Type
                    {
                        CellularReturnType = new DropDown();
                        foreach (var name in Enum.GetNames(typeof(Old.FastNoise.CellularReturnType))) {
                            CellularReturnType.Items.Add(FormatReadable(name));
                        }
                        CellularReturnType.SelectedIndex = (int)Old.FastNoise.CellularReturnType.CellValue;
                        CellularReturnType.SelectedIndexChanged += Generate;
                        AddToTableWithLabel(table, CellularReturnType, "Return Type:");
                    }

                    // Distance2 Indices
                    {
                        // Create elements
                        CellularDistance2Index0 = new NumericStepper() { Value = 0 };
                        CellularDistance2Index0.ValueChanged += Generate;
                        CellularDistance2Index1 = new NumericStepper() { Value = 1 };
                        CellularDistance2Index1.ValueChanged += Generate;

                        // Contain them
                        var stack = new StackLayout()
                        {
                            Orientation = Orientation.Horizontal,
                            Items = {
                                CellularDistance2Index0,
                                CellularDistance2Index1
                            }
                        };

                        // Add to table
                        AddToTableWithLabel(table, stack, "Distance2 Indices:");
                    }

                    // Jitter
                    CellularJitter = new NumericStepper() { Value = 0.45, DecimalPlaces = 2 };
                    CellularJitter.ValueChanged += Generate;
                    AddToTableWithLabel(table, CellularJitter, "Jitter:");
                }

                // Add fractal label
                table.Rows.Add(new TableRow { Cells = { new Label { Text = "Gradient Perturb:" } } });

                {
                    // Amplitude
                    GPAmplitude = new NumericStepper { Value = 30.0, DecimalPlaces = 2 };
                    GPAmplitude.ValueChanged += Generate;
                    AddToTableWithLabel(table, GPAmplitude, "Amplitude:");

                    // Frequency
                    GPFrequency = new NumericStepper { Value = 0.01, DecimalPlaces = 2 };
                    GPFrequency.ValueChanged += Generate;
                    AddToTableWithLabel(table, GPFrequency, "Frequency:");
                }

                // Add table to the controls panel
                controlPanel.Items.Add(table);

                // Add save and github buttons
                {
                    var stack = new StackLayout();
                    stack.Orientation = Orientation.Horizontal;

                    var save = new Button { Text = "Save" };
                    save.Click += Save;
                    stack.Items.Add(new StackLayoutItem(save));

                    var github = new Button { Text = "GitHub" };
                    github.Click += (sender, e) => { Process.Start(new ProcessStartInfo("https://github.com/Auburns/FastNoise") { UseShellExecute = true }); };
                    stack.Items.Add(new StackLayoutItem(github));

                    controlPanel.Items.Add(stack);
                }
            }

            // Add control panel to layout
            mainLayout.Items.Add(controlPanel);

            // Add output panel
            {
                var layout = new StackLayout();
                layout.Orientation = Orientation.Vertical;

                // Info
                {
                    var infoPane = new StackLayout();
                    infoPane.Orientation = Orientation.Horizontal;
                    infoPane.Spacing = 15;

                    // First info column
                    {
                        var column = new StackLayout();
                        column.Orientation = Orientation.Vertical;

                        // Time
                        Time = new Label { Text = "Time (ms): N/A" };
                        column.Items.Add(new StackLayoutItem(Time));

                        // Mean
                        Mean = new Label { Text = "Mean: N/A" };
                        column.Items.Add(new StackLayoutItem(Mean));

                        infoPane.Items.Add(column);
                    }

                    // Second info column
                    {
                        var column = new StackLayout();
                        column.Orientation = Orientation.Vertical;

                        // Time
                        Min = new Label { Text = "Min: N/A" };
                        column.Items.Add(new StackLayoutItem(Min));

                        // Mean
                        Max = new Label { Text = "Max: N/A" };
                        column.Items.Add(new StackLayoutItem(Max));

                        infoPane.Items.Add(column);
                    }

                    // Third entry
                    {
                        ExtraInfo = new Label();
                        infoPane.Items.Add(ExtraInfo);
                    }

                    layout.Items.Add(new StackLayoutItem(infoPane));
                }

                Image = new ImageView();
                //var imageScroll = new Scrollable();
                //imageScroll.Content = Image;
                layout.Items.Add(new StackLayoutItem(Image));
                mainLayout.Items.Add(layout);
            }

            // Display the layout
            Content = mainLayout;

            // Ensure UI state is valid then it generates
            OnUIUpdate(null, null);
        }

        private void Generate(object sender, EventArgs e)
        {
            // Create noise generators
            var fNoise = new Old.FastNoise();
            var perturbNoise = new Old.FastNoise();

            perturbNoise.SetFrequency(0.015f);

            int size = 512;
            try { size = Convert.ToInt32(PreviewSize.SelectedKey); }
            catch (Exception ex) { MessageBox.Show("Size error: " + ex.ToString()); }

            fNoise.SetNoiseType((Old.FastNoise.NoiseType)NoiseType.SelectedIndex);
            fNoise.SetInterp((Old.FastNoise.Interp)Interpolation.SelectedIndex);
            fNoise.SetSeed((int)Seed.Value);
            fNoise.SetFrequency((float) Frequency.Value);
            fNoise.SetFractalType((Old.FastNoise.FractalType)FractalType.SelectedIndex);
            fNoise.SetFractalOctaves((int)FractalOctaves.Value);
            fNoise.SetFractalLacunarity((float) FractalLacunarity.Value);
            fNoise.SetFractalGain((float) FractalGain.Value);
            fNoise.SetGradientPerturbAmp((float) GPAmplitude.Value);

            perturbNoise.SetGradientPerturbAmp((float) GPAmplitude.Value);
            perturbNoise.SetFrequency((float) GPFrequency.Value);

            fNoise.SetCellularDistanceFunction((Old.FastNoise.CellularDistanceFunction)CellularDistanceFunction.SelectedIndex);
            fNoise.SetCellularReturnType((Old.FastNoise.CellularReturnType)CellularReturnType.SelectedIndex);
            fNoise.SetCellularDistance2Indicies((int)CellularDistance2Index0.Value, (int)CellularDistance2Index1.Value);
            fNoise.SetCellularJitter((float) CellularJitter.Value);

            Old.FastNoise lookupNoise = new Old.FastNoise();
            lookupNoise.SetFrequency(0.2f);
            lookupNoise.SetNoiseType(Old.FastNoise.NoiseType.Simplex);
            fNoise.SetCellularNoiseLookup(lookupNoise);

            var bitmap = new FastBitmap(size, size);
            float noise;
            float minN = 99999;
            float maxN = -99999;
            float avg = 0;

            bool get3d = Is3D.Checked == true; // Stupid!
            int halfSize = size / 2;
            bool fractal = NoiseType.SelectedKey.Contains("Fractal");

            // Timer
            Stopwatch sw = new Stopwatch();

            int index = 0;
            if (!NoiseType.SelectedKey.Contains("Perturb"))
            {
                var noiseValues = new float[size * size];
                int warpIndex = GradientPerturb.SelectedIndex;

                sw.Start();
                if (get3d)
                {
                    for (var x = 0; x < size; x++)
                    {
                        for (var y = 0; y < size; y++)
                        {
                            float xf = (float)(x - halfSize);
                            float yf = (float)(y - halfSize);
                            float zf = (float)(zPos);

                            switch (warpIndex)
                            {
                                case 1:
                                    perturbNoise.GradientPerturb(ref xf, ref yf, ref zf);
                                    break;
                                case 2:
                                    perturbNoise.GradientPerturbFractal(ref xf, ref yf, ref zf);
                                    break;
                            }

                            noise = fNoise.GetNoise(xf, yf, zf);
                            avg += noise;
                            maxN = Math.Max(maxN, noise);
                            minN = Math.Min(minN, noise);
                            noiseValues[index++] = noise;
                        }
                    }
                }
                else
                {
                    for (var x = 0; x < size; x++)
                    {
                        for (var y = 0; y < size; y++)
                        {
                            float xf = (float)(x - halfSize);
                            float yf = (float)(y - halfSize);

                            switch (warpIndex)
                            {
                                case 1:
                                    perturbNoise.GradientPerturb(ref xf, ref yf);
                                    break;
                                case 2:
                                    perturbNoise.GradientPerturbFractal(ref xf, ref yf);
                                    break;
                            }

                            noise = fNoise.GetNoise(xf, yf);
                            avg += noise;
                            maxN = Math.Max(maxN, noise);
                            minN = Math.Min(minN, noise);
                            noiseValues[index++] = noise;
                        }
                    }
                }
                sw.Stop();

                avg /= index - 1;
                index = 0;
                float scale = 255 / (maxN - minN);

                for (var x = 0; x < size; x++)
                {
                    for (var y = 0; y < size; y++)
                    {
                        noise = noiseValues[index++];

                        byte value = (byte)Math.Max(0, Math.Min(255, (noise - minN) * scale));
                        if (Invert.Checked == true)
                            value = (byte)(255 - value);

                        bitmap.SetPixel(x, y, Color.FromArgb(value, value, value));
                    }
                }
            }
            else
            {
                var noiseValues = new float[size * size * 3];

                sw.Start();
                if (get3d)
                {
                    for (var x = 0; x < size; x++)
                    {
                        for (var y = 0; y < size; y++)
                        {
                            float xf = (float)x;
                            float yf = (float)y;
                            float zf = (float)zPos;

                            if (fractal)
                                fNoise.GradientPerturbFractal(ref xf, ref yf, ref zf);
                            else fNoise.GradientPerturb(ref xf, ref yf, ref zf);

                            xf -= x;
                            yf -= y;
                            zf -= zPos;

                            avg += xf + yf + zf;
                            maxN = Math.Max(maxN, Math.Max(Math.Max(xf, yf), zf));
                            minN = Math.Min(minN, Math.Min(Math.Min(xf, yf), zf));

                            noiseValues[index++] = xf;
                            noiseValues[index++] = yf;
                            noiseValues[index++] = zf;
                        }
                    }
                }
                else
                {
                    for (var x = 0; x < size; x++)
                    {
                        for (var y = 0; y < size; y++)
                        {
                            float xf = (float)x;
                            float yf = (float)y;

                            if (fractal)
                                fNoise.GradientPerturbFractal(ref xf, ref yf);
                            else fNoise.GradientPerturb(ref xf, ref yf);

                            xf -= x;
                            yf -= y;

                            avg += xf + yf;
                            maxN = Math.Max(maxN, Math.Max(xf, yf));
                            minN = Math.Min(minN, Math.Min(xf, yf));

                            noiseValues[index++] = xf;
                            noiseValues[index++] = yf;
                        }
                    }
                }
                sw.Stop();

                if (get3d)
                    avg /= (index - 1) * 3;
                else avg /= (index - 1) * 2;

                index = 0;
                float scale = 255 / (maxN - minN);

                for (var x = 0; x < size; x++)
                {
                    for (var y = 0; y < size; y++)
                    {
                        byte red;
                        byte green;
                        byte blue;

                        if (get3d)
                        {
                            red = (byte)Math.Max(0, Math.Min(255, (noiseValues[index++] - minN) * scale));
                            green = (byte)Math.Max(0, Math.Min(255, (noiseValues[index++] - minN) * scale));
                            blue = (byte)Math.Max(0, Math.Min(255, (noiseValues[index++] - minN) * scale));
                        }
                        else
                        {
                            var vx = (noiseValues[index++] - minN);
                            var vy = (noiseValues[index++] - minN);

                            byte h = (byte)(vx * scale);
                            byte v = (byte)(vy * scale);

                            // https://discordapp.com/channels/703636892901441577/703638416335568926/735228970231922800
                            //byte h = (byte) (Math.Atan2(vx, vy) * (127.5 * Math.PI) + 127.5);
                            //byte v = (byte) Math.Sqrt(vx * vx + vy * vy);

                            HsvToRgb(h, 255, v, out red, out green, out blue);
                        }

                        if (Invert.Checked == true)
                        {
                            red = (byte)(255 - red);
                            green = (byte)(255 - green);
                            blue = (byte)(255 - blue);
                        }

                        bitmap.SetPixel(x, y, Color.FromArgb(red, green, blue));
                    }
                }
            }

            // Set image
            Bitmap = bitmap.CreateBitmap();
            Image.Image = Bitmap;

            // Set info labels
            Time.Text = "Time (ms): " + sw.ElapsedMilliseconds.ToString();
            Mean.Text = "Mean: " + avg.ToString();
            Min.Text = "Min: " + minN.ToString();
            Max.Text = "Max: " + maxN.ToString();
        }

        private void OnUp(object sender, EventArgs e)
        {
            try
            {
                zPos += ((float) Frequency.Value) * 100.0f;
                Generate(null, null);
            }
            catch (Exception ex) { MessageBox.Show("Frequency error: " + ex.ToString()); }
        }

        private void OnDown(object sender, EventArgs e)
        {
            try
            {
                zPos -= ((float) Frequency.Value) * 100.0f;
                Generate(null, null);
            }
            catch (Exception ex) { MessageBox.Show("Frequency error: " + ex.ToString()); }
        }

        private void OnUIUpdate(object sender, EventArgs e)
        {
            // 3D contorls
            UpButton.Enabled = DownButton.Enabled = (Is3D.Checked == true);

            // Interpolation
            if (NoiseType.SelectedKey.Contains("Value") ||
               NoiseType.SelectedKey.Contains("Perlin") ||
               NoiseType.SelectedKey.Contains("Perturb"))
            {
                Interpolation.Enabled = true;
            }
            else Interpolation.Enabled = false;

            // Warp
            var perturbVis = NoiseType.SelectedKey.Contains("Perturb");
            GradientPerturb.Enabled = !perturbVis;

            if (perturbVis)
            {
                ExtraInfo.Text = "Visualisation of gradient perturb:\r\nHue = X offset, Brightness = Y offset";
            }
            else ExtraInfo.Text = "";

            // Fractal options
            var fractal = NoiseType.SelectedKey.Contains("Fractal");
            FractalType.Enabled = fractal & !perturbVis;
            FractalOctaves.Enabled = fractal;
            FractalLacunarity.Enabled = fractal;
            FractalGain.Enabled = fractal;

            // Cellular
            var cellular = NoiseType.SelectedKey.Contains("Cellular");
            CellularDistanceFunction.Enabled = cellular;
            CellularReturnType.Enabled = cellular;
            CellularDistance2Index0.Enabled = cellular;
            CellularDistance2Index1.Enabled = cellular;
            CellularJitter.Enabled = cellular;

            // Gradient Perturb
            var gp = GradientPerturb.SelectedIndex > 0;
            GPAmplitude.Enabled = gp;
            GPFrequency.Enabled = gp;

            Generate(null, null);
        }

        private void Save(object sender, EventArgs e)
        {
            var save = new SaveFileDialog();
            save.Filters.Add(new FileFilter("PNG File", ".png"));
            var res = save.ShowDialog(null);
            if (res == DialogResult.Ok)
            {
                Bitmap.Save(save.FileName, ImageFormat.Png);
            }
        }

        // Helper
        private void AddToTableWithLabel(TableLayout parent, Control me, string label)
        {
            parent.Rows.Add(new TableRow
            {
                Cells = {
                    new TableCell(new Label { Text = label}),
                    new TableCell(me)
                }
            });
        }

        private string FormatReadable(string enumName) {
            return Regex.Replace(enumName, "(\\B[A-Z0-9])", " $1");
        }

        void HsvToRgb(byte h, byte s, byte v, out byte r, out byte g, out byte b)
        {
            byte region, remainder, p, q, t;

            if (s == 0)
            {
                r = v;
                g = v;
                b = v;
            }

            region = (byte)(h / 43);
            remainder = (byte)((h - (region * 43)) * 6);

            p = (byte)((v * (255 - s)) >> 8);
            q = (byte)((v * (255 - ((s * remainder) >> 8))) >> 8);
            t = (byte)((v * (255 - ((s * (255 - remainder)) >> 8))) >> 8);

            switch (region)
            {
                case 0:
                    r = v;
                    g = t;
                    b = p;
                    break;
                case 1:
                    r = q;
                    g = v;
                    b = p;
                    break;
                case 2:
                    r = p;
                    g = v;
                    b = t;
                    break;
                case 3:
                    r = p;
                    g = q;
                    b = v;
                    break;
                case 4:
                    r = t;
                    g = p;
                    b = v;
                    break;
                default:
                    r = v;
                    g = p;
                    b = q;
                    break;
            }
        }
    }
}
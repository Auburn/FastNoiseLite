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

        private NumericStepper PreviewWidth;
        private NumericStepper PreviewHeight;
        private CheckBox Invert;
        private CheckBox Is3D;
        private CheckBox VisualiseDomainWarp;

        private DropDown NoiseType;
        private NumericStepper Seed;
        private NumericStepper Frequency;

        private DropDown FractalType;
        private NumericStepper FractalOctaves;
        private NumericStepper FractalLacunarity;
        private NumericStepper FractalGain;

        private DropDown CellularDistanceFunction;
        private DropDown CellularReturnType;
        private NumericStepper CellularJitter;

        private DropDown DomainWarp;
        private DropDown DomainWarpFractal;
        private NumericStepper DomainWarpAmplitude;
        private NumericStepper DomainWarpFrequency;

        private Bitmap Bitmap;
        private ImageView Image;
        private int[] ImageData;

        private Label Time;
        private Label Min;
        private Label Max;
        private Label Mean;
        private Label ExtraInfo;

        private float zPos = 0;

        public MainForm()
        {
            // Sets the client (inner) size of the window for your content
            ClientSize = new Size(1050, 820);

            Title = "FastNoise Lite GUI";

            ImageData = new int[1];

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
                        var stack = new StackLayout();
                        stack.Orientation = Orientation.Horizontal;
                        stack.Spacing = 2;
                        PreviewWidth = new NumericStepper { Value = 768 };
                        PreviewWidth.ValueChanged += Generate;
                        stack.Items.Add(new StackLayoutItem(PreviewWidth));
                        PreviewHeight = new NumericStepper { Value = 768 };
                        PreviewHeight.ValueChanged += Generate;
                        stack.Items.Add(new StackLayoutItem(PreviewHeight));

                        AddToTableWithLabel(table, stack, "Preview Size:");
                    }

                    // Invert
                    Invert = new CheckBox();
                    Invert.CheckedChanged += Generate;
                    AddToTableWithLabel(table, Invert, "Invert:");

                    // 3D
                    Is3D = new CheckBox();
                    Is3D.CheckedChanged += OnUIUpdate;
                    AddToTableWithLabel(table, Is3D, "3D:");

                    // Visualise Domain Warp
                    VisualiseDomainWarp = new CheckBox();
                    VisualiseDomainWarp.CheckedChanged += OnUIUpdate;
                    AddToTableWithLabel(table, VisualiseDomainWarp, "Visualise Domain Warp:");
                }

                // TODO: Work out how to make this bold
                table.Rows.Add(new TableRow { Cells = { new Label { Text = "General:" } } });

                // General properties
                {
                    // Noise Type
                    {
                        NoiseType = new DropDown();
                        foreach (var name in Enum.GetNames(typeof(FastNoise.NoiseType)))
                        {
                            NoiseType.Items.Add(FormatReadable(name));
                        }
                        NoiseType.SelectedIndex = (int)FastNoise.NoiseType.Simplex;
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
                }

                // Add fractal label
                table.Rows.Add(new TableRow { Cells = { new Label { Text = "Fractal:" } } });

                {
                    // Fractal type
                    {
                        FractalType = new DropDown();
                        foreach (var name in Enum.GetNames(typeof(FastNoise.FractalType)))
                        {
                            if (name.StartsWith("DomainWarp")) break;
                            FractalType.Items.Add(FormatReadable(name));
                        }
                        FractalType.SelectedIndex = (int)FastNoise.FractalType.FBm;
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
                        foreach (var name in Enum.GetNames(typeof(FastNoise.CellularDistanceFunction)))
                        {
                            CellularDistanceFunction.Items.Add(FormatReadable(name));
                        }
                        CellularDistanceFunction.SelectedIndex = (int)FastNoise.CellularDistanceFunction.Euclidean;
                        CellularDistanceFunction.SelectedIndexChanged += Generate;
                        AddToTableWithLabel(table, CellularDistanceFunction, "Distance Function:");
                    }

                    // Return Type
                    {
                        CellularReturnType = new DropDown();
                        foreach (var name in Enum.GetNames(typeof(FastNoise.CellularReturnType)))
                        {
                            CellularReturnType.Items.Add(FormatReadable(name));
                        }
                        CellularReturnType.SelectedIndex = (int)FastNoise.CellularReturnType.CellValue;
                        CellularReturnType.SelectedIndexChanged += Generate;
                        AddToTableWithLabel(table, CellularReturnType, "Return Type:");
                    }

                    // Jitter
                    CellularJitter = new NumericStepper() { Value = 1.0, DecimalPlaces = 2 };
                    CellularJitter.ValueChanged += Generate;
                    AddToTableWithLabel(table, CellularJitter, "Jitter:");
                }

                // Add fractal label
                table.Rows.Add(new TableRow { Cells = { new Label { Text = "Domain Warp:" } } });

                {
                    // Domain Warp Dropdown
                    {
                        DomainWarp = new DropDown();
                        DomainWarp.Items.Add("None");
                        foreach (var name in Enum.GetNames(typeof(FastNoise.DomainWarpType)))
                        {
                            DomainWarp.Items.Add(FormatReadable(name));
                        }
                        DomainWarp.SelectedIndex = 0;
                        DomainWarp.SelectedIndexChanged += OnUIUpdate;
                        AddToTableWithLabel(table, DomainWarp, "Type:");
                    }

                    // Domain Warp Fractal Dropdown
                    {
                        DomainWarpFractal = new DropDown();
                        DomainWarpFractal.Items.Add("None");
                        foreach (var name in Enum.GetNames(typeof(FastNoise.FractalType)))
                        {
                            if (!name.StartsWith("DomainWarp")) continue;
                            DomainWarpFractal.Items.Add(FormatReadable(name), name);
                        }
                        DomainWarpFractal.SelectedIndex = 0;
                        DomainWarpFractal.SelectedIndexChanged += Generate;
                        AddToTableWithLabel(table, DomainWarpFractal, "Fractal Type:");
                    }

                    // Amplitude
                    DomainWarpAmplitude = new NumericStepper { Value = 30.0, DecimalPlaces = 2 };
                    DomainWarpAmplitude.ValueChanged += Generate;
                    AddToTableWithLabel(table, DomainWarpAmplitude, "Amplitude:");

                    // Frequency
                    DomainWarpFrequency = new NumericStepper { Value = 0.01, DecimalPlaces = 2 };
                    DomainWarpFrequency.ValueChanged += Generate;
                    AddToTableWithLabel(table, DomainWarpFrequency, "Frequency:");
                }

                // Add table to the controls panel
                controlPanel.Items.Add(table);

                // Add save and github buttons
                {
                    var stack = new StackLayout();
                    stack.Orientation = Orientation.Horizontal;
                    stack.Spacing = 5;

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
            var genNoise = new FastNoise();
            var warpNoise = new FastNoise();

            int w = (int)PreviewWidth.Value;
            int h = (int)PreviewHeight.Value;

            genNoise.SetNoiseType((FastNoise.NoiseType)NoiseType.SelectedIndex);
            genNoise.SetSeed((int)Seed.Value);
            genNoise.SetFrequency((float)Frequency.Value);
            genNoise.SetFractalType((FastNoise.FractalType)FractalType.SelectedIndex);
            genNoise.SetFractalOctaves((int)FractalOctaves.Value);
            genNoise.SetFractalLacunarity((float)FractalLacunarity.Value);
            genNoise.SetFractalGain((float)FractalGain.Value);
            genNoise.SetDomainWarpAmp((float)DomainWarpAmplitude.Value);

            genNoise.SetCellularDistanceFunction((FastNoise.CellularDistanceFunction)CellularDistanceFunction.SelectedIndex);
            genNoise.SetCellularReturnType((FastNoise.CellularReturnType)CellularReturnType.SelectedIndex);
            genNoise.SetCellularJitter((float)CellularJitter.Value);

            warpNoise.SetDomainWarpType((FastNoise.DomainWarpType)DomainWarp.SelectedIndex - 1);
            warpNoise.SetDomainWarpAmp((float)DomainWarpAmplitude.Value);
            warpNoise.SetFrequency((float)DomainWarpFrequency.Value);
            warpNoise.SetFractalType((FastNoise.FractalType)Enum.Parse(typeof(FastNoise.FractalType), DomainWarpFractal.SelectedKey));

            if (ImageData.Length != w * h)
            {
                ImageData = new int[w * h];
            }

            float noise;
            float minN = float.MaxValue;
            float maxN = float.MinValue;
            float avg = 0;

            bool get3d = Is3D.Checked == true; // Stupid!
            bool invert = Invert.Checked == true;

            float halfWidth = w / 2.0f;
            float halfHeight = h / 2.0f;

            // Timer
            Stopwatch sw = new Stopwatch();

            int index = 0;
            if (VisualiseDomainWarp.Checked != true)
            {
                var noiseValues = new float[w * h];
                bool warp = DomainWarp.SelectedIndex > 0;

                sw.Start();
                for (var y = 0; y < h; y++)
                {
                    for (var x = 0; x < w; x++)
                    {
                        float xf = (float)(x - halfWidth);
                        float yf = (float)(y - halfHeight);
                        float zf = zPos;


                        if (get3d)
                        {
                            if (warp)
                                warpNoise.DomainWarp(ref xf, ref yf, ref zf);

                            noise = genNoise.GetNoise(xf, yf, zf);
                        }
                        else
                        {
                            if (warp)
                                warpNoise.DomainWarp(ref xf, ref yf);

                            noise = genNoise.GetNoise(xf, yf);
                        }

                        avg += noise;
                        maxN = Math.Max(maxN, noise);
                        minN = Math.Min(minN, noise);
                        noiseValues[index++] = noise;
                    }
                }
                sw.Stop();

                avg /= index - 1;
                float scale = 255 / (maxN - minN);

                for (var i = 0; i < noiseValues.Length; i++)
                {
                    int value = (int)MathF.Round(Math.Max(0, Math.Min(255, (noiseValues[i] - minN) * scale)));

                    if (invert)
                        value = 255 - value;

                    ImageData[i] = value;
                    ImageData[i] |= value << 8;
                    ImageData[i] |= value << 16;
                    ImageData[i] |= 255 << 24;
                }
            }
            else
            {
                var noiseValues = new float[w * h * 3];

                sw.Start();
                for (var y = 0; y < h; y++)
                {
                    for (var x = 0; x < w; x++)
                    {
                        float xf = (float)x;
                        float yf = (float)y;
                        float zf = (float)zPos;
                        
                        if (get3d)
                            warpNoise.DomainWarp(ref xf, ref yf, ref zf);                        
                        else
                            warpNoise.DomainWarp(ref xf, ref yf);

                        xf -= x;
                        yf -= y;
                        zf -= zPos;

                        avg += xf + yf;
                        maxN = Math.Max(maxN, Math.Max(xf, yf));
                        minN = Math.Min(minN, Math.Min(xf, yf));

                        noiseValues[index++] = xf;
                        noiseValues[index++] = yf;

                        if (get3d)
                        {
                            avg += zf;
                            maxN = Math.Max(maxN, zf);
                            minN = Math.Min(minN, zf);
                            noiseValues[index++] = zf;
                        }
                    }
                }
                sw.Stop();

                if (get3d)
                    avg /= (index - 1) * 3;
                else avg /= (index - 1) * 2;

                index = 0;
                float scale = 255 / (maxN - minN);

                for (var i = 0; i < ImageData.Length; i++)
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
                        var vx = (noiseValues[index++] - minN) / (maxN - minN) - 0.5f;
                        var vy = (noiseValues[index++] - minN) / (maxN - minN) - 0.5f;

                        //byte H = (byte)(vx * scale);
                        //byte V = (byte)(vy * scale);

                        // https://discordapp.com/channels/703636892901441577/703638416335568926/735228970231922800
                        byte H = (byte)(MathF.Atan2(vy, vx) * (127.5 / Math.PI) + 127.5);
                        byte V = (byte)Math.Min(MathF.Sqrt(vx * vx + vy * vy) * 255 * 2, 255);

                        HsvToRgb(H, 255, V, out red, out green, out blue);
                    }

                    if (Invert.Checked == true)
                    {
                        red = (byte)(255 - red);
                        green = (byte)(255 - green);
                        blue = (byte)(255 - blue);
                    }

                    ImageData[i] = red;
                    ImageData[i] |= green << 8;
                    ImageData[i] |= blue << 16;
                    ImageData[i] |= 255 << 24;
                }
            }

            // Set image
            Bitmap = new Bitmap(w, h, PixelFormat.Format32bppRgba, ImageData);
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
                zPos += ((float)Frequency.Value) * 100.0f;
                Generate(null, null);
            }
            catch (Exception ex) { MessageBox.Show("Frequency error: " + ex.ToString()); }
        }

        private void OnDown(object sender, EventArgs e)
        {
            try
            {
                zPos -= ((float)Frequency.Value) * 100.0f;
                Generate(null, null);
            }
            catch (Exception ex) { MessageBox.Show("Frequency error: " + ex.ToString()); }
        }

        private void OnUIUpdate(object sender, EventArgs e)
        {
            // 3D contorls
            var is3d = Is3D.Checked == true;
            UpButton.Enabled = DownButton.Enabled = is3d;

            // Warp
            var perturbVis = NoiseType.SelectedKey.Contains("Perturb");
            DomainWarp.Enabled = !perturbVis;

            if (perturbVis)
            {
                if (is3d)
                    ExtraInfo.Text = "Visualisation of domain warp:\r\nRed = X offset, Green = Y offset, Blue = Z offset";
                else ExtraInfo.Text = "Visualisation of domain warp:\r\nHue = X offset, Brightness = Y offset";
            }
            else ExtraInfo.Text = "";

            // Fractal options
            var fractal = FractalType.SelectedIndex > 0;
            FractalOctaves.Enabled = fractal;
            FractalLacunarity.Enabled = fractal;
            FractalGain.Enabled = fractal;

            // Cellular
            var cellular = NoiseType.SelectedKey.Contains("Cellular");
            CellularDistanceFunction.Enabled = cellular;
            CellularReturnType.Enabled = cellular;
            CellularJitter.Enabled = cellular;

            // Gradient Perturb
            var warp = DomainWarp.SelectedIndex > 0;
            DomainWarpAmplitude.Enabled = warp;
            DomainWarpFrequency.Enabled = warp;

            // TODO: Reenable once lag is fixed
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

        private string FormatReadable(string enumName)
        {
            return Regex.Replace(enumName, "([a-z])([A-Z0-9])", "$1 $2");
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
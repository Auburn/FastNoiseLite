using System;
using System.Diagnostics;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using Eto.Drawing;
using Eto.Forms;

// Switch between using floats or doubles for input position (Match with FastNoiseLite.cs)
using FNfloat = System.Single;
//using FNfloat = System.Double;

class Program
{
    [STAThread]
    public static void Main(string[] args)
    {
        new Application().Run(new FastNoiseLiteGUI());
    }
}

public class FastNoiseLiteGUI : Form
{
    private Button GenerateButton;
    private Button UpButton;
    private Button DownButton;
    private Button ScrollButton;

    private NumericStepper PreviewWidth;
    private NumericStepper PreviewHeight;
    private CheckBox Invert;
    private CheckBox Is3D;
    private CheckBox VisualiseDomainWarp;

    private DropDown NoiseType;
    private DropDown RotationType3D;
    private NumericStepper Seed;
    private NumericStepper Frequency;

    private DropDown FractalType;
    private NumericStepper FractalOctaves;
    private NumericStepper FractalLacunarity;
    private NumericStepper FractalGain;
    private NumericStepper FractalWeightedStrength;
    private NumericStepper FractalPingPongStrength;

    private DropDown CellularDistanceFunction;
    private DropDown CellularReturnType;
    private NumericStepper CellularJitter;

    private DropDown DomainWarp;
    private DropDown DomainWarpRotationType3D;
    private NumericStepper DomainWarpAmplitude;
    private NumericStepper DomainWarpFrequency;

    private DropDown DomainWarpFractal;
    private NumericStepper DomainWarpFractalOctaves;
    private NumericStepper DomainWarpFractalLacunarity;
    private NumericStepper DomainWarpFractalGain;

    private Bitmap Bitmap;
    private ImageView Image;
    private int[] ImageData;

    private Label Time;
    private Label Min;
    private Label Max;
    private Label Mean;
    private Label ExtraInfo;

    private bool isScrolling = false;
    private float zPos = 0;
    private Size previewStartSize = new Size(768, 768);
    private Size windowSizeOffset = new Size(334, 52);

    public FastNoiseLiteGUI()
    {
        Title = "FastNoiseLite GUI";
        Resizable = false;

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
                    Spacing = 2
                };

                GenerateButton = new Button();
                GenerateButton.Text = "Generate";
                GenerateButton.Click += Generate;
                GenerateButton.Width = 90;
                layout.Items.Add(GenerateButton);

                UpButton = new Button();
                UpButton.Text = "Up";
                UpButton.Enabled = false;
                UpButton.Click += OnUp;
                UpButton.Width = 70;
                layout.Items.Add(UpButton);

                DownButton = new Button();
                DownButton.Text = "Down";
                DownButton.Enabled = false;
                DownButton.Click += OnDown;
                DownButton.Width = 70;
                layout.Items.Add(DownButton);

                ScrollButton = new Button();
                ScrollButton.Text = "Scroll";
                ScrollButton.Enabled = false;
                ScrollButton.Click += OnScroll;
                ScrollButton.Width = 70;
                layout.Items.Add(ScrollButton);

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
                    PreviewWidth = new NumericStepper { Value = previewStartSize.Width, Increment = 128 };
                    PreviewWidth.ValueChanged += Generate;
                    stack.Items.Add(new StackLayoutItem(PreviewWidth));
                    PreviewHeight = new NumericStepper { Value = previewStartSize.Height, Increment = 128 };
                    PreviewHeight.ValueChanged += Generate;
                    stack.Items.Add(new StackLayoutItem(PreviewHeight));

                    AddToTableWithLabel(table, stack, "Preview Size");
                }

                // Invert
                Invert = new CheckBox();
                Invert.CheckedChanged += Generate;
                AddToTableWithLabel(table, Invert, "Invert");

                // 3D
                Is3D = new CheckBox();
                Is3D.CheckedChanged += OnUIUpdate;
                AddToTableWithLabel(table, Is3D, "3D");

                // Visualise Domain Warp
                VisualiseDomainWarp = new CheckBox();
                VisualiseDomainWarp.CheckedChanged += OnUIUpdate;
                AddToTableWithLabel(table, VisualiseDomainWarp, "Visualise Domain Warp");
            }

            AddHeadingToTable(table, "General");

            // General properties
            {
                // Noise Type
                {
                    NoiseType = new DropDown();
                    foreach (var name in Enum.GetNames(typeof(FastNoiseLite.NoiseType)))
                    {
                        NoiseType.Items.Add(FormatReadable(name));
                    }
                    NoiseType.SelectedIndex = (int)FastNoiseLite.NoiseType.OpenSimplex2;
                    NoiseType.SelectedIndexChanged += OnUIUpdate;
                    AddToTableWithLabel(table, NoiseType, "Noise Type");
                }

                // Rotation Type 3D
                {
                    RotationType3D = new DropDown();
                    foreach (var name in Enum.GetNames(typeof(FastNoiseLite.RotationType3D)))
                    {
                        RotationType3D.Items.Add(FormatReadable(name));
                    }
                    RotationType3D.SelectedIndex = (int)FastNoiseLite.RotationType3D.None;
                    RotationType3D.SelectedIndexChanged += OnUIUpdate;
                    AddToTableWithLabel(table, RotationType3D, "Rotation Type 3D");
                }

                // Seed
                Seed = new NumericStepper { Value = 1337 };
                Seed.ValueChanged += Generate;
                AddToTableWithLabel(table, Seed, "Seed");

                // Frequency
                Frequency = new NumericStepper { Value = 0.02, DecimalPlaces = 3, Increment = 0.005 };
                Frequency.ValueChanged += Generate;
                AddToTableWithLabel(table, Frequency, "Frequency");
            }

            // Add fractal label
            AddHeadingToTable(table, "Fractal");

            {
                // Fractal type
                {
                    FractalType = new DropDown();
                    foreach (var name in Enum.GetNames(typeof(FastNoiseLite.FractalType)))
                    {
                        if (name.StartsWith("DomainWarp")) break;
                        FractalType.Items.Add(FormatReadable(name));
                    }
                    FractalType.SelectedIndex = (int)FastNoiseLite.FractalType.FBm;
                    FractalType.SelectedIndexChanged += OnUIUpdate;
                    AddToTableWithLabel(table, FractalType, "Type");
                }

                // Octaves
                FractalOctaves = new NumericStepper { Value = 5 };
                FractalOctaves.ValueChanged += Generate;
                AddToTableWithLabel(table, FractalOctaves, "Octaves");

                // Lacunarity
                FractalLacunarity = new NumericStepper { Value = 2.0, DecimalPlaces = 2, Increment = 0.1 };
                FractalLacunarity.ValueChanged += Generate;
                AddToTableWithLabel(table, FractalLacunarity, "Lacunarity");

                // Gain
                FractalGain = new NumericStepper { Value = 0.5, DecimalPlaces = 2, Increment = 0.1 };
                FractalGain.ValueChanged += Generate;
                AddToTableWithLabel(table, FractalGain, "Gain");

                // Weighted Strength
                FractalWeightedStrength = new NumericStepper { Value = 0.0, DecimalPlaces = 2, Increment = 0.1 };
                FractalWeightedStrength.ValueChanged += Generate;
                AddToTableWithLabel(table, FractalWeightedStrength, "Weighted Strength");

                // Ping Pong Strength
                FractalPingPongStrength = new NumericStepper { Value = 2.0, DecimalPlaces = 2, Increment = 0.1 };
                FractalPingPongStrength.ValueChanged += Generate;
                AddToTableWithLabel(table, FractalPingPongStrength, "Ping Pong Strength");
            }

            // Add fractal label
            AddHeadingToTable(table, "Cellular");

            {
                // Distance Function
                {
                    CellularDistanceFunction = new DropDown();
                    foreach (var name in Enum.GetNames(typeof(FastNoiseLite.CellularDistanceFunction)))
                    {
                        CellularDistanceFunction.Items.Add(FormatReadable(name));
                    }
                    CellularDistanceFunction.SelectedIndex = (int)FastNoiseLite.CellularDistanceFunction.EuclideanSq;
                    CellularDistanceFunction.SelectedIndexChanged += Generate;
                    AddToTableWithLabel(table, CellularDistanceFunction, "Distance Function");
                }

                // Return Type
                {
                    CellularReturnType = new DropDown();
                    foreach (var name in Enum.GetNames(typeof(FastNoiseLite.CellularReturnType)))
                    {
                        CellularReturnType.Items.Add(FormatReadable(name));
                    }
                    CellularReturnType.SelectedIndex = (int)FastNoiseLite.CellularReturnType.Distance;
                    CellularReturnType.SelectedIndexChanged += Generate;
                    AddToTableWithLabel(table, CellularReturnType, "Return Type");
                }

                // Jitter
                CellularJitter = new NumericStepper() { Value = 1.0, DecimalPlaces = 2, Increment = 0.1 };
                CellularJitter.ValueChanged += Generate;
                AddToTableWithLabel(table, CellularJitter, "Jitter");
            }

            // Add fractal label
            AddHeadingToTable(table, "Domain Warp");

            {
                // Domain Warp Dropdown
                {
                    DomainWarp = new DropDown();
                    DomainWarp.Items.Add("None");
                    foreach (var name in Enum.GetNames(typeof(FastNoiseLite.DomainWarpType)))
                    {
                        DomainWarp.Items.Add(FormatReadable(name));
                    }
                    DomainWarp.SelectedIndex = 0;
                    DomainWarp.SelectedIndexChanged += OnUIUpdate;
                    AddToTableWithLabel(table, DomainWarp, "Type");
                }

                // Domain Warp Rotation Type 3D
                {
                    DomainWarpRotationType3D = new DropDown();
                    foreach (var name in Enum.GetNames(typeof(FastNoiseLite.RotationType3D)))
                    {
                        DomainWarpRotationType3D.Items.Add(FormatReadable(name));
                    }
                    DomainWarpRotationType3D.SelectedIndex = (int)FastNoiseLite.RotationType3D.None;
                    DomainWarpRotationType3D.SelectedIndexChanged += OnUIUpdate;
                    AddToTableWithLabel(table, DomainWarpRotationType3D, "Rotation Type 3D");
                }

                // Amplitude
                DomainWarpAmplitude = new NumericStepper { Value = 30.0, DecimalPlaces = 2, Increment = 5 };
                DomainWarpAmplitude.ValueChanged += Generate;
                AddToTableWithLabel(table, DomainWarpAmplitude, "Amplitude");

                // Frequency
                DomainWarpFrequency = new NumericStepper { Value = 0.005, DecimalPlaces = 3, Increment = 0.005 };
                DomainWarpFrequency.ValueChanged += Generate;
                AddToTableWithLabel(table, DomainWarpFrequency, "Frequency");
            }

            // Add domain warp fractal label
            AddHeadingToTable(table, "Domain Warp Fractal");

            {
                // Domain Warp Fractal Dropdown
                {
                    DomainWarpFractal = new DropDown();
                    DomainWarpFractal.Items.Add("None");
                    foreach (var name in Enum.GetNames(typeof(FastNoiseLite.FractalType)))
                    {
                        if (!name.StartsWith("DomainWarp")) continue;
                        DomainWarpFractal.Items.Add(FormatReadable(name), name);
                    }
                    DomainWarpFractal.SelectedIndex = 0;
                    DomainWarpFractal.SelectedIndexChanged += OnUIUpdate;
                    AddToTableWithLabel(table, DomainWarpFractal, "Fractal Type");
                }

                // Octaves
                DomainWarpFractalOctaves = new NumericStepper { Value = 5 };
                DomainWarpFractalOctaves.ValueChanged += Generate;
                AddToTableWithLabel(table, DomainWarpFractalOctaves, "Octaves");

                // Lacunarity
                DomainWarpFractalLacunarity = new NumericStepper { Value = 2.0, DecimalPlaces = 2, Increment = 0.1 };
                DomainWarpFractalLacunarity.ValueChanged += Generate;
                AddToTableWithLabel(table, DomainWarpFractalLacunarity, "Lacunarity");

                // Gain
                DomainWarpFractalGain = new NumericStepper { Value = 0.5, DecimalPlaces = 2, Increment = 0.1 };
                DomainWarpFractalGain.ValueChanged += Generate;
                AddToTableWithLabel(table, DomainWarpFractalGain, "Gain");
            }

            // Add table to the controls panel
            Scrollable scrollPanel = new Scrollable();
            scrollPanel.Content = table;
            scrollPanel.Border = BorderType.None;
            scrollPanel.Padding = new Padding(0);          

            controlPanel.Items.Add(scrollPanel);

            // Add save and github buttons
            {
                var stack = new StackLayout();
                stack.Orientation = Orientation.Horizontal;
                stack.Spacing = 5;
                stack.Padding = new Padding(10);

                var save = new Button { Text = "Save" };
                save.Click += Save;
                stack.Items.Add(new StackLayoutItem(save));

                var github = new Button { Text = "GitHub" };
                github.Click += (sender, e) => { Process.Start(new ProcessStartInfo("https://github.com/Auburn/FastNoise") { UseShellExecute = true }); };
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
        OnUIUpdate();
    }

    async private void Generate(object sender = null, EventArgs e = null)
    {
        if (isScrolling && sender != ScrollButton) return;

        do
        {
            // Create noise generators
            var genNoise = new FastNoiseLite();
            var warpNoise = new FastNoiseLite();

            int w = (int)PreviewWidth.Value;
            int h = (int)PreviewHeight.Value;

            if (w <= 0 || h <= 0)
            {
                return;
            }

            genNoise.SetNoiseType((FastNoiseLite.NoiseType)NoiseType.SelectedIndex);
            genNoise.SetRotationType3D((FastNoiseLite.RotationType3D)RotationType3D.SelectedIndex);
            genNoise.SetSeed((int)Seed.Value);
            genNoise.SetFrequency((float)Frequency.Value);
            genNoise.SetFractalType((FastNoiseLite.FractalType)FractalType.SelectedIndex);
            genNoise.SetFractalOctaves((int)FractalOctaves.Value);
            genNoise.SetFractalLacunarity((float)FractalLacunarity.Value);
            genNoise.SetFractalGain((float)FractalGain.Value);
            genNoise.SetFractalWeightedStrength((float)FractalWeightedStrength.Value);
            genNoise.SetFractalPingPongStrength((float)FractalPingPongStrength.Value);

            genNoise.SetCellularDistanceFunction((FastNoiseLite.CellularDistanceFunction)CellularDistanceFunction.SelectedIndex);
            genNoise.SetCellularReturnType((FastNoiseLite.CellularReturnType)CellularReturnType.SelectedIndex);
            genNoise.SetCellularJitter((float)CellularJitter.Value);

            warpNoise.SetSeed((int)Seed.Value);
            warpNoise.SetDomainWarpType((FastNoiseLite.DomainWarpType)DomainWarp.SelectedIndex - 1);
            warpNoise.SetRotationType3D((FastNoiseLite.RotationType3D)DomainWarpRotationType3D.SelectedIndex);
            warpNoise.SetDomainWarpAmp((float)DomainWarpAmplitude.Value);
            warpNoise.SetFrequency((float)DomainWarpFrequency.Value);
            warpNoise.SetFractalType((FastNoiseLite.FractalType)Enum.Parse(typeof(FastNoiseLite.FractalType), DomainWarpFractal.SelectedKey));
            warpNoise.SetFractalOctaves((int)DomainWarpFractalOctaves.Value);
            warpNoise.SetFractalLacunarity((float)DomainWarpFractalLacunarity.Value);
            warpNoise.SetFractalGain((float)DomainWarpFractalGain.Value);

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

            // Timer
            Stopwatch sw = new Stopwatch();

            int index = 0;
            if (VisualiseDomainWarp.Checked != true)
            {
                var noiseValues = new float[w * h];
                bool warp = DomainWarp.SelectedIndex > 0;

                sw.Start();
                for (var y = h / -2; y < h / 2; y++)
                {
                    for (var x = w / -2; x < w / 2; x++)
                    {
                        FNfloat xf = x;
                        FNfloat yf = y;
                        FNfloat zf = zPos;


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
                    int value = (int)MathF.Round(Math.Clamp((noiseValues[i] - minN) * scale, 0, 255));

                    if (invert)
                        value = 255 - value;

                    ImageData[i] = value;
                    ImageData[i] |= value << 8;
                    ImageData[i] |= value << 16;
                }
            }
            else
            {
                var noiseValues = new float[w * h * 3];

                sw.Start();
                for (var y = -h / 2; y < h / 2; y++)
                {
                    for (var x = -w / 2; x < w / 2; x++)
                    {
                        FNfloat xf = x;
                        FNfloat yf = y;
                        FNfloat zf = zPos;

                        if (get3d)
                            warpNoise.DomainWarp(ref xf, ref yf, ref zf);
                        else
                            warpNoise.DomainWarp(ref xf, ref yf);

                        xf -= x;
                        yf -= y;
                        zf -= zPos;

                        avg += (float)(xf + yf);
                        maxN = Math.Max(maxN, (float)Math.Max(xf, yf));
                        minN = Math.Min(minN, (float)Math.Min(xf, yf));

                        noiseValues[index++] = (float)xf;
                        noiseValues[index++] = (float)yf;

                        if (get3d)
                        {
                            avg += (float)zf;
                            maxN = Math.Max(maxN, (float)zf);
                            minN = Math.Min(minN, (float)zf);
                            noiseValues[index++] = (float)zf;
                        }
                    }
                }
                sw.Stop();

                if (get3d)
                    avg /= (index - 1) * 3;
                else avg /= (index - 1) * 2;

                index = 0;
                float scale = 1 / (maxN - minN);

                for (var i = 0; i < ImageData.Length; i++)
                {
                    Color color = new Color();

                    if (get3d)
                    {
                        color.R = (noiseValues[index++] - minN) * scale;
                        color.G = (noiseValues[index++] - minN) * scale;
                        color.B = (noiseValues[index++] - minN) * scale;
                    }
                    else
                    {
                        var vx = (noiseValues[index++] - minN) / (maxN - minN) - 0.5f;
                        var vy = (noiseValues[index++] - minN) / (maxN - minN) - 0.5f;

                        ColorHSB hsb = new ColorHSB();

                        hsb.H = MathF.Atan2(vy, vx) * (180 / MathF.PI) + 180;
                        hsb.B = Math.Min(1.0f, MathF.Sqrt(vx * vx + vy * vy) * 2);
                        hsb.S = 0.9f;

                        color = hsb.ToColor();
                    }

                    if (Invert.Checked == true)
                    {
                        color.Invert();
                    }

                    ImageData[i] = color.ToArgb();
                }
            }

            // Set image
            Bitmap = new Bitmap(w, h, PixelFormat.Format32bppRgb, ImageData);
            Image.Image = Bitmap;

            // Set info labels
            Time.Text = "Time (ms): " + sw.ElapsedMilliseconds.ToString();
            Mean.Text = "Mean: " + avg.ToString();
            Min.Text = "Min: " + minN.ToString();
            Max.Text = "Max: " + maxN.ToString();

            // Sets the client (inner) size of the window for your content
            ClientSize = new Size(Math.Max((int)PreviewWidth.Value, 768), Math.Max((int)PreviewHeight.Value, 768)) + windowSizeOffset;

            if (isScrolling)
            {
                await Task.Delay(50);
                try
                {
                    zPos += ((float)Frequency.Value) * 100.0f;
                }
                catch (Exception ex) { MessageBox.Show("Frequency error: " + ex.ToString()); }
            }
        }
        while (isScrolling);
    }

    private void OnUp(object sender, EventArgs e)
    {
        try
        {
            zPos += ((float)Frequency.Value) * 100.0f;
            Generate(sender, e);
        }
        catch (Exception ex) { MessageBox.Show("Frequency error: " + ex.ToString()); }
    }

    private void OnDown(object sender, EventArgs e)
    {
        try
        {
            zPos -= ((float)Frequency.Value) * 100.0f;
            Generate(sender, e);
        }
        catch (Exception ex) { MessageBox.Show("Frequency error: " + ex.ToString()); }
    }

    private void OnScroll(object sender, EventArgs e)
    {
        isScrolling = !isScrolling;
        if (isScrolling) Generate(sender, e);
    }

    private void OnUIUpdate(object sender = null, EventArgs e = null)
    {
        // 3D controls
        var is3d = Is3D.Checked == true;
        UpButton.Enabled = DownButton.Enabled = ScrollButton.Enabled = is3d;

        // Warp
        var warpVis = VisualiseDomainWarp.Checked == true;
        NoiseType.Enabled = !warpVis;
        FractalType.Enabled = !warpVis;
        Frequency.Enabled = !warpVis;

        if (warpVis)
        {
            if (is3d)
                ExtraInfo.Text = "Visualisation of domain warp:\r\nRed = X offset, Green = Y offset, Blue = Z offset";
            else ExtraInfo.Text = "Visualisation of domain warp:\r\nHue = Angle, Brightness = Magnitude";
        }
        else ExtraInfo.Text = "";

        // Fractal options
        var fractalEnabled = (!warpVis && FractalType.SelectedIndex > 0);
        FractalOctaves.Enabled = fractalEnabled;
        FractalLacunarity.Enabled = fractalEnabled;
        FractalGain.Enabled = fractalEnabled;
        FractalWeightedStrength.Enabled = fractalEnabled;
        FractalPingPongStrength.Enabled = (fractalEnabled && FractalType.SelectedKey == "Ping Pong");

        // Domain Warp Fractal options
        var domainWarpFractalEnabled = (DomainWarp.SelectedIndex > 0 && DomainWarpFractal.SelectedIndex > 0);
        DomainWarpFractalOctaves.Enabled = domainWarpFractalEnabled;
        DomainWarpFractalLacunarity.Enabled = domainWarpFractalEnabled;
        DomainWarpFractalGain.Enabled = domainWarpFractalEnabled;

        // Cellular
        var cellular = NoiseType.SelectedKey.Contains("Cellular") && !warpVis;
        CellularDistanceFunction.Enabled = cellular;
        CellularReturnType.Enabled = cellular;
        CellularJitter.Enabled = cellular;

        // Domain Warp
        var warp = DomainWarp.SelectedIndex > 0;
        DomainWarpFractal.Enabled = warp;
        DomainWarpAmplitude.Enabled = warp;
        DomainWarpFrequency.Enabled = warp;

        // 3D Domain Rotation
        RotationType3D.Enabled = !warpVis && is3d;
        DomainWarpRotationType3D.Enabled = warp && is3d;

        Generate(sender, e);
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
        me.Width = 200;

        parent.Rows.Add(new TableRow
        {
            Cells = {
                TableLayout.AutoSized(new Label { Text = label, TextAlignment = TextAlignment.Right, VerticalAlignment = VerticalAlignment.Center, Width = 130 }),
                new TableCell(me)
            }
        });
    }
    private void AddHeadingToTable(TableLayout parent, string label)
    {
        Font bold = new Font(new Label().Font.Family, new Label().Font.Size, FontStyle.Bold);

        parent.Rows.Add(new TableRow { Cells = { 
            new Label { Text = label + "", Font = bold, Wrap = WrapMode.None, VerticalAlignment = VerticalAlignment.Center, Width = 130 } } });
    }

    private string FormatReadable(string enumName)
    {
        return Regex.Replace(Regex.Replace(enumName, "([a-z])([A-Z0-9])", "$1 $2"), "([a-z0-9])([A-Z][a-z])", "$1 $2");
    }    
}

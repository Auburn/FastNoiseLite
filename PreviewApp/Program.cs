using System;
using Eto.Forms;

namespace FastNoiseLite
{
    class Program
    {
        [STAThread]
        public static void Main(string[] args)
        {
            new Application().Run(new MainForm());
        }
    }
}

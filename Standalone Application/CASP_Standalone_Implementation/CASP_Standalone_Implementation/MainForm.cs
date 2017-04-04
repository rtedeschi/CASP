using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CASP_Standalone_Implementation
{
    public partial class MainForm : Form
    {
        public List<string> Modules = new List<string>() {
            "Analyze",
            "Lint",
            "Outline",
            "Print",
            "Translate"
        };

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {

        }
    }
}

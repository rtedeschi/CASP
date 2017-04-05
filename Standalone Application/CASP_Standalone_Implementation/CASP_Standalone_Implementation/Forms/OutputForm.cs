using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CASP_Standalone_Implementation.Forms
{
    public partial class OutputForm : Form
    {
        public OutputForm(string text)
        {
            InitializeComponent();

            OutputTextbox.Text = text;
        }
    }
}

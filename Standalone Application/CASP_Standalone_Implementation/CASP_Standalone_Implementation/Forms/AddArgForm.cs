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
    public partial class AddArgForm : Form
    {
        private Action<string, string> Callback;

        public AddArgForm(Action<string, string> callback)
        {
            Callback = callback;
            InitializeComponent();
        }

        private void AddButton_Click(object sender, EventArgs e)
        {
            Callback(ArgNameTextbox.Text, ArgValueTextbox.Text);
            Close();
            Dispose();
        }

        private void CancelButton_Click(object sender, EventArgs e)
        {
            Close();
            Dispose();
        }
    }
}

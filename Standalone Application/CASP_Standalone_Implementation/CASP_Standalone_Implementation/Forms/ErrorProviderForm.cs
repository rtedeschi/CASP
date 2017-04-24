using Newtonsoft.Json.Linq;
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
    public partial class ErrorProviderForm : Form
    {
        int numErrors;
        int numWarnings;

        public int NumErrors
        {
            get
            {
                return numErrors;
            }
        }
        public int NumWarnings
        {
            get
            {
                return numWarnings;
            }
        }

        public ErrorProviderForm(JObject CASP_Response)
        {
            InitializeComponent();
            ShowErrors(CASP_Response);
            ShowWarnings(CASP_Response);
        }

        void ShowErrors(JObject CASP_Response)
        {
            JArray errors = (JArray)CASP_Response["Errors"];
            ShowInList(errors, ErrorList);
            numErrors = errors.Count;
            ErrorsTab.Text = "Errors (" + numErrors + ")";
        }

        void ShowWarnings(JObject CASP_Response)
        {
            JArray warnings = (JArray)CASP_Response["Warnings"];
            ShowInList(warnings, WarningList);
            numWarnings = warnings.Count;
            WarningsTab.Text = "Warnings (" + numWarnings + ")";
        }

        void ShowInList(JArray list, ListBox control)
        {
            for (int i = 0; i < list.Count; i++)
                control.Items.Add((string)list[i]["message"]);
        }

    }
}

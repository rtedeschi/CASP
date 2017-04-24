using System.Drawing;
using System.Windows.Forms;
using CASP_Standalone_Implementation.Src;
using Newtonsoft.Json.Linq;
using System.Collections.Generic;
using System;
using System.Linq;
using System.Drawing.Drawing2D;

namespace CASP_Standalone_Implementation.Forms
{
    public partial class CASP_TranslateForm : CASP_OutputForm // Form
    {
        class Output
        {
            public KeyValuePair<string, string> Source { get; set; }
            public KeyValuePair<string, string> Target { get; set; }
        }

        public CASP_TranslateForm()
        {
            InitializeComponent();
        }

        public override void Set_CASP_Output(JObject CASP_Response)
        {
            Output output = ParseResponse(CASP_Response);

            SourceLanguageBox.Text = output.Source.Key;
            SourceDataBox.Text = output.Source.Value;
            TargetLanguageBox.Text = output.Target.Key;
            TargetDataBox.Text = output.Target.Value;

        }

        private Output ParseResponse(JObject CASP_Response)
        {
            JObject data = (JObject)CASP_Response["Data"];
            JObject source = (JObject)data["OriginalSource"];
            JObject target = (JObject)data["TranslatedSource"];

            string slanguage = "", sdata = "", tlanguage = "", tdata = "";

            if (source != null)
            {
                slanguage = source["Language"].ToString();
                sdata = source["Data"].ToString();
            }
            if (target != null)
            {
                tlanguage = target["Language"].ToString();
                tdata = target["Data"].ToString();
            }

            return new Output() {
                Source = new KeyValuePair<string, string>(slanguage, sdata),
                Target = new KeyValuePair<string, string>(tlanguage, tdata)
            };
        }

    }
}

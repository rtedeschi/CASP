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
    public partial class CASP_AnalyzeForm: CASP_OutputForm // Form
    {

        public CASP_AnalyzeForm()
        {
            InitializeComponent();
        }

        public override void Set_CASP_Output(JObject CASP_Response)
        {
            ParseResponse(CASP_Response);
        }

        private void ParseResponse(JObject CASP_Response)
        {
            JObject data = (JObject)CASP_Response["Data"];

            int y = fnlabel.Height + 5;

            foreach (KeyValuePair<string, JToken> prop in data)
            {
                JObject ob = (JObject)prop.Value;
                bool undefined = (bool)ob["IsUndefined"];
                string title = prop.Key;
                string analysis = (string)ob["Analysis"];

                Label Title = new Label();
                Title.AutoSize = false;
                Title.Font = fnlabel.Font;
                Title.Size = fnlabel.Size;
                Point TitleLocation = fnlabel.Location;
                TitleLocation.Y = y;
                Title.Location = TitleLocation;
                Title.TextAlign = ContentAlignment.MiddleCenter;
                Title.Text = title;

                Label Analysis = new Label();
                Analysis.AutoSize = false;
                Analysis.Font = fnlabel.Font;
                Analysis.Size = fnlabel.Size;
                Point AnalysisLocation = complexitylabel.Location;
                AnalysisLocation.Y = y;
                Analysis.Location = AnalysisLocation;
                Analysis.TextAlign = ContentAlignment.MiddleCenter;
                Analysis.Text = analysis;

                y += fnlabel.Height + 5;

                Parent.Controls.Add(Title);
                Parent.Controls.Add(Analysis);

                //AnalysisTable.Items.Add(title + ":\t" + analysis);
            }

        }

        private void AnalysisTable_Paint(object sender, PaintEventArgs e)
        {

        }
    }
}

using System;
using System.Data;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using CASP_Standalone_Implementation.Forms;
using CASP_Standalone_Implementation.Src;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace CASP_Standalone_Implementation
{
    public partial class MainForm : Form
    {
        public static string[] Languages 
        {
            get
            {
                string dir = ConsoleWrapper.CORE_DIR + "/cfg/";
                return Directory.GetFiles(dir)
                    .Where(s => s.EndsWith(".cfg"))
                    .Select(s => s.Replace(dir, "").Replace(".cfg", ""))
                    .ToArray();
            }
        }

        public static Dictionary<string, Type> Modules = new Dictionary<string, Type>() {
            { "Analyze", null },
            { "Lint", null },
            { "Outline", typeof(CASP_OutlineForm) },
            { "Print", null },
            { "Translate", null }
        };

        public static string TempFilename = "CASP_Temp_Src.tmp";

        public string request;

        private void UpdateRequest()
        {
            try {

                string module = ConsoleWrapper.GetArgument(ConsoleWrapper.ModuleId, ModuleCombo.SelectedItem.ToString());
                string srclang = ConsoleWrapper.GetArgument(ConsoleWrapper.SourceLanguage, InputLanguageCombo.SelectedItem.ToString());
                string code = ConsoleWrapper.GetArgument(ConsoleWrapper.CodeFile, TempFilename);
                //string code = ConsoleWrapper.GetArgument(ConsoleWrapper.CodeSnippet, InputTextbox.Text);

                request = ConsoleWrapper.GenerateRequest(module, srclang, code);
                RequestTextbox.Text = "CASP " + request;

            }
            catch (Exception e)
            {

            }
        }

        private async void Execute()
        {
            string filename = ConsoleWrapper.CORE_DIR + "/" + TempFilename;
            File.WriteAllText(filename, InputTextbox.Text);

            ExecuteButton.Enabled = false;
            ProgramStatus.Text = "Processing...";
            string output = await ConsoleWrapper.Execute(request);
            ProgramStatus.Text = "Ready";
            ExecuteButton.Enabled = true;

            if (ShowOutputCheckbox.Checked)
                new OutputForm(output).Show();

            Type T = Modules[ModuleCombo.SelectedItem.ToString()];
            if (T != null && T.IsSubclassOf(typeof(CASP_OutputForm)))
            {
                Regex reg = new Regex("CASP_RETURN_DATA_START(.*)CASP_RETURN_DATA_END", RegexOptions.Singleline);
                string jsonString = reg.Match(output).Groups[1].Value.Trim();
                JObject response = JsonConvert.DeserializeObject<JObject>(jsonString);

                CASP_OutputForm form = (CASP_OutputForm)Activator.CreateInstance(T);
                form.Show();
                form.Set_CASP_Output(response);
            }

            File.Delete(filename);
        }

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            ProgramStatus.Text = "Ready";

            ModuleCombo.Items.AddRange(Modules.Select(entry => entry.Key).ToArray());
            ModuleCombo.SelectedItem = ModuleCombo.Items[0];
            ModuleCombo.SelectedItem = "Outline"; // TODO Temp

            InputLanguageCombo.Items.AddRange(Languages);
            InputLanguageCombo.SelectedItem = InputLanguageCombo.Items[0];

            UpdateRequest();
        }

        private void SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateRequest();
        }

        private void ExecuteButton_Click(object sender, EventArgs e)
        {
            Execute();
        }

        private void InputTextbox_TextChanged(object sender, EventArgs e)
        {

        }
    }
}

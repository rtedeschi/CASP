using Newtonsoft.Json.Linq;
using System.Windows.Forms;

namespace CASP_Standalone_Implementation.Src
{
    public abstract class CASP_OutputForm : Form
    {
        public abstract void Set_CASP_Output(JObject CASP_Response);
    }
}

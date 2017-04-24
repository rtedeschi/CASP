using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CASP_Standalone_Implementation.Src
{
    class ConsoleWrapper
    {
        public static string CORE_DIR = Path.GetFullPath("../../../../../Core");

        public static string ModuleId = "moduleid";
        public static string CodeSnippet = "code";
        public static string CodeFile = "codef";
        public static string FunctionArgument = "args";
        public static string SourceLanguage = "sourcelang";

        public static bool Running
        {
            get
            {
                return ActiveProcess != null;
            }
        }

        static Process ActiveProcess = null;
        static long lastRunTime = 0;

        public static long LastRunTime
        {
            get
            {
                return lastRunTime;
            }
        }

        public static string GenerateRequest(params string[] data)
        {
            string ret = "";

            foreach (string arg in data)
            {
                ret += arg + " ";
            }

            return ret.Trim();
        }

        public static string GetArgument(string type, string data)
        {
            return "/" + type + "=\"" + data + "\"";
        }

        public static async Task<string> Execute(string request)
        {
            Stopwatch timer = new Stopwatch();
            return await Task.Run(() =>
            {
                string output = "";
                Kill();
                try {
                    ActiveProcess = new Process();
                    ActiveProcess.StartInfo.UseShellExecute = false;
                    ActiveProcess.StartInfo.RedirectStandardOutput = true;
                    ActiveProcess.StartInfo.CreateNoWindow = true;
                    ActiveProcess.StartInfo.WorkingDirectory = CORE_DIR;
                    ActiveProcess.StartInfo.FileName = CORE_DIR + "/CASP.exe";
                    ActiveProcess.StartInfo.Arguments = request;

                    timer.Start();

                    ActiveProcess.Start();
                    output = ActiveProcess.StandardOutput.ReadToEnd();
                    ActiveProcess.WaitForExit();
                }
                catch (Exception e)
                {
                    // If a process is forcefully killed in the middle of operation, it might throw an error
                }

                timer.Stop();
                lastRunTime = timer.ElapsedMilliseconds;

                Kill();

                return output;
            });
        }

        public static void Kill()
        {
            if (ActiveProcess != null)
            {
                if (!ActiveProcess.HasExited)
                    ActiveProcess.Kill();
                ActiveProcess.Dispose();
                ActiveProcess = null;
            }
        }
    }
}

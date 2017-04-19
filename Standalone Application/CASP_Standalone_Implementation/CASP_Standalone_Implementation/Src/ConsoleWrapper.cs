﻿using System;
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
                Process p = new Process();
                p.StartInfo.UseShellExecute = false;
                p.StartInfo.RedirectStandardOutput = true;
                p.StartInfo.CreateNoWindow = true;
                p.StartInfo.WorkingDirectory = CORE_DIR;
                p.StartInfo.FileName = CORE_DIR + "/CASP.exe";
                p.StartInfo.Arguments = request;

                timer.Start();

                p.Start();
                string output = p.StandardOutput.ReadToEnd();
                p.WaitForExit();

                timer.Stop();
                lastRunTime = timer.ElapsedMilliseconds;

                return output;
            });
        }
    }
}

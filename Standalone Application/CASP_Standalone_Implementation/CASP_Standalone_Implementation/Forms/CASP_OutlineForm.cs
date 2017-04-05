using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using CASP_Standalone_Implementation.Src;

namespace CASP_Standalone_Implementation.Forms
{
    public partial class CASP_OutlineForm : CASP_OutputForm // Form
    {
        public CASP_OutlineForm()
        {
            InitializeComponent();
        }

        public override void Set_CASP_Output(string data)
        {
            //OutputTextbox.Text = data;

            Label s = GetFlowEnd("Flowwww");
            
            FlowPanel.Controls.Add(s);
            s.Location = new Point(50, 50);

        }

        Label GetFlowEnd(string text)
        {
            Label flowblock = CreateFlowblock(text);
            flowblock.Paint += PaintFlowblockEnd;
            return flowblock;
        }

        Label GetFlowProcess(string text)
        {
            Label flowblock = CreateFlowblock(text);
            flowblock.Paint += PaintFlowblockProcess;
            return flowblock;
        }

        Label GetFlowDecision(string text)
        {
            Label flowblock = CreateFlowblock(text);
            flowblock.Paint += PaintFlowblockDecision;
            return flowblock;
        }

        Label GetFlowMethod(string text)
        {
            Label flowblock = CreateFlowblock(text);
            flowblock.Paint += PaintFlowblockMethod;
            return flowblock;
        }

        Label GetFlowLoop(string text)
        {
            Label flowblock = CreateFlowblock(text);
            flowblock.Paint += PaintFlowblockLoop;
            return flowblock;
        }

        private void PaintFlowblockEnd(object sender, PaintEventArgs e)
        {
            Pen pen = Pens.Black;
            int left, right, top, bottom, centerX, centerY;
            ReadFlowblockData(sender, out left, out right, out top, out bottom, out centerX, out centerY);

            Graphics g = e.Graphics;
            g.DrawArc(pen, new Rectangle(left - 5, top, 10, bottom - top), 90, 180);
            g.DrawLine(pen, left, top, right, top);
            g.DrawArc(pen, new Rectangle(right - 5, top, 10, bottom - top), -90, 180);
            g.DrawLine(pen, right, bottom, left, bottom);
        }

        private void PaintFlowblockProcess(object sender, PaintEventArgs e)
        {
            Pen pen = Pens.Black;
            int left, right, top, bottom, centerX, centerY;
            ReadFlowblockData(sender, out left, out right, out top, out bottom, out centerX, out centerY);

            Graphics g = e.Graphics;
            g.DrawLine(pen, left, top, right, top);
            g.DrawLine(pen, right, top, right, bottom);
            g.DrawLine(pen, right, bottom, left, bottom);
            g.DrawLine(pen, left, bottom, left, top);
        }

        private void PaintFlowblockDecision(object sender, PaintEventArgs e)
        {
            Pen pen = Pens.Black;
            int left, right, top, bottom, centerX, centerY;
            ReadFlowblockData(sender, out left, out right, out top, out bottom, out centerX, out centerY);

            Graphics g = e.Graphics;
            g.DrawLine(pen, centerX, top - 8, right + 8, centerY);
            g.DrawLine(pen, right + 8, centerY, centerX, bottom + 8);
            g.DrawLine(pen, centerX, bottom + 8, left - 8, centerY);
            g.DrawLine(pen, left - 8, centerY, centerX, top - 8);
        }

        private void PaintFlowblockMethod(object sender, PaintEventArgs e)
        {
            Pen pen = Pens.Black;
            int left, right, top, bottom, centerX, centerY;
            ReadFlowblockData(sender, out left, out right, out top, out bottom, out centerX, out centerY);

            Graphics g = e.Graphics;
            g.DrawLine(pen, left - 5, top, right + 5, top);
            g.DrawLine(pen, right + 5, top, right + 5, bottom);
            g.DrawLine(pen, right, top, right, bottom);
            g.DrawLine(pen, right + 5, bottom, left - 5, bottom);
            g.DrawLine(pen, left - 5, bottom, left - 5, top);
            g.DrawLine(pen, left, bottom, left, top);
        }

        private void PaintFlowblockLoop(object sender, PaintEventArgs e)
        {
            Pen pen = Pens.Black;
            int left, right, top, bottom, centerX, centerY;
            ReadFlowblockData(sender, out left, out right, out top, out bottom, out centerX, out centerY);

            Graphics g = e.Graphics;
            g.DrawLine(pen, left + 5, top, right - 5, top);
            g.DrawLine(pen, right - 5, top, right + 5, centerY);
            g.DrawLine(pen, right + 5, centerY, right - 5, bottom);
            g.DrawLine(pen, right - 5, bottom, left + 5, bottom);
            g.DrawLine(pen, left + 5, bottom, left - 5, centerY);
            g.DrawLine(pen, left - 5, centerY, left + 5, top);
        }

        private Label CreateFlowblock(string text)
        {
            Label flowblock = new Label();
            flowblock.Text = text;
            flowblock.AutoSize = true;

            FlowPanel.Controls.Add(flowblock);
            int width = flowblock.Width;
            int height = flowblock.Height;
            FlowPanel.Controls.Remove(flowblock);

            flowblock.TextAlign = ContentAlignment.MiddleCenter;
            flowblock.AutoSize = false;
            flowblock.Width = width + 40;
            flowblock.Height = height + 40;

            //flowblock.BorderStyle = BorderStyle.FixedSingle;

            return flowblock;
        }

        private void ReadFlowblockData(object sender, out int left, out int right, out int top, out int bottom, out int centerX, out int centerY)
        {
            Control flowblock = sender as Control;

            int width = flowblock.Width;
            int height = flowblock.Height;

            left = 10;
            right = width - 10 - 2;
            top = 10;
            bottom = height - 10 - 2;
            centerX = left + (right - left) / 2;
            centerY = top + (bottom - top) / 2;
        }
    }
}

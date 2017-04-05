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
    public partial class CASP_OutlineForm : CASP_OutputForm // Form
    {
        public CASP_OutlineForm()
        {
            InitializeComponent();
        }

        public override void Set_CASP_Output(JObject CASP_Response)
        {
            List<OutlineGraph> graphs = ParseResponse(CASP_Response);
            int x = 0;

            for (int i = 0; i < graphs.Count; i++)
            {
                Panel p = BreadthFirstDraw(graphs[i]);
                p.BorderStyle = BorderStyle.FixedSingle;
                p.Location = new Point(x, 0);
                FlowPanel.Controls.Add(p);

                x += p.Width + 20;
                graphs[i].Reset();
            }

        }

        private FlowBlock DrawNode(OutlineNode node, Panel panel, int x, int y, out Point newPoint)
        {
            node.drawn = true;
            FlowBlock block = GetFlowBlock(node);
            block.id = node.index;
            block.Location = new Point(x, y);
            panel.Controls.Add(block);
            newPoint = new Point(block.Right, block.Bottom);
            return block;
        }

        private Panel BreadthFirstDraw(OutlineGraph graph)
        {
            OutlineNode head = graph.nodes[0];

            Panel panel = new Panel();
            panel.AutoSize = true;

            int y = 20;
            List<FlowBlock> blocks = new List<FlowBlock>();
            Dictionary<int, FlowBlock> blockDictionary = new Dictionary<int, FlowBlock>();
            List<List<OutlineNode>> levels = new List<List<OutlineNode>>() { new List<OutlineNode>() { head } };
            for (int i = 0; i < levels.Count; i++)
            {
                List<OutlineNode> nodes = levels[i];
                int levelY = y;
                int x = 20;
                for (int j = 0; j < nodes.Count; j++)
                {
                    OutlineNode node = nodes[j];
                    List<OutlineNode> newNodes = node.edges
                        .Where(e => !e.target.drawn)
                        .Select(e => e.target)
                        .ToList();
                    Point newCoords;
                    FlowBlock block = DrawNode(node, panel, x, levelY, out newCoords);
                    blocks.Add(block);
                    blockDictionary.Add(node.index, block);
                    x += newCoords.X + 20;
                    if (newCoords.Y > y)
                        y = newCoords.Y;

                    if (levels.Count > i + 1)
                        levels[i + 1].AddRange(newNodes);
                    else
                        levels.Add(newNodes);
                }

                y += 20;
            }

            for (int i = 0; i < graph.edges.Count; i++)
            {
                OutlineEdge edge = graph.edges[i];
                FlowBlock source = blockDictionary[edge.source.index];
                FlowBlock target = blockDictionary[edge.target.index];

                if (!source.ConnectTo(target))
                {
                    // uh-oh... not enough space on the node. Should only happen on switch, which we don't have
                }
            }

            DrawEdges(blocks, panel);

            return panel;
        }

        private void DrawEdges(List<FlowBlock> blocks, Panel panel)
        {
            List<GraphicsPath> paths = new List<GraphicsPath>();
            for (int i = 0; i < blocks.Count; i++)
                paths.AddRange(blocks[i].GetEdgeGraphics());

            panel.Paint += (object sender, PaintEventArgs e) =>
            {
                for (int i = 0; i < paths.Count; i++)
                    e.Graphics.DrawPath(Pens.Black, paths[i]);
            };
        }

        FlowBlock GetFlowBlock(OutlineNode node)
        {
            FlowBlock block = null;
            switch (node.type)
            {
                case BlockType.Decision:
                    block = GetFlowDecision(node.text);
                    break;
                case BlockType.End:
                    block = GetFlowEnd(node.text);
                    break;
                case BlockType.EndDecision:
                    block = GetFlowProcess(node.text);
                    break;
                //case BlockType.IO:
                //    block = GetFlowDecision(node.text);
                //    break;
                case BlockType.Loop:
                    block = GetFlowLoop(node.text);
                    break;
                case BlockType.MethodCall:
                    block = GetFlowMethod(node.text);
                    break;
                case BlockType.Process:
                    block = GetFlowProcess(node.text);
                    break;
                case BlockType.Start:
                    block = GetFlowEnd(node.text);
                    break;
                default:
                    block = new FlowBlock();
                    break;
            }
            block.UpdateSockets();

            return block;
        }

        private List<OutlineGraph> ParseResponse(JObject CASP_Response)
        {
            JObject data = (JObject)CASP_Response["Data"];
            JArray outlines = (JArray)data["Outlines"];

            List<OutlineGraph> graphs = new List<OutlineGraph>();

            for (int i = 0; i < outlines.Count; i++)
            {
                List<dynamic> edgeList = new List<dynamic>();
                JArray o = (JArray)outlines[i];

                OutlineGraph graph = new OutlineGraph();

                for (int j = 0; j < o.Count; j++)
                {
                    JObject node = (JObject)o[j];
                    string nodeText = (string)node["data"];
                    BlockType nodeType = (BlockType)Enum.Parse(typeof(BlockType), (string)node["type"]);
                    JArray edges = (JArray)node["edges"];

                    graph.AddNode(new OutlineNode() { text = nodeText, type = nodeType });

                    for (int k = 0; k < edges.Count; k++)
                    {
                        JObject edge = (JObject)edges[k];
                        int source = (int)edge["source"];
                        int target = (int)edge["target"];
                        string edgeText = (string)edge["data"];

                        edgeList.Add(new { source = source, target = target, text = edgeText });
                    }
                }

                for (int j = 0; j < edgeList.Count; j++)
                {
                    graph.AddEdge(edgeList[j].source, edgeList[j].target, edgeList[j].text);
                }

                graphs.Add(graph);
            }

            return graphs;
        }

        FlowBlock GetFlowEnd(string text)
        {
            FlowBlock flowblock = CreateFlowblock(text);
            flowblock.Paint += PaintFlowblockEnd;
            return flowblock;
        }

        FlowBlock GetFlowProcess(string text)
        {
            FlowBlock flowblock = CreateFlowblock(text);
            flowblock.Paint += PaintFlowblockProcess;
            return flowblock;
        }

        FlowBlock GetFlowDecision(string text)
        {
            FlowBlock flowblock = CreateFlowblock(text);
            flowblock.Paint += PaintFlowblockDecision;
            return flowblock;
        }

        FlowBlock GetFlowMethod(string text)
        {
            FlowBlock flowblock = CreateFlowblock(text);
            flowblock.Paint += PaintFlowblockMethod;
            return flowblock;
        }

        FlowBlock GetFlowLoop(string text)
        {
            FlowBlock flowblock = CreateFlowblock(text);
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

        private FlowBlock CreateFlowblock(string text)
        {
            FlowBlock flowblock = new FlowBlock();
            flowblock.Text = text;
            flowblock.AutoSize = true;

            FlowPanel.Controls.Add(flowblock);
            int width = flowblock.Width;
            int height = flowblock.Height;
            FlowPanel.Controls.Remove(flowblock);

            flowblock.BackColor = Color.Transparent;
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

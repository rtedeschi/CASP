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
        Pen blockPen = Pens.Gray;
        
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
                //p.BorderStyle = BorderStyle.FixedSingle;
                p.Location = new Point(x, 0);
                FlowPanel.Controls.Add(p);

                x += p.Width + 20;
                graphs[i].Reset();
            }

        }

        private FlowBlock DrawNode(OutlineNode node, FlowBlock parent, Panel panel, int minX, int y, out Point newPoint)
        {

            node.drawn = true;
            FlowBlock block = GetFlowBlock(node);
            panel.Controls.Add(block);
            int preferredX;

            if (parent != null)
            {
                parent.children.Add(block);
                block.parent = parent;
                preferredX = parent.Center.X - block.Width / 2;
            }
            else
            {
                preferredX = minX;
            }

            block.Location = new Point(Math.Max(minX, preferredX), y);

            newPoint = new Point(block.Right, block.Bottom);
            return block;
        }

        private class node
        {
            public FlowBlock parentFlow;
            public List<OutlineNode> children;
        }

        private Panel BreadthFirstDraw(OutlineGraph graph)
        {
            OutlineNode head = graph.nodes[0];

            Panel panel = new Panel();
            panel.AutoSize = true;
            int yBuff = 30;
            int xBuff = 30;

            int y = yBuff;
            List<FlowBlock> blocks = new List<FlowBlock>();
            Dictionary<int, FlowBlock> blockDictionary = new Dictionary<int, FlowBlock>();
            List<List<node>> levels = new List<List<node>>() { new List<node>() { new node() { parentFlow = null, children = new List<OutlineNode>() { head } } } };
            for (int i = 0; i < levels.Count; i++)
            {
                
                List<node> nodes = levels[i];
                int levelY = y;
                int minX = xBuff;
                for (int k = 0; k < nodes.Count; k++)
                {
                    List<OutlineNode> n = nodes[k].children;
                    FlowBlock parent = nodes[k].parentFlow;

                    for (int j = 0; j < n.Count; j++)
                    {
                        OutlineNode node = n[j];
                        Point newCoords;

                        FlowBlock block = DrawNode(node, parent, panel, minX, levelY, out newCoords);

                        // TODO need to work on decisions
                        if (blockDictionary.ContainsKey(node.index))
                        {
                            FlowBlock old = blockDictionary[node.index];
                            blockDictionary.Remove(node.index);
                            blocks.Remove(old);
                            panel.Controls.Remove(old);
                        }
                        else
                        {
                            node newNode = new node
                            {
                                parentFlow = block,
                                children = node.edges
                                .Where(e => !e.target.drawn)
                                .Select(e => e.target)
                                .ToList()
                            };

                            if (levels.Count > i + 1)
                                levels[i + 1].Add(newNode);
                            else
                                levels.Add(new List<node>() { newNode });
                        }

                        blockDictionary.Add(node.index, block);
                        blocks.Add(block);

                        minX = block.Right + xBuff;
                        if (newCoords.Y > y)
                            y = newCoords.Y;

                    }
                }

                y += yBuff;
            }

            for (int i = 0; i < graph.edges.Count; i++)
            {
                OutlineEdge edge = graph.edges[i];
                FlowBlock source = blockDictionary[edge.source.index];
                FlowBlock target = blockDictionary[edge.target.index];

                if (!source.ConnectTo(target, edge.text))
                {
                    // uh-oh... not enough space on the node. Should only happen on switch, which we don't have
                }
            }

            RenderEdges(blocks, panel);

            return panel;
        }

        private void RenderEdges(List<FlowBlock> blocks, Panel panel)
        {
            panel.Paint += (object sender, PaintEventArgs e) =>
            {
                for (int i = 0; i < blocks.Count; i++)
                    blocks[i].RenderEdgeGraphics(e.Graphics);
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
                    block = GetFlowSink(node.text);
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
            block.id = node.index;
            block.type = node.type;

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

        FlowBlock GetFlowSink(string text)
        {
            FlowBlock flowblock = CreateFlowblock("");
            flowblock.Width = flowblock.Height = 53;
            flowblock.Paint += PaintFlowblockSink;
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
            int left, right, top, bottom, centerX, centerY;
            ReadFlowblockData(sender, out left, out right, out top, out bottom, out centerX, out centerY);

            Graphics g = e.Graphics;
            g.DrawArc(blockPen, new Rectangle(left - 5, top, 10, bottom - top), 90, 180);
            g.DrawLine(blockPen, left, top, right, top);
            g.DrawArc(blockPen, new Rectangle(right - 5, top, 10, bottom - top), -90, 180);
            g.DrawLine(blockPen, right, bottom, left, bottom);
        }

        private void PaintFlowblockProcess(object sender, PaintEventArgs e)
        {
            int left, right, top, bottom, centerX, centerY;
            ReadFlowblockData(sender, out left, out right, out top, out bottom, out centerX, out centerY);

            Graphics g = e.Graphics;
            g.DrawLine(blockPen, left, top, right, top);
            g.DrawLine(blockPen, right, top, right, bottom);
            g.DrawLine(blockPen, right, bottom, left, bottom);
            g.DrawLine(blockPen, left, bottom, left, top);
        }

        private void PaintFlowblockDecision(object sender, PaintEventArgs e)
        {
            int left, right, top, bottom, centerX, centerY;
            ReadFlowblockData(sender, out left, out right, out top, out bottom, out centerX, out centerY);

            Graphics g = e.Graphics;
            g.DrawLine(blockPen, centerX, top - 8, right + 8, centerY);
            g.DrawLine(blockPen, right + 8, centerY, centerX, bottom + 8);
            g.DrawLine(blockPen, centerX, bottom + 8, left - 8, centerY);
            g.DrawLine(blockPen, left - 8, centerY, centerX, top - 8);
        }

        private void PaintFlowblockMethod(object sender, PaintEventArgs e)
        {
            int left, right, top, bottom, centerX, centerY;
            ReadFlowblockData(sender, out left, out right, out top, out bottom, out centerX, out centerY);

            Graphics g = e.Graphics;
            g.DrawLine(blockPen, left - 5, top, right + 5, top);
            g.DrawLine(blockPen, right + 5, top, right + 5, bottom);
            g.DrawLine(blockPen, right, top, right, bottom);
            g.DrawLine(blockPen, right + 5, bottom, left - 5, bottom);
            g.DrawLine(blockPen, left - 5, bottom, left - 5, top);
            g.DrawLine(blockPen, left, bottom, left, top);
        }

        private void PaintFlowblockLoop(object sender, PaintEventArgs e)
        {
            int left, right, top, bottom, centerX, centerY;
            ReadFlowblockData(sender, out left, out right, out top, out bottom, out centerX, out centerY);

            Graphics g = e.Graphics;
            g.DrawLine(blockPen, left + 5, top, right - 5, top);
            g.DrawLine(blockPen, right - 5, top, right + 5, centerY);
            g.DrawLine(blockPen, right + 5, centerY, right - 5, bottom);
            g.DrawLine(blockPen, right - 5, bottom, left + 5, bottom);
            g.DrawLine(blockPen, left + 5, bottom, left - 5, centerY);
            g.DrawLine(blockPen, left - 5, centerY, left + 5, top);
        }

        private void PaintFlowblockSink(object sender, PaintEventArgs e)
        {
            int left, right, top, bottom, centerX, centerY;
            ReadFlowblockData(sender, out left, out right, out top, out bottom, out centerX, out centerY);

            Graphics g = e.Graphics;
            g.DrawEllipse(blockPen, new Rectangle(left, top, right - left, bottom - top));
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

            //flowblock.BorderStyle = BorderStyle.Fixed3D;

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

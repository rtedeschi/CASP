using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CASP_Standalone_Implementation.Src
{
    public enum BlockType { Start, MethodCall, Process, Loop, Decision, EndDecision, IO, End };

    public class OutlineGraph
    {
        public List<OutlineNode> nodes = new List<OutlineNode>();
        public List<OutlineEdge> edges = new List<OutlineEdge>();

        public OutlineNode AddNode(OutlineNode node)
        {
            node.index = nodes.Count;
            nodes.Add(node);
            return node;
        }

        public OutlineEdge AddEdge(int sourceIndex, int targetIndex, string text = "")
        {
            OutlineEdge edge = nodes[sourceIndex].AddEdge(nodes[targetIndex], text);
            edges.Add(edge);
            return edge;
        }

        public void Reset()
        {
            for (int i = 0; i < nodes.Count; i++)
            {
                nodes[i].drawn = false;
            }
        }
    }

    public class OutlineNode
    {
        public bool drawn = false;
        public int index;
        public string text;
        public BlockType type;
        public List<OutlineEdge> edges = new List<OutlineEdge>();

        public OutlineEdge AddEdge(OutlineNode target, string text = "")
        {
            OutlineEdge edge = new OutlineEdge() { source = this, target = target, text = text };
            edges.Add(edge);
            return edge;
        }
    }

    public class OutlineEdge
    {
        public string text;
        public OutlineNode source;
        public OutlineNode target;
    }

    public class FlowBlock : Label
    {
        Socket TopSocket;
        Socket BottomSocket;
        Socket LeftSocket;
        Socket RightSocket;
        bool renderingSockets = false;

        public int id;

        public Point Center
        {
            get
            {
                return new Point(Width / 2 + Location.X, Height / 2 + Location.Y);
            }
        }

        
        public bool RenderSockets
        {
            get
            {
                return renderingSockets;
            }
            set
            {
                if (value != renderingSockets)
                {
                    if (value)
                        Paint += FlowBlock_Paint;
                    else
                        Paint -= FlowBlock_Paint;
                }
                renderingSockets = value;
            }
        }

        public FlowBlock() : base() {
            TopSocket = new Socket(this);
            BottomSocket = new Socket(this);
            LeftSocket = new Socket(this);
            RightSocket = new Socket(this);
            UpdateSockets();
            //RenderSockets = true;
        }

        public void UpdateSockets()
        {
            int halfWidth = Width / 2;
            int halfHeight = Height / 2;
            int Top = 0;
            int Bottom = Height;
            int Left = 0;
            int Right = Width;

            TopSocket.SetLocation(halfWidth, Top);
            BottomSocket.SetLocation(halfWidth, Bottom);
            LeftSocket.SetLocation(Left, halfHeight);
            RightSocket.SetLocation(Right, halfHeight);
        }

        public Socket ClosestSocketToPoint(Point pt)
        {
            Socket closest = TopSocket;
            double min = TopSocket.DistanceToPoint(pt);
            double next = BottomSocket.DistanceToPoint(pt);
            if (next < min)
            {
                min = next;
                closest = BottomSocket;
            }
            next = LeftSocket.DistanceToPoint(pt);
            if (next < min)
            {
                min = next;
                closest = LeftSocket;
            }
            next = RightSocket.DistanceToPoint(pt);
            if (next < min)
            {
                min = next;
                closest = RightSocket;
            }
            return closest;            
        }

        public Socket ClosestUnusedSocketToPoint(Point pt)
        {
            Socket closest = null;
            double min = double.PositiveInfinity;
            double next = 0;

            if (TopSocket.Unused)
            {
                next = TopSocket.DistanceToPoint(pt);
                if (next < min)
                {
                    min = next;
                    closest = TopSocket;
                }
            }
            if (BottomSocket.Unused)
            {
                next = BottomSocket.DistanceToPoint(pt);
                if (next < min)
                {
                    min = next;
                    closest = BottomSocket;
                }
            }
            if (LeftSocket.Unused) {
                next = LeftSocket.DistanceToPoint(pt);
                if (next < min)
                {
                    min = next;
                    closest = LeftSocket;
                }
            }
            if (RightSocket.Unused) {
                next = RightSocket.DistanceToPoint(pt);
                if (next < min)
                {
                    min = next;
                    closest = RightSocket;
                }
            }
            return closest;
        }

        void ClosestSocketPair(FlowBlock block, out Socket sourceSocket, out Socket targetSocket)
        {
            Socket source = null;
            Socket target = null;

            double min = double.PositiveInfinity;
            double next;

            if (TopSocket.Unused)
            {
                Socket temp = block.ClosestUnusedSocketToPoint(TopSocket.Location);
                if (temp != null)
                {
                    next = TopSocket.DistanceToPoint(temp.Location);
                    if (next < min)
                    {
                        min = next;
                        source = TopSocket;
                        target = temp;
                    }
                }
            }

            if (BottomSocket.Unused)
            {
                Socket temp = block.ClosestUnusedSocketToPoint(BottomSocket.Location);
                if (temp != null)
                {
                    next = BottomSocket.DistanceToPoint(temp.Location);
                    if (next < min)
                    {
                        min = next;
                        source = BottomSocket;
                        target = temp;
                    }
                }
            }

            if (LeftSocket.Unused)
            {
                Socket temp = block.ClosestUnusedSocketToPoint(LeftSocket.Location);
                if (temp != null)
                {
                    next = LeftSocket.DistanceToPoint(temp.Location);
                    if (next < min)
                    {
                        min = next;
                        source = LeftSocket;
                        target = temp;
                    }
                }
            }

            if (RightSocket.Unused)
            {
                Socket temp = block.ClosestUnusedSocketToPoint(RightSocket.Location);
                if (temp != null)
                {
                    next = RightSocket.DistanceToPoint(temp.Location);
                    if (next < min)
                    {
                        min = next;
                        source = RightSocket;
                        target = temp;
                    }
                }
            }

            sourceSocket = source;
            targetSocket = target;
        }

        public bool ConnectTo(FlowBlock block)
        {
            Socket sourceSocket;
            Socket targetSocket;
            ClosestSocketPair(block, out sourceSocket, out targetSocket);

            if (sourceSocket != null && targetSocket != null)
            {
                Connector c = new Connector()
                {
                    data = ""
                };
                sourceSocket.ConnectAsSource(c);
                targetSocket.ConnectAsTarget(c);
                return true;
            }
            return false;
        }

        private void FlowBlock_Paint(object sender, PaintEventArgs e)
        {
            int size = 10;
            int hs = size / 2;

            if (TopSocket.Unused)
                e.Graphics.DrawEllipse(Pens.Black, new Rectangle(TopSocket.LocalLocation.X - hs, TopSocket.LocalLocation.Y - hs, size, size));
            else
                e.Graphics.FillEllipse(Brushes.Black, new Rectangle(TopSocket.LocalLocation.X - hs, TopSocket.LocalLocation.Y - hs, size, size));

            if (BottomSocket.Unused)
                e.Graphics.DrawEllipse(Pens.Black, new Rectangle(BottomSocket.LocalLocation.X - hs, BottomSocket.LocalLocation.Y - hs, size, size));
            else
                e.Graphics.FillEllipse(Brushes.Black, new Rectangle(BottomSocket.LocalLocation.X - hs, BottomSocket.LocalLocation.Y - hs, size, size));

            if (LeftSocket.Unused)
                e.Graphics.DrawEllipse(Pens.Black, new Rectangle(LeftSocket.LocalLocation.X - hs, LeftSocket.LocalLocation.Y - hs, size, size));
            else
                e.Graphics.FillEllipse(Brushes.Black, new Rectangle(LeftSocket.LocalLocation.X - hs, LeftSocket.LocalLocation.Y - hs, size, size));

            if (RightSocket.Unused)
                e.Graphics.DrawEllipse(Pens.Black, new Rectangle(RightSocket.LocalLocation.X - hs, RightSocket.LocalLocation.Y - hs, size, size));
            else
                e.Graphics.FillEllipse(Brushes.Black, new Rectangle(RightSocket.LocalLocation.X - hs, RightSocket.LocalLocation.Y - hs, size, size));

        }

        public List<GraphicsPath> GetEdgeGraphics()
        {
            List<GraphicsPath> paths = new List<GraphicsPath>();

            if (!TopSocket.Unused && TopSocket.isSource)
                paths.Add(TopSocket.Connector.GetGraphicsPath());

            if (!BottomSocket.Unused && BottomSocket.isSource)
                paths.Add(BottomSocket.Connector.GetGraphicsPath());

            if (!LeftSocket.Unused && LeftSocket.isSource)
                paths.Add(LeftSocket.Connector.GetGraphicsPath());

            if (!RightSocket.Unused && RightSocket.isSource)
                paths.Add(RightSocket.Connector.GetGraphicsPath());

            return paths;
        }
    }

    public class Socket
    {
        public FlowBlock FlowBlock;
        public Point LocalLocation;
        public Connector Connector = null;

        public bool isSource
        {
            get
            {
                if (Connector != null)
                    return Connector.Source == this;
                return false;
            }
        }

        public bool isTarget
        {
            get
            {
                if (Connector != null)
                    return Connector.Target == this;
                return false;
            }
        }

        public Point Location
        {
            get
            {
                Point pt = new Point(0, 0);
                pt.Offset(FlowBlock.Location);
                pt.Offset(LocalLocation);
                return pt;
            }
        }

        public bool Unused
        {
            get
            {
                return Connector == null;
            }
        }

        public Socket(FlowBlock parent, int localX = 0, int localY = 0)
        {
            FlowBlock = parent;
            LocalLocation = new Point(localX, localY);
        }

        public void SetLocation(int localX, int localY)
        {
            LocalLocation = new Point(localX, localY);
        }

        public double DistanceToPoint(Point pt)
        {
            return Math.Sqrt(Math.Pow(pt.X - Location.X, 2) + Math.Pow(pt.Y - Location.Y, 2));
        }

        public bool ConnectAsSource(Connector connector)
        {
            if (Connector == null)
            {
                if (connector.Source != null)
                    connector.Source.Connector = null;
                connector.Source = this;
                Connector = connector;
                return true;
            }
            return false;
        }

        public bool ConnectAsTarget(Connector connector)
        {
            if (Connector == null)
            {
                if (connector.Target != null)
                    connector.Target.Connector = null;
                connector.Target = this;
                Connector = connector;
                return true;
            }
            return false;
        }

    }

    public class Connector
    {
        public Socket Source;
        public Socket Target;

        public string data;

        public GraphicsPath GetGraphicsPath()
        {
            GraphicsPath path = new GraphicsPath();

            if (Source != null && Target != null)
            {
                int arrowLen = 10;
                Point source = Source.Location;
                Point target = Target.Location;

                double initialAngleDeg = 360 - Math.Atan2(target.Y - source.Y, target.X - source.X) * 180 / Math.PI;
                double angleLRad = (initialAngleDeg - 180 + 45 / 2) * Math.PI / 180;
                double angleRRad = (initialAngleDeg - 180 - 45 / 2) * Math.PI / 180;

                Point arrowL = new Point(target.X + (int)(arrowLen * Math.Cos(angleLRad)), target.Y - (int)(arrowLen * Math.Sin(angleLRad)));
                Point arrowR = new Point(target.X + (int)(arrowLen * Math.Cos(angleRRad)), target.Y - (int)(arrowLen * Math.Sin(angleRRad)));

                path.AddLine(source, target);
                path.AddLine(target, arrowL);
                path.AddLine(target, arrowR);
                //path.AddPolygon(new Point[] { target, arrowL, arrowR });
            }

            return path;
        }
    }
}

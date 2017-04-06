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

        public bool MouseOver = false;
        bool renderingSockets = false;
        public BlockType type;
        public FlowBlock parent = null;
        public List<FlowBlock> children = new List<FlowBlock>();

        public List<FlowBlock> siblings
        {
            get
            {
                List<FlowBlock> sibs = new List<FlowBlock>();
                if (parent != null)
                {
                    for (int i = 0; i < parent.children.Count; i++)
                    {
                        if (parent.children[i] != this)
                            sibs.Add(parent.children[i]);
                    }
                }
                return sibs;
            }
        }

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
            TopSocket = new Socket(this, 90);
            BottomSocket = new Socket(this, 270);
            LeftSocket = new Socket(this, 180);
            RightSocket = new Socket(this, 0);
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
                Socket temp = block.type != BlockType.EndDecision ?
                    block.ClosestUnusedSocketToPoint(TopSocket.Location) :
                    block.ClosestSocketToPoint(TopSocket.Location);
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
                Socket temp = block.type != BlockType.EndDecision ?
                    block.ClosestUnusedSocketToPoint(BottomSocket.Location) :
                    block.ClosestSocketToPoint(BottomSocket.Location);
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
                Socket temp = block.type != BlockType.EndDecision ?
                    block.ClosestUnusedSocketToPoint(LeftSocket.Location) :
                    block.ClosestSocketToPoint(LeftSocket.Location);
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
                Socket temp = block.type != BlockType.EndDecision ? 
                    block.ClosestUnusedSocketToPoint(RightSocket.Location) :
                    block.ClosestSocketToPoint(RightSocket.Location);
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

        public bool ConnectTo(FlowBlock block, string data = "")
        {
            Socket sourceSocket;
            Socket targetSocket;
            ClosestSocketPair(block, out sourceSocket, out targetSocket);

            if (sourceSocket != null && targetSocket != null)
            {
                Connector c = new Connector()
                {
                    data = data
                };
                sourceSocket.ConnectAsSource(c);
                targetSocket.ConnectAsTarget(c);
                return true;
            }
            return false;
        }

        private void FlowBlock_Paint(object sender, PaintEventArgs e)
        {
            Pen pen = Pens.Black;
            Brush brush = Brushes.Black;

            int size = 10;
            int hs = size / 2;

            if (TopSocket.Unused)
                e.Graphics.DrawEllipse(pen, new Rectangle(TopSocket.LocalLocation.X - hs, TopSocket.LocalLocation.Y - hs, size, size));
            else
                e.Graphics.FillEllipse(brush, new Rectangle(TopSocket.LocalLocation.X - hs, TopSocket.LocalLocation.Y - hs, size, size));

            if (BottomSocket.Unused)
                e.Graphics.DrawEllipse(pen, new Rectangle(BottomSocket.LocalLocation.X - hs, BottomSocket.LocalLocation.Y - hs, size, size));
            else
                e.Graphics.FillEllipse(brush, new Rectangle(BottomSocket.LocalLocation.X - hs, BottomSocket.LocalLocation.Y - hs, size, size));

            if (LeftSocket.Unused)
                e.Graphics.DrawEllipse(pen, new Rectangle(LeftSocket.LocalLocation.X - hs, LeftSocket.LocalLocation.Y - hs, size, size));
            else
                e.Graphics.FillEllipse(brush, new Rectangle(LeftSocket.LocalLocation.X - hs, LeftSocket.LocalLocation.Y - hs, size, size));

            if (RightSocket.Unused)
                e.Graphics.DrawEllipse(pen, new Rectangle(RightSocket.LocalLocation.X - hs, RightSocket.LocalLocation.Y - hs, size, size));
            else
                e.Graphics.FillEllipse(brush, new Rectangle(RightSocket.LocalLocation.X - hs, RightSocket.LocalLocation.Y - hs, size, size));

        }

        public void RenderEdgeGraphics(Graphics g)
        {
            if (!TopSocket.Unused && TopSocket.isSource)
                TopSocket.Connector.RenderGraphicsPath(g);

            if (!BottomSocket.Unused && BottomSocket.isSource)
                BottomSocket.Connector.RenderGraphicsPath(g);

            if (!LeftSocket.Unused && LeftSocket.isSource)
                LeftSocket.Connector.RenderGraphicsPath(g);

            if (!RightSocket.Unused && RightSocket.isSource)
                RightSocket.Connector.RenderGraphicsPath(g);
        }
    }

    public class Socket
    {
        public FlowBlock FlowBlock;
        public Point LocalLocation;
        public Connector Connector = null;
        public double angle = 0;
        public double outAngle
        {
            get
            {
                return angle % 360;
            }
        }
        public double inAngle
        {
            get
            {
                return (outAngle + 180) % 360;
            }
        }

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

        public Socket(FlowBlock parent, double angle, int localX = 0, int localY = 0)
        {
            FlowBlock = parent;
            LocalLocation = new Point(localX, localY);
            this.angle = angle;
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
            //if (Connector == null)
            //{
            //    if (connector.Source != null)
            //        connector.Source.Connector = null;
                connector.Source = this;
                Connector = connector;
                return true;
            //}
            //return false;
        }

        public bool ConnectAsTarget(Connector connector)
        {
            //if (Connector == null)
            //{
            //    if (connector.Target != null)
            //        connector.Target.Connector = null;
                connector.Target = this;
                Connector = connector;
                return true;
            //}
            //return false;
        }

    }

    public class Connector
    {
        public Socket Source;
        public Socket Target;

        public string data;

        public void RenderGraphicsPath(Graphics g)
        {
            if (Source != null && Target != null)
            {
                RenderArrow(g, Pens.SlateGray, Brushes.SlateGray, 60, 8);

                int fontSize = 10;
                PointF pt = Source.Location;
                StringFormatFlags flags = StringFormatFlags.NoWrap;
                if (Source.outAngle == 0 || Source.outAngle == 180)
                {
                    flags = flags | StringFormatFlags.DirectionVertical;
                }
                StringFormat sf = new StringFormat(flags);
                FontFamily fam = new FontFamily("microsoft sans serif");
                Font font = new Font(fam, fontSize);

                SizeF size = g.MeasureString(data, font);

                if (Source.outAngle == 0)
                    pt = new PointF(pt.X + 3 - size.Height / 2, pt.Y - size.Width / 2);
                else if (Source.outAngle == 180)
                    pt = new PointF(pt.X - 3 - size.Height / 2, pt.Y - size.Width / 2);
                else if (Source.outAngle == 90)
                    pt = new PointF(pt.X - size.Width / 2, pt.Y - 3 - size.Height / 2);
                else if (Source.outAngle == 270)
                    pt = new PointF(pt.X - size.Width / 2, pt.Y + 3 - size.Height / 2);

                
                g.DrawString(data, font, Brushes.Black, pt, sf);
            }
        }

        private void RenderArrow(Graphics g, Pen pen, Brush brush, int arrowAngle, int arrowLength)
        {
            Point end = Source.Location;

            double outAngle = Source.outAngle;
            double inAngle = Target.inAngle;

            // complimentary sides (t & b, l & r)
            if (outAngle == inAngle)
            {
                // vertical
                if (Math.Abs(outAngle % 180) == 90)
                {
                    if (Source.Location.X != Target.Location.X)
                    {
                        int hy = Source.Location.Y + (Target.Location.Y - Source.Location.Y) / 2;
                        Point p1 = Source.Location;
                        Point p2 = new Point(Source.Location.X, hy);
                        Point p3 = new Point(Target.Location.X, hy);
                        g.DrawLine(pen, p1, p2);
                        g.DrawLine(pen, p2, p3);
                        end = p3;
                    }
                }
                // horizontal
                else if (Math.Abs(outAngle % 180) == 0)
                {
                    if (Source.Location.Y != Target.Location.Y)
                    {
                        int hx = Source.Location.X + (Target.Location.X - Source.Location.X) / 2;
                        Point p1 = Source.Location;
                        Point p2 = new Point(hx, Source.Location.Y);
                        Point p3 = new Point(hx, Target.Location.Y);
                        g.DrawLine(pen, p1, p2);
                        g.DrawLine(pen, p2, p3);
                        end = p3;
                    }
                }
            }
            // opposite sides (l & l, t & t, etc.)
            else if ((inAngle + 180) % 360 == outAngle)
            {
                bool set = false;
                Point p1 = Source.Location, p2 = new Point(), p3 = new Point();
                // right
                if (outAngle == 0)
                {
                    set = true;
                    int maxX = Math.Max(Source.Location.X, Target.Location.X) + 10;
                    p2 = new Point(maxX, Source.Location.Y);
                    p3 = new Point(maxX, Target.Location.Y);
                }
                // left
                else if (outAngle == 180)
                {
                    set = true;
                    int minX = Math.Min(Source.Location.X, Target.Location.X) - 10;
                    p2 = new Point(minX, Source.Location.Y);
                    p3 = new Point(minX, Target.Location.Y);
                }
                // top
                if (outAngle == 90)
                {
                    set = true;
                    int minY = Math.Max(Source.Location.Y, Target.Location.Y) - 10;
                    p2 = new Point(Source.Location.X, minY);
                    p3 = new Point(Target.Location.X, minY);
                }
                // bottom
                else if (outAngle == 270)
                {
                    set = true;
                    int maxY = Math.Min(Source.Location.Y, Target.Location.Y) + 10;
                    p2 = new Point(Source.Location.X, maxY);
                    p3 = new Point(Target.Location.X, maxY);
                }

                if (set)
                {
                    g.DrawLine(pen, p1, p2);
                    g.DrawLine(pen, p2, p3);
                    end = p3;
                }
            }
            // different axes
            else
            {
                Point p1 = Source.Location, p2 = new Point(), p3 = new Point(), p4 = new Point();
                // out right or left
                if (outAngle == 0 || outAngle == 180)
                {
                    // in bottom or top
                    if (inAngle == 90 || inAngle == 270)
                    {
                        bool comp;
                        int xOff = 10;
                        if (outAngle == 0)
                        {
                            comp = Source.Location.X < Target.Location.X;
                        }
                        else
                        {
                            comp = Source.Location.X > Target.Location.X;
                            xOff *= -1;
                        }

                        if (comp)
                        {
                            p2 = new Point(Target.Location.X, Source.Location.Y);
                            g.DrawLine(pen, p1, p2);
                            end = p2;
                        }
                        else
                        {
                            int yOff = -10;
                            if (inAngle == 90)
                                yOff *= -1;

                            p2 = new Point(Source.Location.X + xOff, p1.Y);
                            p3 = new Point(p2.X, Target.Location.Y + yOff);
                            p4 = new Point(Target.Location.X, p3.Y);
                            g.DrawLine(pen, p1, p2);
                            g.DrawLine(pen, p2, p3);
                            g.DrawLine(pen, p3, p4);
                            end = p4;
                        }
                    }
                }
                // out bottom or top
                if (outAngle == 90 || outAngle == 270)
                {
                    // in left or right
                    if (inAngle == 0 || inAngle == 180)
                    {
                        bool comp;
                        int yOff = 10;
                        if (outAngle == 90)
                        {
                            comp = Source.Location.Y > Target.Location.Y;
                            yOff *= -1;
                        }
                        else
                        {
                            comp = Source.Location.Y < Target.Location.Y;
                        }

                        if (comp)
                        {
                            p2 = new Point(Source.Location.X, Target.Location.Y);
                            g.DrawLine(pen, p1, p2);
                            end = p2;
                        }
                        else
                        {
                            int xOff = 10;
                            if (inAngle == 90)
                                xOff *= -1;

                            p2 = new Point(p1.X, Source.Location.Y + yOff);
                            p3 = new Point(Target.Location.X + xOff, p2.Y);
                            p4 = new Point(p3.X, Target.Location.Y);
                            g.DrawLine(pen, p1, p2);
                            g.DrawLine(pen, p2, p3);
                            g.DrawLine(pen, p3, p4);
                            end = p4;
                        }
                    }
                }
            }

            Point source = end;
            Point target = Target.Location;

            double initialAngleDeg = Math.Atan2(target.Y - source.Y, target.X - source.X) * 180 / Math.PI;
            double angleLRad = (initialAngleDeg - 180 + arrowAngle / 2) * Math.PI / 180;
            double angleRRad = (initialAngleDeg - 180 - arrowAngle / 2) * Math.PI / 180;

            Point arrowL = new Point(target.X + (int)(arrowLength * Math.Cos(angleLRad)), target.Y + (int)(arrowLength * Math.Sin(angleLRad)));
            Point arrowR = new Point(target.X + (int)(arrowLength * Math.Cos(angleRRad)), target.Y + (int)(arrowLength * Math.Sin(angleRRad)));

            g.FillPolygon(brush, new Point[] { target, arrowL, arrowR });
            g.DrawLine(pen, source, target);
        }
    }
}

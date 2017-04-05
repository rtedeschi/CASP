using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CASP_Standalone_Implementation.Src
{
    public enum BlockType { Start, MethodCall, Process, Loop, Decision, EndDecision, IO, End };

    public class OutlineGraph
    {
        public List<OutlineNode> nodes = new List<OutlineNode>();

        public void AddNode(OutlineNode node)
        {
            nodes.Add(node);
        }

        public void AddEdge(int sourceIndex, int targetIndex, string text = "")
        {
            nodes[sourceIndex].AddEdge(nodes[targetIndex], text);
        }
    }

    public class OutlineNode
    {
        public string text;
        public BlockType type;
        public List<OutlineEdge> edges = new List<OutlineEdge>();

        public void AddEdge(OutlineNode target, string text = "")
        {
            edges.Add(new OutlineEdge() { source = this, target = target, text = text });
        }
    }

    public class OutlineEdge
    {
        public string text;
        public OutlineNode source;
        public OutlineNode target;
    }
}

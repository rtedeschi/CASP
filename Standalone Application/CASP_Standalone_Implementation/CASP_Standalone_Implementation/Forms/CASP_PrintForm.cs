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
    public partial class CASP_PrintForm : CASP_OutputForm
    {
        class Tree
        {
            public string Title;
            public string Data;

            public List<Tree> Children = new List<Tree>();

            public bool isLeaf
            {
                get
                {
                    return Children.Count == 0;
                }
            }
        }

        public CASP_PrintForm()
        {
            InitializeComponent();
        }

        public override void Set_CASP_Output(JObject CASP_Response)
        {
            Tree t = Parse((JObject)CASP_Response["Data"]["ParseTree"]);
            
            TreeView.Nodes.Add(Display(t));

            Display(t);
        }

        TreeNode Display(Tree t)
        {
            TreeNode node = new TreeNode();
            node.Text = t.Title;

            if (!t.isLeaf)
            {
                for (int i = 0; i < t.Children.Count; i++)
                    node.Nodes.Add(Display(t.Children[i]));
            }
            else
            {
                node.Nodes.Add(new TreeNode(t.Data));
            }

            node.Expand();

            return node;
        }

        Tree Parse(JObject response)
        {
            Tree newTree = new Tree();
            JArray children = (JArray)response["Children"];

            newTree.Title = (string)response["Title"];
            newTree.Data = (string)response["Data"];

            for (int i = 0; i < children.Count; i++)
            {
                newTree.Children.Add(Parse((JObject)children[i]));
            }

            return newTree;
        }

    }
}

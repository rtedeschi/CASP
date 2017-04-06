#include "OutlineModule.h"

static string _OutlineModule = RegisterPlugin("Outline", new OutlineModule());

string EntryTypes[] = { "Start", "MethodCall", "Process", "Loop", "Decision", "EndDecision", "IO", "End" };

OutlineModule::OutlineModule() {}

CASP_Return* OutlineModule::Execute(Markup* markup, string* fnArgs, int fnArgCount) {

    /*
        This module hasn't implemented any Function Args yet!
        Use Helpers::ParseArrayArgument() and Helpers::ParseArgument() to scrape out arguments
    */

    cout << "This is the entry point for the " << _OutlineModule << " Module!\n";

    // markup->Print();
    // vector<Markup*> m = markup->FindAllById("statement", false);
    // for (int i = 0; i < m.size(); i++) {
    //     m[i]->Print();
    // }

    vector<Outline*> outlines = GetAllOutlines(markup);
    return FormatData(outlines);
}

vector<Outline*> OutlineModule::GetAllOutlines(Markup* masterTree) {
    vector<Outline*> outlines;
    vector<Markup*> functions = masterTree->FindAllById("function-definition", true);
    vector<Markup*> sls = masterTree->FindAllChildrenById("statement-list");

    if (sls.size() > 0) {
        outlines.push_back(GetRootOutline(sls));
    }
    if (functions.size() > 0) {
        for (int i = 0; i < functions.size(); i++) {
            outlines.push_back(GetFunctionOutline(functions[i]));
        }
    }

    return outlines;
}

Outline* OutlineModule::GetRootOutline(vector<Markup*> parseTrees) {
    string functionTitle = "ROOT";

    Outline* outline = new Outline();
    Node* currentNode = outline->AppendBlock(Start, functionTitle, NULL);

    for (int i = 0; i < parseTrees.size(); i++) {
        currentNode = processBlock(parseTrees[i], outline, currentNode);
    }

    outline->AppendBlock(End, "End " + functionTitle, currentNode);
    return outline;
}

Outline* OutlineModule::GetFunctionOutline(Markup* functionTree) {
    string functionTitle = functionTree->FindFirstChildById("function-identifier")->GetData();

    Outline* outline = new Outline();
    Node* currentNode = outline->AppendBlock(Start, functionTitle, NULL);

    Markup* block = functionTree->FindFirstById("block");
    currentNode = processBlock(block, outline, currentNode);

    outline->AppendBlock(End, "End " + functionTitle, currentNode);

    return outline;
}

CASP_Return* OutlineModule::FormatData(vector<Outline*> outlines) {
    CASP_Return* ret = new CASP_Return();
    GenericObject* data = ret->Data();
    GenericArray* o = new GenericArray();

    for (int i = 0; i < outlines.size(); i++) {
        o->Add(outlines[i]->Output());
        outlines[i]->Print();
        cout << endl;
    }

    data->Add("Outlines", o);

    // ret->Print();
    // cout << endl;

    return ret;
}

Node* OutlineModule::stripProcess(Markup* parseTree, Outline* outline, Node* startNode, string firstEdgeData) {

    Node* currentNode = startNode;

    string type = parseTree->GetID();
    bool sameType = currentNode->data.find(type) == 0;

    if (!sameType) {
        currentNode = outline->AppendBlock(Process, type + ":\n\t" + parseTree->GetData(), currentNode);
    } else {
        currentNode->data += "\n\t" + parseTree->GetData();
    }

    // cout << parseTree->GetID() << endl;
    // parseTree->Print();

    return currentNode;
}
Node* OutlineModule::stripMethodCall(Markup* parseTree, Outline* outline, Node* startNode, string firstEdgeData) {
    string blockData = parseTree->FindFirstChildById("function-identifier")->GetData();
    Markup* methodArgsTree = parseTree->FindFirstChildById("arg-list");

    if (methodArgsTree != NULL) {
        blockData = blockData + ": " + methodArgsTree->GetData();
    }

    return outline->AppendBlock(MethodCall, blockData, startNode, firstEdgeData);
}
Node* OutlineModule::stripDecision(Markup* parseTree, Outline* outline, Node* startNode, string firstEdgeData) {

    Node* currentDecisionHead;
    Node* currentNode = startNode;
    Node* endDecision = new Node("End Decision", EndDecision, 0);

    Markup* condition = parseTree->FindFirstChildById("expression");
    Markup* proc;
    vector<Markup*> decisionCases = parseTree->FindAllChildrenById("decision-case");
    Markup* fallback = parseTree->FindFirstChildById("decision-fallback");
    string blockData;

    blockData = condition->GetData() + "?";
    currentDecisionHead = outline->AppendBlock(Decision, blockData, currentNode, firstEdgeData);

    if ((proc = parseTree->FindFirstChildById("block")) != NULL) {
        currentNode = processBlock(proc, outline, currentDecisionHead, "True");
        currentNode->AddEdgeTo(endDecision);
    }
    else if ((proc = parseTree->FindFirstChildById("statement")) != NULL) {
        currentNode = processStatement(proc, outline, currentDecisionHead, "True");
        currentNode->AddEdgeTo(endDecision);
    }

    for (int i = 0; i < decisionCases.size(); i++) {
        condition = decisionCases[i]->FindFirstChildById("expression");
        blockData = condition->GetData() + " ?";
        currentDecisionHead = outline->AppendBlock(Decision, blockData, currentDecisionHead, "False");

        if ((proc = decisionCases[i]->FindFirstChildById("block")) != NULL) {
            currentNode = processBlock(proc, outline, currentDecisionHead, "True");
            currentNode->AddEdgeTo(endDecision);
        }
        else if ((proc = decisionCases[i]->FindFirstChildById("statement")) != NULL) {
            currentNode = processStatement(proc, outline, currentDecisionHead, "True");
            currentNode->AddEdgeTo(endDecision);
        }
    }

    if (fallback != NULL) {
        if ((proc = fallback->FindFirstChildById("block")) != NULL) {
            currentNode = processBlock(proc, outline, currentDecisionHead, "False");
            currentNode->AddEdgeTo(endDecision);
        }
        else if ((proc = fallback->FindFirstChildById("statement")) != NULL) {
            currentNode = processStatement(proc, outline, currentDecisionHead, "False");
            currentNode->AddEdgeTo(endDecision);
        }
    } else {
        currentDecisionHead->AddEdgeTo(endDecision, "False");
    }

    return outline->AppendBlock(endDecision);
}
Node* OutlineModule::stripLoop(Markup* parseTree, Outline* outline, Node* startNode, string firstEdgeData) {

    Markup* init = parseTree->FindFirstChildById("for-init");
    Markup* condition = parseTree->FindFirstChildById("for-condition");
    Markup* increment = parseTree->FindFirstChildById("for-increment");
    Markup* body = parseTree->FindFirstChildById("for-body");
    Markup* proc = NULL;
    string blockData = "Loop";

    if (init != NULL || condition != NULL || increment != NULL) {
        bool prev = false;
        blockData += ": ";
        if (init != NULL) {
            blockData += init->GetData();
            prev = true;
        }
        if (condition != NULL) {
            if (prev)
                blockData += ", ";
            blockData += condition->GetData();
            prev = true;
        }
        if (increment != NULL) {
            if (prev)
                blockData += ", ";
            blockData += increment->GetData();
        }
    }

    Node* currentNode = startNode = 
        outline->AppendBlock(Loop, blockData, startNode, firstEdgeData);

    if ((proc = body->FindFirstChildById("block")) != NULL) {
        currentNode = processBlock(proc, outline, startNode, "Loop Iteration");
        currentNode->AddEdgeTo(startNode);
    } else if ((proc = body->FindFirstChildById("statement")) != NULL) {
        currentNode = processStatement(proc, outline, startNode, "Loop Iteration");
        currentNode->AddEdgeTo(startNode);
    }

    return startNode;
}
Node* OutlineModule::processBlock(Markup* parseTree, Outline* outline, Node* startNode, string firstEdgeData) {
    Node* currentNode = startNode;
    Markup* sl = parseTree->FindFirstById("statement-list");

    if (sl != NULL) {
        vector<Markup*> statements = sl->Children();

        for (int i = 0; i < statements.size(); i++) {
            currentNode = processStatement(statements[i], outline, currentNode, firstEdgeData);
        }
    }
    return currentNode;
}
Node* OutlineModule::processStatement(Markup* statement, Outline* outline, Node* startNode, string firstEdgeData) {
    Node* currentNode = NULL;
    Markup* s = statement->ChildAt(0);
    string id = s->GetID();

    if (id == "for-loop") {
        currentNode = stripLoop(s, outline, startNode, firstEdgeData);
    } else if (id == "decision") {
        currentNode = stripDecision(s, outline, startNode, firstEdgeData);
    } else if (id == "block") {
        currentNode = processBlock(s, outline, startNode, firstEdgeData);
    } else {
        s = s->ChildAt(0);
        id = s->GetID();
        if (id == "method-invokation") {
            currentNode = stripMethodCall(s, outline, startNode, firstEdgeData);
        } else {
            currentNode = stripProcess(s, outline, startNode, firstEdgeData);
        }
    }

    return currentNode;
}

Outline::Outline() {}

GenericArray* Outline::Output() {
    GenericArray* arr = new GenericArray();

    for (int i = 0; i < nodes.size(); i++) {
        arr->Add(nodes[i]->Output());
    }

    return arr;
}

void Outline::Print() {
    // if (head != NULL) {
    //     head->Print();
    // } else {
    //     cout << "No data to print\n";
    // }

    for (int i = 0; i < nodes.size(); i++) {
        nodes[i]->Print();
    }
}

Node* Outline::AppendBlock(EntryType type, string nodeData, Node* sourceNode) {
    return AppendBlock(type, nodeData, sourceNode, "");
}

Node* Outline::AppendBlock(EntryType type, string nodeData, Node* sourceNode, string edgeData) {

    Node* node = new Node(nodeData, type, maxId++);
    if (sourceNode != NULL) {
        sourceNode->AddEdgeTo(node, edgeData);
    }
    if (head == NULL) {
        head = node;
    }
    nodes.push_back(node);

    return node;
}

Node* Outline::AppendBlock(Node* node) {

    node->id = maxId++;

    if (head == NULL) {
        head = node;
    }
    nodes.push_back(node);

    return node;
}

Node::Node(string data, EntryType type, int id) {

    this->id = id;
    this->data = data;
    this->type = type;

}

GenericObject* Node::Output() {
    GenericObject* ob = new GenericObject();
    GenericArray* arr = new GenericArray();

    ob->Add("id", new GenericLeaf<int>(id));
    ob->Add("data", new GenericLeaf<string>("\"" + data + "\""));
    ob->Add("type", new GenericLeaf<string>("\"" + EntryTypes[type] + "\""));

    for (int i = 0; i < edges.size(); i++) {
        arr->Add(edges[i]->Output());
    }

    ob->Add("edges", arr);

    return ob;
}

void Node::Print() {
    cout << id << "\t" << data << " (" << EntryTypes[type] << ")\n";
    for (int i = 0; i < edges.size(); i++) {
        cout << "\t" << (i + 1) << "\t";
        edges[i]->Print();
    }

    // for (int i = 0; i < edges.size(); i++) {
    //     if (edges[i]->target->id > id)
    //         edges[i]->target->Print();
    // }
    
}

Edge* Node::AddEdgeTo(Node* toNode) {

    Edge* edge = new Edge(this, toNode);
    edges.push_back(edge);

    return edge;
}

Edge* Node::AddEdgeFrom(Node* fromNode) {

    Edge* edge = new Edge(fromNode, this);
    fromNode->edges.push_back(edge);

    return edge;
}

Edge* Node::AddEdgeTo(Node* toNode, string edgeData) {

    Edge* edge = new Edge(this, toNode, edgeData);
    edges.push_back(edge);

    return edge;
}

Edge* Node::AddEdgeFrom(Node* fromNode, string edgeData) {

    Edge* edge = new Edge(fromNode, this, edgeData);
    fromNode->edges.push_back(edge);

    return edge;
}

Edge::Edge(Node* source, Node* target) {

    this->source = source;
    this->target = target;

}

Edge::Edge(Node* source, Node* target, string data) {

    this->data = data;
    this->source = source;
    this->target = target;

}

GenericObject* Edge::Output() {
    GenericObject* ob = new GenericObject();

    ob->Add("data", new GenericLeaf<string>("\"" + data + "\""));
    ob->Add("source", new GenericLeaf<int>(source->id));
    ob->Add("target", new GenericLeaf<int>(target->id));

    return ob;
}

void Edge::Print() {
    cout << "Edge from " << source->id << " to " << target->id;
    if (data != "")
        cout << " (" << data << ")";
    cout << endl;
}
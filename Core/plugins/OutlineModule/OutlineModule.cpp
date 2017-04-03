#include "OutlineModule.h"

static string _OutlineModule = RegisterPlugin("Outline", new OutlineModule());

string EntryTypes[] = { "Start", "Method Call", "Process", "Loop", "Decision", "I/O", "End" };

OutlineModule::OutlineModule() {}

void OutlineModule::Execute(Markup* markup, string* fnArgs, int fnArgCount) {

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

void OutlineModule::FormatData(vector<Outline*> outlines) {
    for (int i = 0; i < outlines.size(); i++) {
        outlines[i]->Print();
        cout << endl;
    }

    // return void;
}

Node* OutlineModule::stripProcess(Markup* parseTree, Outline* outline, Node* startNode) {

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
Node* OutlineModule::stripMethodCall(Markup* parseTree, Outline* outline, Node* startNode) {
    string blockData = parseTree->FindFirstChildById("function-identifier")->GetData();
    Markup* methodArgsTree = parseTree->FindFirstChildById("arg-list");

    if (methodArgsTree != NULL) {
        blockData = blockData + ": " + methodArgsTree->GetData();
    }

    return outline->AppendBlock(MethodCall, blockData, startNode);
}
Node* OutlineModule::stripDecision(Markup* parseTree, Outline* outline, Node* startNode) {

    Node* currentNode = startNode;
    // todo
    return currentNode;
}
Node* OutlineModule::stripLoop(Markup* parseTree, Outline* outline, Node* startNode) {

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
        outline->AppendBlock(Loop, blockData, startNode);

    if ((proc = body->FindFirstChildById("block")) != NULL) {
        currentNode = processBlock(proc, outline, startNode);
        currentNode->AddEdgeTo(startNode);
    } else if ((proc = body->FindFirstChildById("statement")) != NULL) {
        currentNode = processStatement(proc, outline, startNode);
        currentNode->AddEdgeTo(startNode);
    }

    return startNode;
}
Node* OutlineModule::processBlock(Markup* parseTree, Outline* outline, Node* startNode) {
    Node* currentNode = startNode;
    Markup* sl = parseTree->FindFirstById("statement-list");

    if (sl != NULL) {
        vector<Markup*> statements = sl->Children();

        for (int i = 0; i < statements.size(); i++) {
            currentNode = processStatement(statements[i], outline, currentNode);
        }
    }
    return currentNode;
}
Node* OutlineModule::processStatement(Markup* statement, Outline* outline, Node* startNode) {
    Node* currentNode = NULL;
    Markup* s = statement->ChildAt(0);
    string id = s->GetID();

    if (id == "for-loop") {
        currentNode = stripLoop(s, outline, startNode);
    } else if (id == "decision") {
        currentNode = stripDecision(s, outline, startNode);
    } else if (id == "block") {
        currentNode = processBlock(s, outline, startNode);
    } else {
        s = s->ChildAt(0);
        id = s->GetID();
        if (id == "method-invokation") {
            currentNode = stripMethodCall(s, outline, startNode);
        } else {
            currentNode = stripProcess(s, outline, startNode);
        }
    }

    return currentNode;
}

Outline::Outline() {}

void Outline::Print() {
    if (head != NULL) {
        head->Print();
    } else {
        cout << "No data to print\n";
    }
}

Node* Outline::AppendBlock(EntryType type, string nodeData, Node* sourceNode) {

    Node* node = new Node(nodeData, type, ++maxId);
    if (sourceNode != NULL) {
        sourceNode->AddEdgeTo(node);
    }
    if (head == NULL) {
        head = node;
    }

    return node;
}

Node::Node(string data, EntryType type, int id) {

    this->id = id;
    this->data = data;
    this->type = type;

}

void Node::Print() {
    cout << id << "\t" << data << " (" << EntryTypes[type] << ")\n";
    for (int i = 0; i < edges.size(); i++) {
        cout << "\t" << (i + 1) << "\t";
        edges[i]->Print();
    }

    for (int i = 0; i < edges.size(); i++) {
        if (edges[i]->target->id > id)
            edges[i]->target->Print();
    }
    
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

void Edge::Print() {
    cout << "Edge from " << source->id << " to " << target->id;
    if (data != "")
        cout << " (" << data << ")";
    cout << endl;
}
#include "AnalyzeModule.h"

static string _AnalyzeModule = RegisterPlugin("Analyze", new AnalyzeModule());

AnalyzeModule::AnalyzeModule() {}

void AnalyzeModule::Execute(Markup* markup, string* fnArgs, int fnArgCount) {

    /*
        This module hasn't implemented any Function Args yet!
        Use Helpers::ParseArrayArgument() and Helpers::ParseArgument() to scrape out arguments
    */

    cout << "This is the entry point for the " << _AnalyzeModule << " Module!\n";

}


    string functionTitle = "ROOT";

    Analyze* Analyze = new Analyze();
    Node* currentNode = Analyze->AppendBlock(Start, functionTitle, NULL);

    for (int i = 0; i < parseTrees.size(); i++) {
        currentNode = processBlock(parseTrees[i], Analyze, currentNode);
    }

    Analyze->AppendBlock(End, "End " + functionTitle, currentNode);
    return Analyze;
}

Analyze* AnalyzeModule::GetFunctionAnalyze(Markup* functionTree) {
    string functionTitle = functionTree->FindFirstChildById("function-identifier")->GetData();

    cout << "Analyze Starting for " << functionTitle << endl;

    Analyze* Analyze = new Analyze();
    Node* currentNode = Analyze->AppendBlock(Start, functionTitle, NULL);

    Markup* block = functionTree->FindFirstById("block");
    currentNode = processBlock(block, Analyze, currentNode);

    Analyze->AppendBlock(End, "End " + functionTitle, currentNode);

    cout << "Analyze Complete for " << functionTitle << endl;

    return Analyze;
}

Node* AnalyzeModule::stripMethodCall(Markup* parseTree, Analyze* Analyze, Node* startNode) {
    string blockData = parseTree->FindFirstChildById("function-identifier")->GetData();
    Markup* methodArgsTree = parseTree->FindFirstChildById("arg-list");

    if (methodArgsTree != NULL) {
        blockData = blockData + ": " + methodArgsTree->GetData();
    }

    return Analyze->AppendBlock(MethodCall, blockData, startNode);
}
Node* AnalyzeModule::stripDecision(Markup* parseTree, Analyze* Analyze, Node* startNode) {

    Node* currentNode = startNode;
    // todo
    return currentNode;
}
Node* AnalyzeModule::stripLoop(Markup* parseTree, Analyze* Analyze, Node* startNode) {

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
        Analyze->AppendBlock(Loop, blockData, startNode);

    if ((proc = body->FindFirstChildById("block")) != NULL) {
        currentNode = processBlock(proc, Analyze, startNode);
        currentNode->AddEdgeTo(startNode);
    } else if ((proc = body->FindFirstChildById("statement")) != NULL) {
        currentNode = processStatement(proc, Analyze, startNode);
        currentNode->AddEdgeTo(startNode);
    }

    return startNode;
}

Node* AnalyzeModule::processStatement(Markup* statement, Analyze* Analyze, Node* startNode) {
    Node* currentNode = NULL;
    Markup* s = statement->ChildAt(0);
    string id = s->GetID();
    cout << "s" << endl;

    if (id == "for-loop") {
        currentNode = stripLoop(s, Analyze, startNode);
    } else if (id == "decision") {
        currentNode = stripDecision(s, Analyze, startNode);
    } else if (id == "block") {
        currentNode = processBlock(s, Analyze, startNode);
    } else {
        s = s->ChildAt(0);
        id = s->GetID();
        if (id == "method-invokation") {
            currentNode = stripMethodCall(s, Analyze, startNode);
        } else {
            currentNode = stripProcess(s, Analyze, startNode);
        }
    }
    cout << "e" << endl;

    return currentNode;
}

//findby functions

Markup* Markup::FindFirstChildById(string id) {
    Markup* result = NULL;

    for (int i = 0; i < children.size(); i++) {
        if (children[i]->id == id) {
            result = children[i];
            break;
        }
    }
    
    return result;
}

vector<Markup*> Markup::FindAllById(string id, bool findChildrenOfMatches) {
    vector<Markup*> results;

    if (this->id == id) {
        results.push_back(this);
    }

    if (this->id != id || findChildrenOfMatches) {
        for (int i = 0; i < children.size(); i++) {
            vector<Markup*> v = children[i]->FindAllById(id, findChildrenOfMatches);
            results.insert(results.end(), v.begin(), v.end());
        }
    }

    return results;
}

Markup* Markup::FindFirstById(string id) {
    Markup* result = NULL;
    if (this->id == id) {
        result = this;
    } else {
        for (int i = 0; i < children.size(); i++) {
            if ((result = children[i]->FindFirstById(id)) != NULL)
                break;
        }
    }
    return result;
}
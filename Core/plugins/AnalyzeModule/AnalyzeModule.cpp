#include "AnalyzeModule.h"

static string _AnalyzeModule = RegisterPlugin("Analyze", new AnalyzeModule());

AnalyzeModule::AnalyzeModule() {}

CASP_Return* AnalyzeModule::Execute(Markup* markup, LanguageDescriptorObject* source_ldo, vector<arg> fnArgs) {

    /*
        This module hasn't implemented any Function Args yet!
        Use Helpers::ParseArrayArgument() and Helpers::ParseArgument() to scrape out arguments
    */

    cout << "This is the entry point for the " << _AnalyzeModule << " Module!\n";

    GetAllAnalyses(markup);

    return NULL;
}

void AnalyzeModule::GetAllAnalyses(Markup* masterTree) {
    vector<Markup*> functions = masterTree->FindAllById("function-definition", true);
    vector<Markup*> sls = masterTree->FindAllChildrenById("statement-list");

    if (sls.size() > 0) {
        GetRootAnalysis(sls);
    }
    if (functions.size() > 0) {
        int i;
        for (i = 0; i < functions.size(); i++) {
            string fnName = functions[i]->FindFirstChildById("function-identifier")->GetData();
            markupTable[fnName] = functions[i];
        }
        for (i = 0; i < functions.size(); i++) {
            GetFunctionAnalysis(functions[i]);
        }
    }

}

Analysis* AnalyzeModule::GetRootAnalysis(vector<Markup*> parseTrees) {

    AnalysisTree* analysis = new AnalysisTree();

    for (int i = 0; i < parseTrees.size(); i++) {
        processBlock(parseTrees[i], analysis);
    }

    return functionTable["ROOT"] = analysis->GetAnalysis();
}

Analysis* AnalyzeModule::GetFunctionAnalysis(Markup* functionTree) {

    if (functionTree == NULL)
        return NULL;

    string functionTitle = functionTree->FindFirstChildById("function-identifier")->GetData();

    if (functionTable[functionTitle] == NULL) {
        AnalysisTree* analysis = new AnalysisTree();
        Markup* block = functionTree->FindFirstById("block");
        processBlock(block, analysis);
        functionTable[functionTitle] = analysis->GetAnalysis();
    }

    return functionTable[functionTitle];
}

void AnalyzeModule::analyzeMethodCall(Markup* parseTree, AnalysisTree* analysis) {
    string functionTitle = parseTree->FindFirstChildById("function-identifier")->GetData();

    Analysis* fnAnalysis = GetFunctionAnalysis(markupTable[functionTitle]);

    // todo - Add a warning if the function doesn't exist
    AnalysisTree* node = new AnalysisTree();
    node->SetAnalysis(fnAnalysis);

    analysis->AddChild(node);
}

void AnalyzeModule::analyzeDecision(Markup* parseTree, AnalysisTree* analysis) {

    AnalysisTree* node = new AnalysisTree();
    analysis->AddChild(node);
    

    // todo
}

void AnalyzeModule::analyzeProcess(Markup* parseTree, AnalysisTree* analysis) {

    AnalysisNode* node = new AnalysisNode();
    node->SetToConstant();

    AnalysisTree* tree = new AnalysisTree();
    tree->AddFactor(node);

    analysis->AddChild(tree);

}
void AnalyzeModule::analyzeLoop(Markup* parseTree, AnalysisTree* analysis) {

    Markup* init = parseTree->FindFirstChildById("for-init")->ChilAt(0);
    Markup* condition = parseTree->FindFirstChildById("for-condition");
    Markup* increment = parseTree->FindFirstChildById("for-increment");
    Markup* body = parseTree->FindFirstChildById("for-body");
    Markup* proc = NULL;
    string blockData = "Loop";

    AnalysisTree* tree = new AnalysisTree();
    analysis->AddChild(tree);

    if (init != NULL || condition != NULL || increment != NULL) {
        bool prev = false;
        blockData += ": ";
        if (init != NULL){
            blockData += init->GetData();
            prev = true;
        }
        if(condition != NULL){
            blockData += condition->GetData();
            prev = true;
        }
        if(increment != NULL){
            blockData += increment->GetData();
            prev = true;
        }

    }

    if ((proc = body->FindFirstChildById("block")) != NULL) {
        processBlock(proc, tree);
    } else if ((proc = body->FindFirstChildById("statement")) != NULL) {
        processStatement(proc, tree);
    }


}

void AnalyzeModule::processStatement(Markup* statement, AnalysisTree* analysis, Node* startNode, string firstEdgeData) {
    Node* currentNode = NULL;
    Markup* s = statement->ChildAt(0);
    string id = s->GetID();

    if (id == "for-loop") {
        analyzeLoop(s, analysis);
    } else if (id == "decision") {
        analyzeDecision(s, analysis);
    } else if (id == "block") {
        processBlock(s, analysis);
    } else if (id == "expression-statement") {
        s = s->ChildAt(0)->ChildAt(0);
        id = s->GetID();
        while (id == "grouped-expression") {
            s = s->ChildAt(1);
            id = s->GetID();
        }

        if (id == "method-invokation") {
            currentNode = stripMethodCall(s, analysis, startNode, firstEdgeData);
        }
        else {
            currentNode = stripProcess(s, analysis, startNode, firstEdgeData);
        }
    }

    return currentNode;
}
void AnalyzeModule::processBlock(Markup* parseTree, AnalysisTree* analysis) {
    Markup* sl = parseTree->FindFirstById("statement-list");

    Node* currentNode = startNode;
    Markup* cs = NULL;
    int ct = 0;

    while (sl != NULL) {
        cs = sl->FindFirstChildById("statement");
        currentNode = processStatement(cs, analysis, currentNode, ct++ == 0 ? firstEdgeData : "");
        sl = sl->FindFirstChildById("statement-list");
    }
    return currentNode;
}

AnalysisTree::AnalysisTree() {
    analysis = new Analysis();
}

void AnalysisTree::AddChild(AnalysisTree* tree) {
    children.push_back(tree);
}

void AnalysisTree::SetAnalysis(Analysis* analysis) {
    this->analysis = analysis;
}

Analysis* AnalysisTree::GetAnalysis() {
    // todo - this should condense the tree into a single analysis
    return analysis;
}

void AnalysisTree::AddFactor(AnalysisNode* node) {
    analysis->AddFactor(node);
}

Analysis::Analysis() {}

void Analysis::AddFactor(AnalysisNode* node) {
    children.push_back(node);
}

string Analysis::ToString() {
    // todo
    return "";
}

AnalysisNode::AnalysisNode() {}

void AnalysisNode::SetToConstant() {
    this->base = 1;
    this->exponent = 1;
    this->type = Constant;
}
void AnalysisNode::SetToExponential(int exponent){
    this->base = 1;
    this->exponent = exponent;
    this->type = Exponential;
}
void AnalysisNode::SetToLogarithmic(int base, int exponent){
    this->base = base;
    this->exponent = exponent;
    this->type = Logarithmic;
}
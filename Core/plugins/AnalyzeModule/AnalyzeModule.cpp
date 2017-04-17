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
    // analyze each block, add worst-case block to analysis

    /* each block in tree stored as a list nlogn would be push as n,logn 
    */
    
    Markup* condition = parseTree->FindFirstChildById("expression");
    Markup* body = parseTree->FindFirstChildById("decision-body");
    Markup* proc;
    vector<Markup*> decisionCases = parseTree->FindFirstChildById("decision-cases")->RecursiveElements();
    Markup* fallback = parseTree->FindFirstChildById("decision-fallback");

    // process if expression here, too
    if ((proc = body->FindFirstChildById("block")) != NULL) {
        processBlock(proc, node);
    }
    else if ((proc = body->FindFirstChildById("statement")) != NULL) {
        processStatement(proc, node);
    }

    for (int i = 0; i < decisionCases.size(); i++) {
        // create a new tree node and append it to the current tree?
        // process else-if expression here, too
        condition = decisionCases[i]->FindFirstChildById("expression");
        body = decisionCases[i]->FindFirstChildById("decision-body");

        if ((proc = body->FindFirstChildById("block")) != NULL) {
            processBlock(proc, node);
        }
        else if ((proc = body->FindFirstChildById("statement")) != NULL) {
            processStatement(proc, node);
        }
    }

    if (fallback != NULL) {
        // create a new tree node and append it to the current tree?
        body = fallback->FindFirstChildById("decision-body");
        if ((proc = body->FindFirstChildById("block")) != NULL) {
            processBlock(proc, node);
        }
        else if ((proc = body->FindFirstChildById("statement")) != NULL) {
            processStatement(proc, node);
        }
    }

}

void AnalyzeModule::analyzeProcess(Markup* parseTree, AnalysisTree* analysis) {

    AnalysisNode* node = new AnalysisNode();
    node->SetToConstant();

    AnalysisTree* tree = new AnalysisTree();
    tree->AddFactor(node);

    analysis->AddChild(tree);

}
void AnalyzeModule::analyzeLoop(Markup* parseTree, AnalysisTree* analysis) {

    Markup* init = parseTree->FindFirstChildById("for-init")->ChildAt(0);
    Markup* condition = parseTree->FindFirstChildById("for-condition")->ChildAt(0);
    Markup* increment = parseTree->FindFirstChildById("for-increment")->ChildAt(0);
    Markup* body = parseTree->FindFirstChildById("for-body");
    Markup* proc = NULL;

    AnalysisTree* tree = new AnalysisTree();
    analysis->AddChild(tree);

    string id;
    bool hasInit = false;
    int initVal = 0;
    bool hasCondition = false;
    int term = 0;
    bool hasIncrement = false;
    int incr = 0;

    if (init != NULL || condition != NULL || increment != NULL) {
        if (init != NULL){
            // Get initial condition
            Markup* assign = init->FindFirstById("assignment-tail");
            if (assign != NULL) {
                Markup* identifier = init->FindFirstById("identifier"); // could be float literal?
                id = identifier->GetData();
                Markup* lit = assign->FindFirstById("INT_LITERAL"); // could be float literal or id?
                if (lit != NULL) {
                    hasInit = true;
                    initVal = stoi(lit->GetData());
                } else {
                    // An appropriate literal was not found. Could be an expression. Calculate?
                }
            } else{
                // there is no definition here, look for it elsewhere based on the condition/increment?
            }

            cout << id << " = " << initVal << endl;
        }
        if(condition != NULL){
            // Get final condition
            Markup* operation = condition->FindFirstById("relational-expression");
            if (operation != NULL){
                Markup* op = operation->FindFirstChildById("relational-binary-op");
                Markup* identifier = operation->FindFirstChildById("identifier"); // could be float literal?
                id = identifier->GetData();
                Markup* lit = operation->FindFirstById("INT_LITERAL"); // could be float literal or id?
                // assume identifier is on left, value is on right
                if (identifier != NULL && lit != NULL && (id == identifier->GetData())){
                    string opType = op->ChildAt(0)->GetID();
                    if (opType == "LT") {

                    } else if (opType == "LT_EQ"){
                        
                    } else if (opType == "GT") {

                    } else if (opType == "GT_EQ"){
                        
                    } else if (opType == "EQ") {
                        // requires extra calculation. Ignore for now?
                    } else if (opType == "NOT_EQ") {
                        // requires extra calculation. Ignore for now?
                    }

                } else {
                    // an appropriate expression could not be found
                }

            } else {
                // no relational condition. Can't calculate?
            }
        }
        if(increment != NULL){
            // get increment
            Markup* operation = increment->FindFirstById("algebraic-expression");
            if(operation != NULL){
                Markup* op = operation->FindFirstChildById("math-binary-op");
                Markup* identifier = operation->FindFirstChildById("identifier");
                id = identifier->GetData();
                if(identifier != NULL && op != NULL){
                    string opType = op->ChildAt(0)->GetID();
                    hasIncrement = true;
                    if(opType == "INCR"){

                        

                    } else if(opType == "DECR"){


                    }



                }
            }else {
                //get unary operation 
                Markup* op = operation-> FindFirstById("unary-expression");
                Markup* identifier = operation->FindFirstChildById("identifier");
                if(op != NULL){
                    Markup* post = op->FindFirstChildById("unary-postfix-expression");
                    Markup* pre = op->FindFirstChildById("unary-prefix-expression");
                    if(post != NULL || pre != NULL){

                    }

                } else{
                    //nothing

                }
            }
        }

    }

    if ((proc = body->FindFirstChildById("block")) != NULL) {
        processBlock(proc, tree);
    } else if ((proc = body->FindFirstChildById("statement")) != NULL) {
        processStatement(proc, tree);
    }


}

void AnalyzeModule::processStatement(Markup* statement, AnalysisTree* analysis) {
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

        if (id == "method-invocation") {
            analyzeMethodCall(s, analysis);
        }
        else {
            analyzeProcess(s, analysis);
        }
    }
}
void AnalyzeModule::processBlock(Markup* parseTree, AnalysisTree* analysis) {
    Markup* sl = parseTree->FindFirstById("statement-list");

    Markup* cs = NULL;
    int ct = 0;

    while (sl != NULL) {
        cs = sl->FindFirstChildById("statement");
        processStatement(cs, analysis);
        sl = sl->FindFirstChildById("statement-list");
    }
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
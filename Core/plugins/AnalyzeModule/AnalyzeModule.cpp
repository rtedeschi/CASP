#include "AnalyzeModule.h"

static string _AnalyzeModule = RegisterPlugin("Analyze", new AnalyzeModule());

AnalyzeModule::AnalyzeModule() {}

CASP_Return* AnalyzeModule::Execute(Markup* markup, LanguageDescriptorObject* source_ldo, vector<arg> fnArgs, CASP_Return* inputReturn) {
    returnData = (inputReturn != NULL ? inputReturn : new CASP_Return());

    /*
        This module hasn't implemented any Function Args yet!
        Use Helpers::ParseArrayArgument() and Helpers::ParseArgument() to scrape out arguments
    */

    cout << "This is the entry point for the " << _AnalyzeModule << " Module!\n";

    GetAllAnalyses(markup);

    for (auto it = functionTable.begin(); it != functionTable.end(); it++) {
        bool undefined = it->second == NULL || it->second->IsUndefined();
        string analysis = it->second != NULL ? it->second->ToString() : "Undefined";
        GenericObject* ob = CreateObject({
            { "IsUndefined", CreateLeaf(undefined) },
            { "Analysis", CreateLeaf(analysis) },
            { "Title", CreateLeaf(it->first) }
        });
        returnData->Data()->Add(it->first, ob);
        // if (it->second != NULL) {
        //     cout << it->first << ": O(" << it->second->ToString() << ")" << endl;
        // } else {
        //     cout << it->first << ": Undefined" << endl;
        // }
    }

    return returnData;
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
        Markup* dc = decisionCases[i]->FindFirstChildById("decision-case");
        condition = dc->FindFirstChildById("expression");
        body = dc->FindFirstChildById("decision-body");

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

    AnalysisTree* tree = new AnalysisTree();
    tree->AddConstantFactor();

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

    AnalysisNode* a = new AnalysisNode();
    a->SetToUndefined();

    string conditionalOp = "";
    int incrVal = 0;
    string id = "";
    string conditional = "";
    bool idValSet = false;
    int idVal = 0;
    bool conditionalValSet = false;
    int conditionalVal = 0;

    unordered_map<string, Markup*> declaredIds;


    if (init != NULL || condition != NULL || increment != NULL) {
        // TODO if the incremented id is declared outside of the for loop, this won't operate correctly
        if (init != NULL) {
            // Get initial condition
            Markup* assign = NULL;
            
            if ((assign = init->FindFirstChildById("assignment")) != NULL) {
                string ident = assign->FindFirstChildById("assignment-target")->GetData();
                Markup* expr = assign->FindFirstChildById("assignment-tail")->ChildAt(0)->FindFirstChildById("assign-expression")->ChildAt(0);

                declaredIds[ident] = ActionRoutines::ExecuteAction("ResolveExpr", parseTree, { expr });
                
            } else if ((assign = init->FindFirstChildById("declaration")) == NULL) {

                Markup* start = init->FindFirstById("initializer-list");
                vector<Markup*> recursive = start->RecursiveElements();
                vector<Markup*> list = { start };
                list.insert(list.end(), recursive.begin(), recursive.end());

                for (int i = 0; i < list.size(); i++) {
                    string ident = list[i]->FindFirstChildById("identifier")->GetData();
                    Markup* expr = list[i]->FindFirstChildById("initializer-assignment-tail")->ChildAt(0)->FindFirstChildById("assign-expression")->ChildAt(0);
                cout << "Declared " << ident << endl;
                
                    declaredIds[ident] = ActionRoutines::ExecuteAction("ResolveExpr", parseTree, { expr });
                }

            } else {
                // there is no definition here, look for it elsewhere based on the condition/increment?
            }

        }
        if(increment != NULL){
            // get increment
            Markup* operation;
            if ((operation = increment->FindFirstChildById("assignment")) != NULL) {
                id = operation->FindFirstChildById("assignment-target")->GetData();
                if (declaredIds[id] != NULL && declaredIds[id]->GetID() == "INT_LITERAL") {
                    idValSet = true;
                    idVal = stoi(declaredIds[id]->GetData());
                }
                Markup* tail = operation->FindFirstChildById("assignment-tail");
                Markup* t = NULL;
                if ((t = tail->FindFirstChildById("algebraic-assignment-tail")) != NULL) {
                    Markup* op = t->FindFirstChildById("math-assign-op")->ChildAt(0);
                    Markup* expr = ActionRoutines::ExecuteAction("ResolveExpr", parseTree, { t->FindFirstChildById("assign-expression")->ChildAt(0) });

                    if (expr->GetID() == "INT_LITERAL") {
                        string opId = op->GetID();
                        if (opId == "PLUS_ASSIGN" || opId == "MINUS_ASSIGN") {
                            a->SetToExponential(1);
                        } else if (opId == "ASTERISK_ASSIGN" || opId == "SLASH_ASSIGN") {
                            int base = 10; //stoi(expr->GetData());
                            a->SetToLogarithmic(base, 1);
                        }
                        incrVal = stoi(expr->GetData());
                        conditionalOp = opId;
                    } else {
                        // unable to calculate
                    }

                } else if ((t = tail->FindFirstChildById("standard-assignment-tail")) != NULL) {
                    // TODO This is potentially complex logic
                }
            }
            else if ((operation = increment->FindFirstChildById("operation")) != NULL) {
                Markup* unary = operation->FindFirstChildById("unary-expression");
                if (unary != NULL) {
                    string opType = unary->ChildAt(0)->GetID();
                    if (opType == "unary-postfix-expression" || opType == "unary-prefix-expression") {
                        Markup* op = unary->ChildAt(0)->FindFirstChildById("unary-op");
                        Markup* identifier = unary->ChildAt(0)->FindFirstChildById("identifier");
                        id = identifier->GetData();
                        opType = op->ChildAt(0)->GetID();
                        if(opType == "INCR") {
                            a->SetToExponential(1);
                            conditionalOp = "PLUS";
                        } else if(opType == "DECR") {
                            a->SetToExponential(1);
                            conditionalOp = "MINUS";
                        }
                        incrVal = 1;
                    }
                } else {
                    // any other operation does nothing
                }
            } else {
                // can't get an increment
            }
        }
        if(condition != NULL){
            // Get final condition
            Markup* operation = condition->ChildAt(0)->ChildAt(0)->FindFirstChildById("relational-expression");
            if (operation != NULL) {
                Markup* lExpr = operation->FindFirstChildById("operation-expression")->ChildAt(0);
                lExpr = ActionRoutines::ExecuteAction("ResolveExpr", parseTree, { lExpr });
                Markup* rExpr = operation->FindFirstChildById("relational-expression-tail")->FindFirstChildById("operation-expression")->ChildAt(0);
                rExpr = ActionRoutines::ExecuteAction("ResolveExpr", parseTree, { rExpr });
                Markup* op = operation->FindFirstChildById("relational-expression-tail")->FindFirstChildById("relational-binary-op")->ChildAt(0);
                string opType = op->GetID();
                // id = identifier->GetData();
                Markup* lit = operation->FindFirstById("INT_LITERAL"); // could be float literal or id?
                string lType = lExpr->GetID();
                string rType = rExpr->GetID();

                // the calculation can only be done right now if at least one side resolves to an ID
                if (lType == "ID" || rType == "ID") {
                    if (lExpr->GetData() == id) {
                        if (rType == "INT_LITERAL") {
                            conditionalValSet = true;
                            conditionalVal = stoi(rExpr->GetData());
                        }
                        conditional = opType;
                    } else if (rExpr->GetData() == id) {
                        if (lType == "INT_LITERAL") {
                            conditionalValSet = true;
                            conditionalVal = stoi(lExpr->GetData());
                        }
                        // reverse the operator to move the conditional operand to the right side
                        if (opType == "LT") {
                            conditional = "GT";
                        } else if (opType == "LT_EQ") {
                            conditional = "GT_EQ";
                        } else if (opType == "GT") {
                            conditional = "LT";
                        } else if (opType == "GT_EQ") {
                            conditional = "LT_EQ";
                        }
                    }
                }

            } else {
                // there is no relational condition
            }
        }

        if (conditional != "" && conditionalOp != "") {
            if (conditionalOp == "LT" || conditionalOp == "LT_EQ") {
                if (((conditionalOp == "MINUS" && incrVal >= 0) ||
                    (conditionalOp == "PLUS" && incrVal <= 0) ||
                    (conditionalOp == "ASTERISK" && incrVal <= 1 && incrVal > -1) ||
                    (conditionalOp == "SLASH" && (incrVal >= 1 || incrVal <= -1)))) {
                    // TODO add warning for probable infinite loop
                    a->SetToUndefined();
                } else if (!(conditionalValSet && idValSet) && (conditionalValSet || idValSet)) {
                    if (conditionalOp == "MINUS" || conditionalOp == "PLUS")
                        a->SetToExponential(1);
                    else if (conditionalOp == "SLASH" || conditionalOp == "ASTERISK")
                        a->SetToLogarithmic(10/*incrVal*/, 1);
                }
                // 
            } else if (conditionalOp == "GT" || conditionalOp == "GT_EQ") {
                if (((conditionalOp == "MINUS" && incrVal <= 0) ||
                    (conditionalOp == "PLUS" && incrVal >= 0) ||
                    (conditionalOp == "ASTERISK" && (incrVal >= 1 || incrVal <= -1)) ||
                    (conditionalOp == "SLASH" && incrVal <= 1 && incrVal > -1))) {
                    
                    // TODO add warning for probable infinite loop
                    a->SetToUndefined();
                } else if (!(conditionalValSet && idValSet) && (conditionalValSet || idValSet)) {
                    if (conditionalOp == "MINUS" || conditionalOp == "PLUS")
                        a->SetToExponential(1);
                    else if (conditionalOp == "SLASH" || conditionalOp == "ASTERISK")
                        a->SetToLogarithmic(10/*incrVal*/, 1);
                }
                // 
            } else if (conditionalOp == "EQ") {
                // TODO requires extra calculation.
            } else if (conditionalOp == "NOT_EQ") {
                // TODO requires extra calculation.
            }
        }

    }

    tree->AddFactor(a);

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
    analysis->AddConstantFactor();
}

void AnalysisTree::AddChild(AnalysisTree* tree) {
    children.push_back(tree);
}

void AnalysisTree::SetAnalysis(Analysis* analysis) {
    this->analysis = analysis;
}

Analysis* AnalysisTree::GetAnalysis() {

    if (children.size() > 0) {
        Analysis* max = children[0]->GetAnalysis();
        Analysis* c = NULL;
        for (int i = 1; i < children.size(); i++) {
            c = children[i]->GetAnalysis();
            if (*c > *max)
                max = c;
        }

        return &(*analysis * *max);

    } else {
        return analysis;
    }
}

void AnalysisTree::AddFactor(AnalysisNode* node) {
    analysis->AddFactor(node);
}

void AnalysisTree::AddConstantFactor() {
    analysis->AddConstantFactor();
}
void AnalysisTree::AddExponentialFactor(int exponent) {
    analysis->AddExponentialFactor(exponent);
}
void AnalysisTree::AddLogarithmicFactor(int base, int exponent) {
    analysis->AddLogarithmicFactor(base, exponent);
}

Analysis::Analysis() {

}

bool Analysis::IsUndefined() {
    return undefined;
}

void Analysis::AddConstantFactor() {
    AnalysisNode* node = new AnalysisNode();
    node->SetToConstant();
    AddFactor(node);
}
void Analysis::AddExponentialFactor(int exponent) {
    AnalysisNode* node = new AnalysisNode();
    node->SetToExponential(exponent);
    AddFactor(node);
}
void Analysis::AddLogarithmicFactor(int base, int exponent) {
    AnalysisNode* node = new AnalysisNode();
    node->SetToLogarithmic(base, exponent);
    AddFactor(node);
}

void Analysis::AddFactor(AnalysisNode* node) {
    switch (node->type) {
        case Undefined:
            undefined = true;
            break;
        case Constant:
            if (this->constant == NULL) {
                this->constant = node;
            }
            break;
        case Exponential:
            if (this->exponential == NULL) {
                this->exponential = node;
            } else {
                this->exponential = &(*this->exponential * *node);
            }
            break;
        case Logarithmic:
            if (this->logarithmic == NULL) {
                this->logarithmic = node;
            } else {
                this->logarithmic = &(*this->logarithmic * *node);
            }
            break;
    }
}

string Analysis::ToString() {

    string str = "";

    if (undefined) {
        str += "Undefined";
    } else {

        if (exponential != NULL) {
            str += exponential->ToString();
        }
        
        if (logarithmic != NULL) {
            if (str != "")
                str += " ";
            str += logarithmic->ToString();
        }

        if (str == "" && constant != NULL) {
            str += constant->ToString();
        }

    }

    return str;
}

Analysis& Analysis::operator*(Analysis& r) {
    Analysis* a = new Analysis();

    if (r.undefined || this->undefined) {
        a->undefined = true;
        return *a;
    }

    if (this->exponential != NULL && r.exponential != NULL)
        a->exponential = &(*(this->exponential) * *(r.exponential));
    else if (this->exponential != NULL) 
        a->exponential = this->exponential;
    else if (r.exponential != NULL) 
        a->exponential = r.exponential;

    if (this->logarithmic != NULL && r.logarithmic != NULL)
        a->logarithmic = &(*(this->logarithmic) * *(r.logarithmic));
    else if (this->logarithmic != NULL) 
        a->logarithmic = this->logarithmic;
    else if (r.logarithmic != NULL) 
        a->logarithmic = r.logarithmic;

    if (this->constant != NULL) 
        a->constant = this->constant;
    else if (r.constant != NULL) 
        a->constant = r.constant;

    return *a;
}

bool operator==(const Analysis& l, const Analysis& r) {
    bool same = true;

    if (l.undefined && r.undefined)
        return true;
    else if (l.undefined || r.undefined)
        return false;

    if (l.exponential != NULL && r.exponential != NULL)
        same = same && *(l.exponential) == *(r.exponential);
    else if ((l.exponential == NULL || r.exponential == NULL) && !(l.exponential == NULL && r.exponential == NULL))
        return false;

    if (l.logarithmic != NULL && r.logarithmic != NULL)
        same = same && *(l.logarithmic) == *(r.logarithmic);
    else if ((l.logarithmic == NULL || r.logarithmic == NULL) && !(l.logarithmic == NULL && r.logarithmic == NULL))
        return false;

    if (l.exponential == NULL && r.exponential == NULL && l.logarithmic == NULL && r.logarithmic == NULL) {
        if (l.constant != NULL && r.constant != NULL)
            same = same && true;
        else if ((l.constant == NULL || r.constant == NULL) && !(l.constant == NULL && r.constant == NULL))
            return false;
    }

    return same;
}
bool operator!=(const Analysis& l, const Analysis& r) {
    return !(l == r);
}
bool operator>(const Analysis& l, const Analysis& r) {
    bool gtr = true;

    if (l.undefined)
        return false;
    else if (r.undefined)
        return true;

    if (l.exponential != NULL && r.exponential != NULL) {
        gtr = gtr && *(l.exponential) > *(r.exponential);
    } else if (l.exponential == NULL && r.exponential == NULL) {

    } else if (l.exponential == NULL) {
        return false;
    } else {
        return true;
    }

    if (l.logarithmic != NULL && r.logarithmic != NULL) {
        gtr = gtr && *(l.logarithmic) > *(r.logarithmic);
    } else if (l.logarithmic == NULL && r.logarithmic == NULL) {

    } else if (l.logarithmic == NULL) {
        return false;
    } else {
        return true;
    }

    if (l.exponential == NULL && r.exponential == NULL && l.logarithmic == NULL && r.logarithmic == NULL) {
        
        if (l.constant == NULL)
            return false;
        else if (r.constant == NULL)
            return true;
    }

    return gtr;
}
bool operator>=(const Analysis& l, const Analysis& r) {
    return (l > r || l == r);
}
bool operator<(const Analysis& l, const Analysis& r) {
    return (r > l);
}
bool operator<=(const Analysis& l, const Analysis& r) {
    return (l < r || l == r);
}

AnalysisNode::AnalysisNode() {}

AnalysisNode& AnalysisNode::operator=(AnalysisNode& target) {
    if (this != &target) {
        this->type = target.type;
        this->base = target.base;
        this->exponent = target.exponent;
    }
    return *this;
}
AnalysisNode* AnalysisNode::operator=(AnalysisNode* target) {
    if (this != target) {
        this->type = target->type;
        this->base = target->base;
        this->exponent = target->exponent;
    }
    return this;
}
AnalysisNode& AnalysisNode::operator*(AnalysisNode& r) {
    AnalysisNode* node = new AnalysisNode();

    if (this->type == r.type && (this->type != Logarithmic || this->base == r.base)) {
        node = this;
        node->exponent += r.exponent;
    }

    return *node;
}
bool operator==(const AnalysisNode& l, const AnalysisNode& r) {
    return (r.type == l.type && r.exponent == l.exponent && r.base == l.base);
}
bool operator!=(const AnalysisNode& l, const AnalysisNode& r) {
    return !(l == r);
}
bool operator>(const AnalysisNode& l, const AnalysisNode& r) {
    if (l.type != Undefined && r.type != Undefined) {

        if (l.type == r.type) {
            if (l.exponent == r.exponent) {
                if (r.type == Logarithmic) {
                    return !(r.base == l.base || r.base > l.base);
                } else {
                    return false;
                }
            } else {
                return (r.exponent < l.exponent);
            }

        } else if (l.type == Constant || r.type == Exponential) {
            return false;
        } else if (l.type == Exponential || r.type == Constant) {
            return true;
        }

    } else if (l.type == Undefined) {
        return false;
    } else if (r.type == Undefined) {
        return true;
    }
    return false;
}
bool operator>=(const AnalysisNode& l, const AnalysisNode& r) {
    return (l > r || l == r);
}
bool operator<(const AnalysisNode& l, const AnalysisNode& r) {
    return (r > l);
}
bool operator<=(const AnalysisNode& l, const AnalysisNode& r) {
    return (l < r || l == r);
}

string AnalysisNode::ToString() {
    string str = "";
    if (type == Logarithmic) {
        str = "log(n)";
        if (exponent != 1) {
            str = "(" + str + ")^" + to_string(exponent);
        }
    } else if (type == Exponential) {
        str = "n";
        if (exponent != 1) {
            str += "^" + to_string(exponent);
        }
    } else if (type == Constant) {
        str = "C";
    }
    return str;
}

void AnalysisNode::SetToUndefined() {
    this->base = 1;
    this->exponent = 1;
    this->type = Undefined;
}
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
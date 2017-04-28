/*  
 *  AnalyzeModule.h
 *  
 *  
 *  Created: 3/24/2017 by Ryan Tedeschi
 */

#ifndef ANALYZEMODULE_H
#define ANALYZEMODULE_H

#include <string>
#include <vector>
#include <iostream>
#include "../../shared/CASP_Plugin/CASP_Plugin.h"

using namespace std;

enum NodeType { Constant, Exponential, Logarithmic, Undefined };

class AnalysisNode {
    public:
        AnalysisNode();

        void SetToUndefined();
        void SetToConstant();
        void SetToExponential(int exponent);
        void SetToLogarithmic(int base, int exponent);
        
        string ToString();

        int exponent = 1;
        int base = 1;
        NodeType type = Undefined;

        friend bool operator==(const AnalysisNode&, const AnalysisNode&);
        friend bool operator!=(const AnalysisNode&, const AnalysisNode&);
        friend bool operator>(const AnalysisNode&, const AnalysisNode&);
        friend bool operator>=(const AnalysisNode&, const AnalysisNode&);
        friend bool operator<(const AnalysisNode&, const AnalysisNode&);
        friend bool operator<=(const AnalysisNode&, const AnalysisNode&);
        AnalysisNode& operator*(AnalysisNode&);
        AnalysisNode& operator=(AnalysisNode&);
        AnalysisNode* operator=(AnalysisNode*);

    private:

};

class Analysis {

    public:
        Analysis();
        void AddFactor(AnalysisNode*);
        void AddConstantFactor();
        void AddExponentialFactor(int);
        void AddLogarithmicFactor(int, int);
        string ToString();

        friend bool operator==(const Analysis&, const Analysis&);
        friend bool operator!=(const Analysis&, const Analysis&);
        friend bool operator>(const Analysis&, const Analysis&);
        friend bool operator>=(const Analysis&, const Analysis&);
        friend bool operator<(const Analysis&, const Analysis&);
        friend bool operator<=(const Analysis&, const Analysis&);
        Analysis& operator*(Analysis&);

        bool IsUndefined();

    private:
        AnalysisNode* constant = NULL;
        AnalysisNode* exponential = NULL;
        AnalysisNode* logarithmic = NULL;

        bool undefined = false;
};

class AnalysisTree {
    public:
        AnalysisTree();

        void AddChild(AnalysisTree*);
        void AddFactor(AnalysisNode*);
        void AddConstantFactor();
        void AddExponentialFactor(int);
        void AddLogarithmicFactor(int, int);
        void SetAnalysis(Analysis*);

        Analysis* GetAnalysis();

    private:
    
        vector<AnalysisTree*> children;
        Analysis* analysis = NULL;

};

class AnalyzeModule : public CASP_Plugin {
    public:
        AnalyzeModule();

        virtual CASP_Return* Execute(Markup* markup, LanguageDescriptorObject* source_ldo, vector<arg> fnArgs, CASP_Return* inputReturn = NULL);

    private:
        void GetAllAnalyses(Markup*);
        Analysis* GetRootAnalysis(vector<Markup*>);
        Analysis* GetFunctionAnalysis(Markup*);

        void analyzeProcess(Markup*, AnalysisTree*);
        void analyzeMethodCall(Markup*, AnalysisTree*);
        void analyzeDecision(Markup*, AnalysisTree*);
        void analyzeLoop(Markup*, AnalysisTree*);
        void processStatement(Markup*, AnalysisTree*);
        void processBlock(Markup*, AnalysisTree*); 

        unordered_map<string, Analysis*> functionTable;
        unordered_map<string, Markup*> markupTable;

};

#endif
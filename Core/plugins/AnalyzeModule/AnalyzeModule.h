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

enum NodeType { Constant, Exponential, Logarithmic };

class AnalysisNode {
    public:
        AnalysisNode();

        void SetToConstant();
        void SetToExponential(int exponent);
        void SetToLogarithmic(int base, int exponent);

        int exponent = 1;
        int base = 1;
        NodeType type = Constant;

    private:

};

class Analysis {

    public:
        Analysis();
        void AddFactor(AnalysisNode*);
        string ToString();

    private:
        vector<AnalysisNode*> children;


};

class AnalysisTree {
    public:
        AnalysisTree();

        void AddChild(AnalysisTree*);
        void AddFactor(AnalysisNode*);
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
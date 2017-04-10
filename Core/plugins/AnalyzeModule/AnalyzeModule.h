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

enum NodeType { Constant, Power, Logarithm };

class AnalysisNode {
    public:
        AnalysisNode(int, NodeType);

        int base = 1;
        NodeType type = Constant;

    private:
        Markup* markup = NULL;

};

class AnalysisTree {
    public:
        AnalysisTree(Markup*);

    private:
    
        vector<AnalysisTree*> children;
        vector<AnalysisNode*> analysis;


};

class AnalyzeModule : public CASP_Plugin {
    public:
        AnalyzeModule();

        virtual CASP_Return* Execute(Markup*, LanguageDescriptorObject*, vector<arg>);

    private:
        vector<AnalysisTree*> GetAllAnalysis(Markup*);
        AnalysisTree* GetRootAnalyze(vector<Markup*>);
        AnalysisTree* GetFunctionAnalyze(Markup*);

        AnalysisTree* analyzeProcess(Markup*, AnalysisTree*);
        AnalysisTree* analyzeMethodCall(Markup*, AnalysisTree*);
        AnalysisTree* analyzeDecision(Markup*, AnalysisTree*);
        AnalysisTree* analyzeLoop(Markup*, AnalysisTree*);
        AnalysisTree* processStatement(Markup*, AnalysisTree*);
        AnalysisTree* processBlock(Markup*, AnalysisTree*); 

};

#endif
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

class AnalyzeModule : public CASP_Plugin {
    public:
        AnalyzeModule();

        virtual CASP_Return* Execute(Markup*, string*, int);

    private:
        vector<Analyze*> GetAllAnalysis(Markup*);
        Analyze* GetRootAnalyze(vector<Markup*>);
        Analyze* GetFunctionAnalyze(Markup*);

        Node* stripProcess(Markup*, Analyze*, Node*);
        Node* stripMethodCall(Markup*, Analyze*, Node*);
        Node* stripDecision(Markup*, Analyze*, Node*);
        Node* stripLoop(Markup*, Analyze*, Node*);
        Node* processStatement(Markup*, Analyze*, Node*);
        Node* processBlock(Markup*, Analyze*, Node*); 

};

#endif
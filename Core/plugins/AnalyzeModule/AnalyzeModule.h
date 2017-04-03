/*  
 *  AnalyzeModule.h
 *  
 *  
 *  Created: 3/24/2017 by Ryan Tedeschi
 */

#ifndef ANALYZEMODULE_H
#define ANALYZEMODULE_H

#include <string>
#include <iostream>
#include "../../shared/CASP_Plugin/CASP_Plugin.h"

using namespace std;

class AnalyzeModule : public CASP_Plugin {
    public:
        AnalyzeModule();

         void AddChild(Markup*);
         Markup* Parent();

        // Finds the first matching child by identifier, null if no match
        Markup* FindFirstChildById(string);
        // Finds the first matching node in self or any descendants by identifier, null if no match
        Markup* FindFirstById(string);
        // Finds all matching children by identifier
        vector<Markup*> FindAllChildrenById(string);
        // Finds all matching self or descendants by identifier
        vector<Markup*> FindAllById(string, bool);
       


    private:
        Node* stripProcess(Markup*, Analyze*, Node*);
        Node* stripMethodCall(Markup*, Analyze*, Node*);
        Node* stripDecision(Markup*, Analyze*, Node*);
        Node* stripLoop(Markup*, Analyze*, Node*);
        Node* processStatement(Markup*, Analyze*, Node*);
        Node* processBlock(Markup*, Analyze*, Node*); 
        virtual void Execute(Markup*, string*, int);

};

#endif
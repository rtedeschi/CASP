/*  
 *  PrintModule.h
 *  
 *  
 *  Created: 4/2/2017 by Ryan Tedeschi
 */

#ifndef PRINTMODULE_H
#define PRINTTMODULE_H

#include <string>
#include <iostream>
#include "../../shared/CASP_Plugin/CASP_Plugin.h"

using namespace std;

class Tree {
    public:
        string Title;
        string Data;

        vector<Tree*> Children;

        GenericObject* Output();
};

class PrintModule : public CASP_Plugin {
    public:
        PrintModule();

        virtual CASP_Return* Execute(Markup*, LanguageDescriptorObject*, vector<arg>);

    private:
        Tree* GenerateTree(Markup*);
};


#endif
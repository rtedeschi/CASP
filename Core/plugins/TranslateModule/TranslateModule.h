/*  
 *  TranslateModule.h
 *  
 *  
 *  Created: 3/24/2017 by Ryan Tedeschi
 */

#ifndef TRANSLATEMODULE_H
#define TRANSLATEMODULE_H

#include <string>
#include <iostream>
#include "../../shared/CASP_Plugin/CASP_Plugin.h"

using namespace std;

class TranslateModule : public CASP_Plugin {
    public:
        TranslateModule();

        virtual CASP_Return* Execute(Markup*, LanguageDescriptorObject*, vector<arg>);

    private:

        string PrettyPrint(vector<Token>);
        string PrintBlockBody(vector<Token>, int*, int);

        void ReadLanguageFile(string);
        void Translate(Markup*);
        Markup* MatchTargetProd(Markup*);
        Markup* TranslateProd(Markup*, Production*);

        Markup* HandleTerminal(Markup*, ProductionSet*, bool);
        Markup* HandleProduction(Markup*, ProductionSet*, bool);
        Markup* HandleAlternation(Markup*, ProductionSet*);

        LanguageDescriptorObject* target_ldo = NULL;
        LanguageDescriptorObject* source_ldo = NULL;

};

#endif
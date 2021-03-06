/*  
 *  CASP_Plugin.h
 *  Defines the base class for a Plugin Module
 *  
 *  Created: 3/24/2017 by Ryan Tedeschi
 */ 

#ifndef CASP_PLUGIN_H
#define CASP_PLUGIN_H

#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include "../Markup/Markup.h"
#include "../CASP_Return/CASP_Return.h"
#include "../LanguageDescriptor/LanguageDescriptor.h"

using namespace std;

class CASP_Plugin {
    public:
        virtual CASP_Return* Execute(Markup* markup, LanguageDescriptorObject* source_ldo, vector<arg> fnArgs, CASP_Return* inputReturn = NULL) = 0;

        CASP_Return* returnData = NULL;
};

// extern unordered_map<string, CASP_Plugin*> plugins;
string RegisterPlugin(string, CASP_Plugin*);
CASP_Plugin* GetModule(string);
bool ModuleExists(string);

#endif
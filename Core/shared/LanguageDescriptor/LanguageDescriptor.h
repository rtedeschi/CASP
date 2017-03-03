/*  
 *  LanguageDescriptor.h
 *  Defines the Language Descriptor class, which is the bridge between a text language descriptor file
 *      
 *  
 *  Created: 1/3/2017 by Ryan Tedeschi
 */ 

#ifndef LANGUAGE_DESCRIPTOR_H
#define LANGUAGE_DESCRIPTOR_H

#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include "../Helpers/Helpers.h"

#define CFG_EXT ".cfg"
#define CFG_DIR "./cfg/"

using namespace std;

class Production;
class LanguageDescriptorObject;

class LanguageDescriptorObject
{
    public:
        LanguageDescriptorObject(string);
        LanguageDescriptorObject();
        ~LanguageDescriptorObject();

        // void BuildRegex();
        // int[] GetProductions();
        void Parse(string);
        Production* findProdById(string);
        int getProdIndex(string);
        vector<Production*> GetOrderedProductions(vector<string>);
        vector<Production*> GetProductions();

    private:
        vector<Production*> productions;
};

class Production {
    public:
        Production(LanguageDescriptorObject*, string, string);
        void Parse(string, string);

        string GetRegex();
        string GetId();
        vector<Production*> GetContainedProductions();

    private:
        LanguageDescriptorObject* ldo;
        string id;
        string data;
        vector<string> subproductions;
};

#endif
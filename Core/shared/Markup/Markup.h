/*  
 *  Markup.h
 *  Defines the markup class, which is the hierarchical representation of code
 *      
 *  
 *  Created: 1/10/2017 by Ryan Tedeschi
 */ 

#ifndef MARKUP_H
#define MARKUP_H

#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include "../LanguageDescriptor/LanguageDescriptor.h"
#include "../Helpers/Helpers.h"

class Markup 
{
    public:
        Markup();
        Markup(vector<Production*>, string);
        Markup(vector<Production*>, string, Markup*, Production*);
        ~Markup();

        void Parse(vector<Production*>, string);
        int NumChildren() const;
        Markup* ChildAt(int i) const;
        vector<Markup*> Children() const;
        Markup* Parent() const;
        Production* ThisProduction() const;
        string ThisData() const;
        bool IsRoot() const;
        bool IsTerminal() const;

        void Output(int);
    private:
        Markup* parent;
        vector<Markup*> children;
        Production* production;
        string data;
};

#endif
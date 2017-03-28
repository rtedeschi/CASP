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
#include "../Helpers/Helpers.h"

class Markup 
{
    public:
        Markup();
        Markup(string);
        Markup(string, string);
        ~Markup();

        void AddChild(Markup*);
        void AddChildren(vector<Markup*>);
        Markup* ChildAt(int i);

        Markup* Parent();
        int NumChildren();
        string GetData();
        string GetID();
        vector<Markup*> Children();
        bool IsRoot();
        bool IsLeaf();

        void Print();
        void Print(int);

    private:
        Markup* parent;
        vector<Markup*> children;
        string data;
        string id;
};

#endif
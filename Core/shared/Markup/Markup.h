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

class Markup 
{
    public:
        Markup();
        ~Markup();

        int NumChildren() const;
        Markup ChildAt(int i) const;
        Markup Parent() const;
        bool IsRoot() const;
        bool IsTerminal() const;

    private:
        Markup parent;
        vector<Markup> children;
        string symbolName;

};

#endif
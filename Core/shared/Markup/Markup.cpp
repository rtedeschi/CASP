/*  
 *  Markup.cpp
 *  Defines the markup class, which is the hierarchical representation of code
 *      
 *  
 *  Created: 1/10/2017 by Ryan Tedeschi
 */ 

#include "Markup.h"

Markup::Markup() {
    parent = NULL;
    production = NULL;
}
Markup::Markup(vector<Production*> prods, string data) {
    parent = NULL;
    production = NULL;
    this->data = data;
    Parse(prods, data);
}
Markup::Markup(vector<Production*> prods, string data, Markup* parent, Production* prod) {
    this->parent = parent;
    production = prod;
    this->data = data;
    Parse(prods, data);
}
Markup::~Markup() {
    
}

void Markup::Parse(vector<Production*> prods, string data) {
    int numProds = prods.size();
    // cout << "Parse markup tree (" << numProds << "): " << data << endl;

    regex r;
    smatch matches;
    string t = data;
    // cout << endl << t << endl << endl;

    for (int i = 0; i < numProds; i++) {
        r = regex(prods[i]->GetRegex()); 
        
        while (regex_search (t, matches, r)) {
            // cout << "From '" << data << "' -> Found " << prods[i]->GetId() << " '" << matches[0] << "'" << endl;
            vector<Production*> subprods = prods[i]->GetContainedProductions();
            children.push_back(new Markup(subprods, matches[0], this, prods[i]));
            t = matches.prefix().str() + matches.suffix().str();
        }
    }

    // cout << endl << t << endl << endl;
}

int Markup::NumChildren() const {
    return children.size();
}
Markup* Markup::ChildAt(int i) const {
    return children[i];
}
Markup* Markup::Parent() const {
    return parent;
}
Production* Markup::ThisProduction() const {
    return production;
}
vector<Markup*> Markup::Children() const {
    return children;
}

string Markup::ThisData() const {
    return data;
}


bool Markup::IsRoot() const {
    return parent == NULL;
}

bool Markup::IsTerminal() const {
    return children.size() == 0;
}

void Markup::Output(int tabIndex) {
    int i;
    for (i = 0; i < tabIndex; i++) 
        cout << "\t";

    if (!IsRoot())
        cout << production->GetId();
    else
        cout << "Root";
    cout << ": \"" << data << "\"\n";

    for (int i = 0; i < NumChildren(); i++) {
        children[i]->Output(tabIndex + 1);
    }
}
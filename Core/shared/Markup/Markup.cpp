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
}
Markup::Markup(string id, string data) {
    parent = NULL;
    this->data = data;
    this->id = id;
}
Markup::Markup(string id) {
    parent = NULL;
    this->id = id;
}
Markup::~Markup() {
    
}

void Markup::AddChild(Markup* c) {
    c->parent = this;
    c->index = children.size();
    children.push_back(c);
}

void Markup::AddChildren(vector<Markup*> list) {
    for (int i = 0; i < list.size(); i++) {
        Markup* c = list[i];
        c->parent = this;
        c->index = children.size();
        children.push_back(c);
    }
}

int Markup::NumChildren() {
    return children.size();
}
Markup* Markup::ChildAt(int i) {
    if (i >= 0) {
        if (i < children.size())
            return children[i];
    } else {
        if (children.size() + i >= 0);
            return children[children.size() + i];
    }
    return NULL;
}
Markup* Markup::Parent() {
    return parent;
}
vector<Markup*> Markup::Children() {
    return children;
}
string Markup::GetData() {
    if (!IsLeaf()) {
        string d = children[0]->GetData();
        for (int i = 1; i < children.size(); i++) {
            d += " " + children[i]->GetData();
        }
        return d;
    } else {
        return data;
    }
}

vector<Markup*> Markup::RecursiveElements() {

    Markup* rm = this;
    vector<Markup*> recursives;

    while (rm != NULL) {
        recursives.push_back(rm);
        rm = rm->FindFirstChildById(id);
    }

    return recursives;
}

string Markup::GetID() {
    return id;
}

bool Markup::IsRoot() {
    return parent == NULL;
}
bool Markup::IsLeaf() {
    return children.size() == 0;
}

void Markup::Print() {
    Print(0);
}
void Markup::Print(int tabIndex) {
    int i;
    for (i = 0; i < tabIndex; i++) 
        cout << "\t";

    cout << id << ": \"" << GetData() << "\"\n";

    for (int i = 0; i < NumChildren(); i++) {
        children[i]->Print(tabIndex + 1);
    }
}

Markup* Markup::FindFirstById(string id) {
    Markup* result = NULL;
    if (this->id == id) {
        result = this;
    } else {
        for (int i = 0; i < children.size(); i++) {
            if ((result = children[i]->FindFirstById(id)) != NULL)
                break;
        }
    }
    return result;
}
vector<Markup*> Markup::FindAllById(string id, bool findChildrenOfMatches) {
    vector<Markup*> results;

    if (this->id == id) {
        results.push_back(this);
    }

    if (this->id != id || findChildrenOfMatches) {
        for (int i = 0; i < children.size(); i++) {
            vector<Markup*> v = children[i]->FindAllById(id, findChildrenOfMatches);
            results = Helpers::concat(results, v);
        }
    }

    return results;
}

Markup* Markup::FindFirstChildById(string id) {
    Markup* result = NULL;

    for (int i = 0; i < children.size(); i++) {
        if (children[i]->id == id) {
            result = children[i];
            break;
        }
    }
    
    return result;
}
vector<Markup*> Markup::FindAllChildrenById(string id) {
    vector<Markup*> results;

    for (int i = 0; i < children.size(); i++) {
        if (children[i]->id == id)
            results.push_back(children[i]);
    }

    return results;
}

Markup* Markup::FindFirstTerminalByVal(string id, string val) {
    Markup* result = NULL;
    if (this->IsLeaf()) {
        if (this->id == id && this->data == val)
            result = this;
    } else {
        for (int i = 0; i < children.size() && result == NULL; i++) {
            result = children[i]->FindFirstTerminalByVal(id, val);
        }
    }
    return result;
}
Markup* Markup::FindFirstTerminalByVal(string val) {
    Markup* result = NULL;
    if (this->IsLeaf()) {
        if (this->data == val)
            result = this;
    } else {
        for (int i = 0; i < children.size() && result == NULL; i++) {
            result = children[i]->FindFirstTerminalByVal(val);
        }
    }
    return result;
}
vector<Markup*> Markup::FindAllTerminalsByVal(string id, string val) {
    vector<Markup*> results;
    if (this->IsLeaf()) {
        if (this->id == id && this->data == val)
            results.push_back(this);
    } else {
        for (int i = 0; i < children.size(); i++) {
            vector<Markup*> v = children[i]->FindAllTerminalsByVal(val);
            results = Helpers::concat(results, v);
        }
    }
    return results;
}
vector<Markup*> Markup::FindAllTerminalsByVal(string val) {
    vector<Markup*> results;
    if (this->IsLeaf()) {
        if (this->data == val)
            results.push_back(this);
    } else {
        for (int i = 0; i < children.size(); i++) {
            vector<Markup*> v = children[i]->FindAllTerminalsByVal(val);
            results = Helpers::concat(results, v);
        }
    }
    return results;
}

Markup* Markup::FindAncestorById(string id) {
    Markup* result = NULL;
    if (parent != NULL) {
        if (parent->id == id)
            result = parent;
        else
            result = parent->FindAncestorById(id);
    }
    return result;
}

unordered_map<string, string> Markup::AccessibleDeclarations() {
    unordered_map<string, string> declarations;

    // try to get any global declarations
    Markup* statementAncestor = FindAncestorById("statement");
    if (statementAncestor != NULL) {
        unordered_map<string, string> parentDecl = statementAncestor->AccessibleDeclarations();
        for ( auto it = parentDecl.begin(); it != parentDecl.end(); ++it )
            declarations[it->first] = it->second;
    }
    
    Markup* fnAncestor = FindAncestorById("function-definition");
    if (fnAncestor != NULL) {
        unordered_map<string, string> parentDecl = fnAncestor->AccessibleDeclarations();
        for ( auto it = parentDecl.begin(); it != parentDecl.end(); ++it )
            declarations[it->first] = it->second;
    }

    // try to get previous sibling declarations
    if (parent != NULL) {
        for (int i = 0; i < index; i++) {
            unordered_map<string, string> sibDecl = parent->ChildAt(i)->AccessibleDeclarations();
            for ( auto it = sibDecl.begin(); it != sibDecl.end(); ++it )
                declarations[it->first] = it->second;
        }
    }

    // try to get previous statement declarations
    if (parent != NULL && id == "statement") {
        vector<Markup*> s;
        Markup* sl = parent;
        while ((sl = sl->parent) != NULL && sl->GetID() == "statement-list") {
            s.insert(s.begin(), sl->FindFirstChildById("statement"));
        }
        for (int i = 0; i < s.size(); i++) {
            unordered_map<string, string> sibDecl = s[i]->AccessibleDeclarations();
            for ( auto it = sibDecl.begin(); it != sibDecl.end(); ++it )
                declarations[it->first] = it->second;
        }
    }

    // add any local declarations
    for ( auto it = localDeclarations.begin(); it != localDeclarations.end(); ++it )
        declarations[it->first] = it->second;

    return declarations;
}

unordered_map<string, Markup*> Markup::AccessibleValues() {
    unordered_map<string, Markup*> values;

    // cout << "Getting accessible values on " << GetData() << " (" << GetID() << ")" << endl;

    // try to get any global values
    Markup* statementAncestor = FindAncestorById("statement");
    if (statementAncestor != NULL) {
        unordered_map<string, Markup*> parentDecl = statementAncestor->AccessibleValues();
        for ( auto it = parentDecl.begin(); it != parentDecl.end(); ++it )
            values[it->first] = it->second;
    }

    Markup* fnAncestor = FindAncestorById("function-definition");
    if (fnAncestor != NULL) {
        unordered_map<string, Markup*> parentDecl = fnAncestor->AccessibleValues();
        for ( auto it = parentDecl.begin(); it != parentDecl.end(); ++it )
            values[it->first] = it->second;
    }

    // try to get previous sibling values
    if (parent != NULL) {
        for (int i = 0; i < index; i++) {
            unordered_map<string, Markup*> sibDecl = parent->ChildAt(i)->AccessibleValues();
            for ( auto it = sibDecl.begin(); it != sibDecl.end(); ++it )
                values[it->first] = it->second;
        }
    }

    // try to get previous statement values
    if (parent != NULL && id == "statement") {
        vector<Markup*> s;
        Markup* sl = parent;
        while ((sl = sl->parent) != NULL && sl->GetID() == "statement-list") {
            s.insert(s.begin(), sl->FindFirstChildById("statement"));
        }
        for (int i = 0; i < s.size(); i++) {
            unordered_map<string, Markup*> sibDecl = s[i]->AccessibleValues();
            for ( auto it = sibDecl.begin(); it != sibDecl.end(); ++it )
                values[it->first] = it->second;
        }
    }

    // add any local values
    for ( auto it = localValues.begin(); it != localValues.end(); ++it )
        values[it->first] = it->second;

    return values;
}

int Markup::IndexInParent() {
    return index;
}

Markup* Markup::Clone() {
    Markup* m = new Markup(id);
    m->localDeclarations = localDeclarations;
    m->localValues = localValues;

    if (IsLeaf()) {
        m->data = data;
    } else {
        for (int i = 0; i < children.size(); i++) {
            m->AddChild(children[i]->Clone());
        }
    }
    return m;
}
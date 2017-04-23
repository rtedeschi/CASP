/*  
 *  Markup.h
 *  Defines the markup class, which represents a parse tree of the code
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
#include "../Printable/Printable.h"

/*
    The Markup class is used to represent the parse tree of a particular snippet of code.
*/
class Markup : public Printable
{
    public:
        /*
            Creates a Markup object with no ID or Data
        */
        Markup();
        /*
            Creates a Markup object with only an ID. This is meant for production nodes
            id - accociated production/terminal title
        */
        Markup(string id);
        /*
            Creates a Markup object with both an ID and Data. This is meant for terminal nodes (leaves)
            id - associated production/terminal title
            data - code associated with the node
        */
        Markup(string id, string data);
        /*
            Destructor (UNIMPLEMENTED)
        */
        ~Markup();

        /*
            Adds a child to the end of the markup list
            c - child to add
        */
        void AddChild(Markup* c);
        /*
            Concatenates a vector of children to the end of thet markup list
            list - vector of children to add
        */
        void AddChildren(vector<Markup*> list);
        /*
            Retrieves the child at the specified index.
            i - if non-negative, indexes from the front of the child array. If negative, indexes from the back of the child array
        */
        Markup* ChildAt(int i);
        /*
            Retrieves a vector containing recursive productions matching the current ID.
        */
        vector<Markup*> RecursiveElements();

        /*
            Finds the first matching child by ID, null if no match
            id - ID to match
        */
        Markup* FindFirstChildById(string id);
        /*
            Finds the first matching node in self or any descendants by ID, null if no match
            id - ID to match
        */
        Markup* FindFirstById(string id);
        /*
            Finds all matching children by ID
            id - ID to match
        */
        vector<Markup*> FindAllChildrenById(string id);
        /*
            Finds all matching self or descendants by ID
            id - ID to match
            findChildrenOfMatches - if true, continues searching inside matching nodes
        */
        vector<Markup*> FindAllById(string id, bool findChildrenOfMatches);

        /*
            Finds the first matching terminal by value, null if no match
            id - optional terminal ID
            val - value to match
        */
        Markup* FindFirstTerminalByVal(string id, string val);
        Markup* FindFirstTerminalByVal(string val);
        /*
            Finds all matching terminals by value
            id - optional terminal ID
            val - value to match
        */
        vector<Markup*> FindAllTerminalsByVal(string id, string val);
        vector<Markup*> FindAllTerminalsByVal(string val);
        /*
            Finds the first matching ancestor by ID, NULL if no match
            id - ID to match
        */
        Markup* FindAncestorById(string id);

        /*
            Retrieves the parent of the node (NULL if none)
        */
        Markup* Parent();
        /*
            Retrieves the number of children of the node
        */
        int NumChildren();
        /*
            Retrieves the associated code of the node. If the node is a leaf, 
            this returns the string data. Otherwise, this collects all leaf data and returns it
        */
        string GetData();
        /*
            Retrieves the associated production/terminal ID
        */
        string GetID();
        /*
            Retrieves the vector of children
        */
        vector<Markup*> Children();
        /*
            Returns if this node is a root (no parent)
        */
        bool IsRoot();
        /*
            Returns if this node is a leaf (no children)
        */
        bool IsLeaf();

        /*
            Prints the node out
        */
        void Print();
        /*
            Prints the node out at a specific tab indent
        */
        void Print(int tabIndex);
        /*
            Gets the index of the node in its parent
        */
        int IndexInParent();
        /*
            Gets all accessible variable declarations to the node
        */
        unordered_map<string, string> AccessibleDeclarations();
        /*
            Gets all accessible variable declarations to the node
        */
        unordered_map<string, Markup*> AccessibleValues();

        unordered_map<string, string> localDeclarations;
        unordered_map<string, Markup*> localValues;

    private:
        // Parent of the node
        Markup* parent;
        // list of children of the node
        vector<Markup*> children;
        // code data - only used in leaf nodes
        string data;
        // production/terminal ID
        string id;
        // index in parent
        int index = 0;
};

#endif
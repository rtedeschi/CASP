/*  
 *  OutlineModule.h
 *  
 *  
 *  Created: 3/24/2017 by Ryan Tedeschi
 */

#ifndef OUTLINEMODULE_H
#define OUTLINEMODULE_H

#include <string>
#include <iostream>
#include <vector>
#include "../../shared/CASP_Plugin/CASP_Plugin.h"
#include "../../shared/Printable/Printable.h"

using namespace std;

enum EntryType { Start, MethodCall, Process, Loop, Decision, IO, End };
class OutlineModule;
class Outline;
class Node;
class Edge;

class OutlineModule : public CASP_Plugin {
    public:
        OutlineModule();
        virtual void Execute(Markup*, string*, int);

    private:
        vector<Outline*> GetAllOutlines(Markup*);
        Outline* GetRootOutline(vector<Markup*>);
        Outline* GetFunctionOutline(Markup*);
        void FormatData(vector<Outline*>);

        Node* stripProcess(Markup*, Outline*, Node*);
        Node* stripMethodCall(Markup*, Outline*, Node*);
        Node* stripDecision(Markup*, Outline*, Node*);
        Node* stripLoop(Markup*, Outline*, Node*);
        Node* processStatement(Markup*, Outline*, Node*);
        Node* processBlock(Markup*, Outline*, Node*);
};

class Outline : public Printable {
    public:
        Outline();

        Node* AppendBlock(EntryType, string, Node*);
        void Print();

    private:
        int maxId = 0;
        Node* head = NULL;
};

class Node : public Printable {
    public:
        Node(string, EntryType, int);

        Edge* AddEdgeTo(Node*);
        Edge* AddEdgeTo(Node*, string);
        Edge* AddEdgeFrom(Node*);
        Edge* AddEdgeFrom(Node*, string);
        void Print();

        int id;
        string data;
        EntryType type;

    private:
        vector<Edge*> edges;

};

class Edge : public Printable {
    public:
        Edge(Node*, Node*, string);
        Edge(Node*, Node*);
        void Print();

        string data = "";
        Node* source = NULL;
        Node* target = NULL;
        
    private:
};

#endif
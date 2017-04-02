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

class Outline {
    public:
        Outline();

        Node* AppendBlock(EntryType, string, Node*);

    private:
        int maxId = 0;
        Node* head = NULL;
};

class Node {
    public:
        Node(string, EntryType, int);

        Edge* AddEdgeTo(Node*);
        Edge* AddEdgeTo(Node*, string);
        Edge* AddEdgeFrom(Node*);
        Edge* AddEdgeFrom(Node*, string);

        int id;
        string data;
        EntryType type;

    private:
        vector<Edge*> edges;

};

class Edge {
    public:
        Edge(Node*, Node*, string);
        Edge(Node*, Node*);
        
    private:
        string data = "";
        Node* source = NULL;
        Node* target = NULL;
};

#endif
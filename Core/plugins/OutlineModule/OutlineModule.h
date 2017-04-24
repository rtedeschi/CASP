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

enum EntryType { Start, MethodCall, Process, Loop, Decision, EndDecision, IO, End };
class OutlineModule;
class Outline;
class Node;
class Edge;

class OutlineModule : public CASP_Plugin {
    public:
        OutlineModule();
        virtual CASP_Return* Execute(Markup* markup, LanguageDescriptorObject* source_ldo, vector<arg> fnArgs, CASP_Return* inputReturn = NULL);

    private:
        vector<Outline*> GetAllOutlines(Markup*);
        Outline* GetRootOutline(vector<Markup*>);
        Outline* GetFunctionOutline(Markup*);
        CASP_Return* FormatData(vector<Outline*>);

        Node* stripProcess(Markup*, Outline*, Node*, string = "");
        Node* stripMethodCall(Markup*, Outline*, Node*, string = "");
        Node* stripDecision(Markup*, Outline*, Node*, string = "");
        Node* stripFor(Markup*, Outline*, Node*, string = "");
        Node* stripWhile(Markup*, Outline*, Node*, string = "");
        Node* processStatement(Markup*, Outline*, Node*, string = "");
        Node* processBlock(Markup*, Outline*, Node*, string = "");
};

class Outline : public Printable {
    public:
        Outline();

        Node* AppendBlock(EntryType, string, Node*);
        Node* AppendBlock(EntryType, string, Node*, string);
        Node* AppendBlock(Node*);
        void Print();

        GenericArray* Output();

    private:
        vector<Node*> nodes;
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

        GenericObject* Output();        

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

        GenericObject* Output();        

        string data = "";
        Node* source = NULL;
        Node* target = NULL;
        
    private:
};

#endif
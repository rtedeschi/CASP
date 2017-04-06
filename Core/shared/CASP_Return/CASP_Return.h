/*  
 *  CASP_Return.h
 *  Defines a generic return object for the program
 *  
 *  Created: 4/4/2017 by Ryan Tedeschi
 */ 

#ifndef CASP_RETURN_H
#define CASP_RETURN_H

#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include "../Printable/Printable.h"

using namespace std;

class GenericData : public Printable {
    public:
        string type = "";
        virtual void Print();
};

template<typename T>
class GenericLeaf : public GenericData {
    public:
        string type = "Leaf";
        GenericLeaf(T data) {
            this->data = data;
        };

        virtual void Print() {
            cout << data;
        };

        void Assign(T data) {
            this->data = data;
        };

    protected:
        T data;
};

class GenericObject : public GenericData {
    public:
        string type = "Object";
        virtual void Print();
        void Add(string, GenericData*);
        GenericData* At(string);

    protected:
        unordered_map<string, GenericData*> data;

};

class GenericArray : public GenericData {
    public:
        string type = "Array";
        virtual void Print();
        void Add(GenericData*);
        GenericData* At(int);

    
    protected:
        vector<GenericData*> data;
};

class CASP_Return : public GenericObject {
    public:
        CASP_Return();

        GenericObject* Errors();
        GenericObject* Warnings();
        GenericObject* Data();
    
    private:
};


#endif
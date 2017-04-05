/*  
 *  CASP_Return.h
 *  Defines a generic return object for the program
 *  
 *  Created: 4/4/2017 by Ryan Tedeschi
 */ 

#ifndef CASP_RETURN_H
#define CASP_RETURN_H

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include "../Printable/Printable.h"

using namespace std;

class GenericData : public Printable {
    public:
        string type = "";
        virtual void Print() {};
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
        virtual void Print() {
            int count = 0;
            cout << "{";
            for (auto it = data.begin(); it != data.end(); ++it ) {
                if (count++ > 0) {
                    cout << ",";
                }
                cout << "\"" << it->first << "\":";
                if (it->second != NULL)
                    it->second->Print();
                else
                    cout << "null";
            }
            cout << "}";
        };

        void Add(string key, GenericData* d) {
            data[key] = d;
        };

        GenericData* At(string key) {
            return data[key];
        };

    protected:
        unordered_map<string, GenericData*> data;

};

class GenericArray : public GenericData {
    public:
        string type = "Array";
        virtual void Print() {
            cout << "[";
            for (int i = 0; i < data.size(); i++) {
                if (i > 0)
                    cout << ",";
                data[i]->Print();
            }
            cout << "]";
        };

        void Add(GenericData* d) {
            data.push_back(d);
        };

        GenericData* At(int index) {
            return data[index];
        }
    
    protected:
        vector<GenericData*> data;
};

class CASP_Return : public GenericObject {
    public:
        CASP_Return() {
            Add("Data", new GenericObject());
            Add("Warnings", new GenericObject());
            Add("Errors", new GenericObject());
        };

        GenericObject* Errors() {
            return (GenericObject*)data["Errors"];
        };
        GenericObject* Warnings() {
            return (GenericObject*)data["Warnings"];
        };
        GenericObject* Data() {
            return (GenericObject*)data["Data"];
        };
    
    private:
};


#endif
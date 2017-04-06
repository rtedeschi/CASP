#include "CASP_Return.h"

void GenericData::Print() {}

void GenericObject::Print() {
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
}

void GenericObject::Add(string key, GenericData* d) {
    data[key] = d;
}

GenericData* GenericObject::At(string key) {
    return data[key];
}

void GenericArray::Print() {
    cout << "[";
    for (int i = 0; i < data.size(); i++) {
        if (i > 0)
            cout << ",";
        data[i]->Print();
    }
    cout << "]";
}

void GenericArray::Add(GenericData* d) {
    data.push_back(d);
}

GenericData* GenericArray::At(int index) {
    return data[index];
}
    
CASP_Return::CASP_Return() {
    Add("Data", new GenericObject());
    Add("Warnings", new GenericObject());
    Add("Errors", new GenericObject());
}

GenericObject* CASP_Return::Errors() {
    return (GenericObject*)data["Errors"];
}
GenericObject* CASP_Return::Warnings() {
    return (GenericObject*)data["Warnings"];
}
GenericObject* CASP_Return::Data() {
    return (GenericObject*)data["Data"];
}

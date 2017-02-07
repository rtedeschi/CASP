/*  
 *  LanguageDescriptor.h
 *  Defines the Language Descriptor class, which is the bridge between a text language descriptor file
 *      
 *  
 *  Created: 1/3/2017 by Ryan Tedeschi
 */ 

#include "LanguageDescriptor.h"

using namespace std;

LanguageDescriptorObject::LanguageDescriptorObject() {

}

LanguageDescriptorObject::LanguageDescriptorObject(string file) {

}

LanguageDescriptorObject::~LanguageDescriptorObject() {

}

void LanguageDescriptorObject::Parse(string file) {
    cout << file << "\n\n";
    // regex Regex = new regex("(.?) =:  (.+?)\n\n", regex_constants.ECMAScript);
    
}
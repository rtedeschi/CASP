/*  
 *  LanguageDescriptor.h
 *  Defines the Language Descriptor class, which is the bridge between a text language descriptor file
 *      
 *  
 *  Created: 1/3/2017 by Ryan Tedeschi
 */ 

 #ifndef LANGUAGE_DESCRIPTOR_H
 #define LANGUAGE_DESCRIPTOR_H

 #include <string>
 #include <regex>
 #include <iostream>

 using namespace std;

class LanguageDescriptorObject
{
    public:
        LanguageDescriptorObject();
        LanguageDescriptorObject(string);
        ~LanguageDescriptorObject();

        // void BuildRegex();
        // int[] GetProductions();
        void Parse(string);

    private:
        int* Productions;
};

#endif
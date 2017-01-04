/*  
 *  ControlModule.h
 *  Defines the Control Module class, which handles the flow of data between the consumer
 *      and the action modules
 *  
 *  Created: 1/3/2017 by Ryan Tedeschi
 */ 

#ifndef CONTROLMODULE_H
#define CONTROLMODULE_H

#include <string>
#include "ControlModule.cpp"


#define LANGUAGE_DESCRIPTOR_OBJECT void // Placeholder for the formatted results type
#define FORMATTED_RESULTS void // Placeholder for the formatted results type

using namespace std;

class ControlModule
{
    public:
        //(string sourceLanguage, string moduleID, string[] codeSnippets, string[] moduleArgs)
        FORMATTED_RESULTS Run(string, string, string[], string[]);

    private:
        //(string sourceLanguage)
        LANGUAGE_DESCRIPTOR_OBJECT GetLanguageDescriptor(string);

};

#endif
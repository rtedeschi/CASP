/*  
 *  ControlModule.h
 *  Defines the Control Module class, which handles the flow of data between the consumer
 *      and the action modules
 *  
 *  Created: 1/3/2017 by Ryan Tedeschi
 */ 

#ifndef CONTROLMODULE_H
#define CONTROLMODULE_H

/*
 *  Control Module Input parameter type placeholders
 */
#define SOURCE_LANGUAGE string
#define MODULE_ID std::string
#define CODE_COUNT int
#define CODE_INPUT std::string*
#define ARG_COUNT int
#define FUNCTION_ARGS std::string*
/*
 *  Control Module Output parameter type placeholders
 */
#define FORMATTED_RESULTS void*
/*
 *  Control Module Internal parameter type placeholders
 */
#define MODULE_RESPONSE void*
#define LANGUAGE_DESCRIPTOR_OBJECT void*
#define MARKUP_OBJECT void*
#define CODE_OUTPUT std::string*
#define MODULE_REF void*

#include <string>
#include "../shared/LanguageDescriptor/LanguageDescriptor.h"

using namespace std;

class ControlModule {
    public:
        ControlModule();
        ~ControlModule();
        FORMATTED_RESULTS Run(SOURCE_LANGUAGE, MODULE_ID, CODE_COUNT, CODE_INPUT, ARG_COUNT, FUNCTION_ARGS);

    private:
        LANGUAGE_DESCRIPTOR_OBJECT GetLanguageDescriptor(SOURCE_LANGUAGE) throw (std::string);
        bool ValidateSourceLanguage(SOURCE_LANGUAGE);
        LANGUAGE_DESCRIPTOR_OBJECT ReadLanguageFile(SOURCE_LANGUAGE) throw (std::string);
        CODE_OUTPUT CoalesceCode(CODE_INPUT); // is this necessary?..
        MARKUP_OBJECT Parse(CODE_OUTPUT, LANGUAGE_DESCRIPTOR_OBJECT);
        FORMATTED_RESULTS Execute(MARKUP_OBJECT, MODULE_ID, ARG_COUNT, FUNCTION_ARGS);
        MODULE_REF ModuleRetrieval(MODULE_ID);
        MODULE_RESPONSE ModuleExecution(MODULE_REF, MARKUP_OBJECT, ARG_COUNT, FUNCTION_ARGS);
        FORMATTED_RESULTS FormatOutput(MODULE_RESPONSE);
};

#endif
/*  
 *  ControlModule.h
 *  Defines the Control Module class, which handles the flow of data between the consumer
 *      and the action modules
 *  
 *  Created: 1/3/2017 by Ryan Tedeschi
 */ 

#ifndef CONTROLMODULE_H
#define CONTROLMODULE_H

/*  These are all placeholders. 
 *  However, they can be left in for ease of modification since related artifacts are all of these types
 */
#define MODULE_RESPONSE void* // Placeholder for the formatted results type
#define LANGUAGE_DESCRIPTOR_OBJECT void* // Placeholder for the formatted results type
#define FORMATTED_RESULTS void* // Placeholder for the formatted results type
#define MARKUP_OBJECT void* // Placeholder for the formatted results type
#define MODULE_ID std::string* // Placeholder for the formatted results type
#define SOURCE_LANGUAGE string* // Placeholder for the formatted results type
#define CODE_INPUT std::string** // Placeholder for the formatted results type
#define CODE_OUTPUT std::string* // Placeholder for the formatted results type
#define FUNCTION_ARGS std::string** // Placeholder for the formatted results type
#define MODULE_REF void* // Placeholder for the formatted results type

#include <string>

using namespace std;

class ControlModule {
    public:
        ControlModule();
        ~ControlModule();
        FORMATTED_RESULTS Run(SOURCE_LANGUAGE, MODULE_ID, CODE_INPUT, FUNCTION_ARGS);

    private:
        LANGUAGE_DESCRIPTOR_OBJECT GetLanguageDescriptor(SOURCE_LANGUAGE) throw (std::string);
        bool ValidateSourceLanguage(SOURCE_LANGUAGE); // is this even necessary?...
        LANGUAGE_DESCRIPTOR_OBJECT ReadLanguageFile(SOURCE_LANGUAGE) throw (std::string);
        CODE_OUTPUT CoalesceCode(CODE_INPUT); // is this necessary?..
        MARKUP_OBJECT Parse(CODE_OUTPUT, LANGUAGE_DESCRIPTOR_OBJECT);
        FORMATTED_RESULTS Execute(MARKUP_OBJECT, MODULE_ID, FUNCTION_ARGS);
        MODULE_REF ModuleRetrieval(MODULE_ID);
        MODULE_RESPONSE ModuleExecution(MODULE_REF, MARKUP_OBJECT, FUNCTION_ARGS);
        FORMATTED_RESULTS FormatOutput(MODULE_RESPONSE);
};

#endif
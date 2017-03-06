/*  
 *  ControlModule.cpp
 *  Defines the Control Module class, which handles the flow of data between the consumer
 *      and the action modules
 *  
 *  Created: 1/3/2017 by Ryan Tedeschi
 */ 

#include "ControlModule.h"

ControlModule::ControlModule() {

}

ControlModule::~ControlModule() {
    
}

FORMATTED_RESULTS ControlModule::Run(SOURCE_LANGUAGE sourceLanguage, MODULE_ID moduleID, CODE_COUNT codeCount, CODE_INPUT codeSnippets, ARG_COUNT argCount, FUNCTION_ARGS functionArgs) {

    LANGUAGE_DESCRIPTOR_OBJECT descriptor = NULL;
    CODE_OUTPUT code = NULL;
    MARKUP_OBJECT markup = NULL;
    FORMATTED_RESULTS results = NULL;

    try {
        descriptor = GetLanguageDescriptor(sourceLanguage);
        code = CoalesceCode(codeSnippets);
        // TODO temporary passthrough
        code = codeSnippets;
        markup = Parse(code, descriptor);
    } catch (std::string message) {
        // do something with the message here and don't continue with the execution

    }

    results = Execute(markup, moduleID, argCount, functionArgs);

    return results;
}

LANGUAGE_DESCRIPTOR_OBJECT ControlModule::GetLanguageDescriptor(SOURCE_LANGUAGE sourceLanguage) throw (std::string) {
    if (ValidateSourceLanguage(sourceLanguage))
        return ReadLanguageFile(sourceLanguage);
    throw "Language 'sourceLanguage' has not been defined"; // TODO change this
}

bool ControlModule::ValidateSourceLanguage(SOURCE_LANGUAGE sourceLanguage) {
    return true;
}

LANGUAGE_DESCRIPTOR_OBJECT ControlModule::ReadLanguageFile(SOURCE_LANGUAGE sourceLanguage) throw (std::string) {
    LANGUAGE_DESCRIPTOR_OBJECT languageDescriptor;
    try {
        // read and parse file;
        languageDescriptor = new LanguageDescriptorObject(sourceLanguage);
        languageDescriptor->Parse_Special(sourceLanguage);
    } catch (...) {
        
        throw "Language 'sourceLanguage' could not be read"; // TODO change this
    }

    return languageDescriptor;
}

CODE_OUTPUT ControlModule::CoalesceCode(CODE_INPUT codeSnippets) {
    CODE_OUTPUT code = NULL;
    
    // do some iterations over codeSnippets to unify it 

    return code;
}

MARKUP_OBJECT ControlModule::Parse(CODE_OUTPUT code, LANGUAGE_DESCRIPTOR_OBJECT languageDescriptor) {
    MARKUP_OBJECT markup = NULL;

    languageDescriptor->Tokenize(code[0]);

    vector<Production*> prods = languageDescriptor->GetProductions();
    markup = new Markup(prods, code[0]); // code[0] is temp
    
    markup->Output(0);

    // do some gross manipulations to change the input into a markup object

    return markup;
}

FORMATTED_RESULTS ControlModule::Execute(MARKUP_OBJECT markup, MODULE_ID moduleID, ARG_COUNT argCount, FUNCTION_ARGS functionArgs) {
    MODULE_REF ref = ModuleRetrieval(moduleID);
    MODULE_RESPONSE response = ModuleExecution(ref, markup, argCount, functionArgs);
    FORMATTED_RESULTS results = FormatOutput(response);
    return results;
}

MODULE_REF ControlModule::ModuleRetrieval(MODULE_ID moduleID) {
    MODULE_REF ref = NULL;

    // get module ref here;

    return ref;
}

MODULE_RESPONSE ControlModule::ModuleExecution(MODULE_REF moduleRef, MARKUP_OBJECT markup, ARG_COUNT argCount, FUNCTION_ARGS functionArgs) {
    MODULE_RESPONSE response = NULL;

    try {
        // attempt to execute the module
    } catch (...) {
        // an error occurred, put it in the response object
    }

    return response;
}

FORMATTED_RESULTS ControlModule::FormatOutput(MODULE_RESPONSE moduleResponse) {
    FORMATTED_RESULTS results = NULL;

    // do some conversion from moduleResponse to formattedResults

    return results;
}
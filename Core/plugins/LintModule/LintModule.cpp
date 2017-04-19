#include "LintModule.h"

static string _LintModule = RegisterPlugin("Lint", new LintModule());

LintModule::LintModule() {}

CASP_Return* LintModule::Execute(Markup* markup, LanguageDescriptorObject* source_ldo, vector<arg> fnArgs, CASP_Return* inputReturn) {
    returnData = (inputReturn != NULL ? inputReturn : new CASP_Return());

    /*
        This module hasn't implemented any Function Args yet!
        Use Helpers::ParseArrayArgument() and Helpers::ParseArgument() to scrape out arguments
    */

    cout << "This is the entry point for the " << _LintModule << " Module!\n";

    return NULL;

}
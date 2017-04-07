#include "TranslateModule.h"

static string _TranslateModule = RegisterPlugin("Translate", new TranslateModule());

TranslateModule::TranslateModule() {}

CASP_Return* TranslateModule::Execute(Markup* markup, LanguageDescriptorObject* source_ldo, vector<arg> fnArgs) {

    /*
        This module hasn't implemented any Function Args yet!
        Use Helpers::ParseArrayArgument() and Helpers::ParseArgument() to scrape out arguments
    */

    cout << "This is the entry point for the " << _TranslateModule << " Module!\n";

    return NULL;

}
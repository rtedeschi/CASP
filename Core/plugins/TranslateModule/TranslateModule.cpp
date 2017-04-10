#include "TranslateModule.h"

static string _TranslateModule = RegisterPlugin("Translate", new TranslateModule());

TranslateModule::TranslateModule() {}

CASP_Return* TranslateModule::Execute(Markup* markup, LanguageDescriptorObject* source_ldo, vector<arg> fnArgs) {

    cout << "This is the entry point for the " << _TranslateModule << " Module!\n";

    string targetLanguage = Helpers::ParseArgument("targetlang", fnArgs);
    ReadLanguageFile(targetLanguage);

    return NULL;

}

void TranslateModule::ReadLanguageFile(string targetLanguage) {
    try {
        // read and parse file;
        target_ldo = new LanguageDescriptorObject(targetLanguage);
    } catch (...) {
        string err = "Language '" + targetLanguage + "' could not be read";
        cout << err << endl;
        throw err;
    }
}
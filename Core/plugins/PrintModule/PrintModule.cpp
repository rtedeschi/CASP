#include "PrintModule.h"

static string _PrintModule = RegisterPlugin("Print", new PrintModule());

PrintModule::PrintModule() {}

CASP_Return* PrintModule::Execute(Markup* markup, LanguageDescriptorObject* source_ldo, vector<arg> fnArgs) {

    markup->Print();

    return NULL;

}
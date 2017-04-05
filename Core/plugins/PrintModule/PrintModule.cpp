#include "PrintModule.h"

static string _PrintModule = RegisterPlugin("Print", new PrintModule());

PrintModule::PrintModule() {}

CASP_Return* PrintModule::Execute(Markup* markup, string* fnArgs, int fnArgCount) {

    markup->Print();

    return NULL;

}
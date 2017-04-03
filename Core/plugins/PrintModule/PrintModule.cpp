#include "PrintModule.h"

static string _PrintModule = RegisterPlugin("Print", new PrintModule());

PrintModule::PrintModule() {}

void PrintModule::Execute(Markup* markup, string* fnArgs, int fnArgCount) {

    markup->Print();

}
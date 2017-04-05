#include "LintModule.h"

static string _LintModule = RegisterPlugin("Lint", new LintModule());

LintModule::LintModule() {}

CASP_Return* LintModule::Execute(Markup* markup, string* fnArgs, int fnArgCount) {

    /*
        This module hasn't implemented any Function Args yet!
        Use Helpers::ParseArrayArgument() and Helpers::ParseArgument() to scrape out arguments
    */

    cout << "This is the entry point for the " << _LintModule << " Module!\n";

    return NULL;

}
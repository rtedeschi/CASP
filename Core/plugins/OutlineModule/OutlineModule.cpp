#include "OutlineModule.h"

static string _OutlineModule = RegisterPlugin("Outline", new OutlineModule());

OutlineModule::OutlineModule() {}

void OutlineModule::Execute(Markup* markup, string* fnArgs, int fnArgCount) {

    /*
        This module hasn't implemented any Function Args yet!
        Use Helpers::ParseArrayArgument() and Helpers::ParseArgument() to scrape out arguments
    */

    cout << "This is the entry point for the " << _OutlineModule << " Module!\n";

}
/*  
 *  Main.cpp
 *  Initiates the program
 *  
 *  Created: 1/3/2017 by Ryan Tedeschi
 */ 

#include "ControlModule.h"
#include "Timestamp.h"
#include <string>

using namespace std;

int main(int argCount, char** argArray)
{
    printf("Code Analyzer Software Package (CASP)\n");
    printf("Current as of ");
    printf(TIMESTAMP);
    printf("\n");

    ControlModule* control = new ControlModule();
    printf("Hello, Control Module!");
    // control.Run();
}
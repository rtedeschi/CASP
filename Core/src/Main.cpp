/*  
 *  Main.cpp
 *  Initiates the program
 *  
 *  Created: 1/3/2017 by Ryan Tedeschi
 */ 

#include "ControlModule.h"
#include "Timestamp.h"
#include <iostream>
#include <string>

#define OTHER 0
#define FN_ARG 1
#define MODULE 2
#define LANGUAGE 3
int SIZES[] { 0, 6, 10, 12 };

int paramType(string);
string parseParam(string, int);
string unescapeCharacters(string);

int main(int argCount, char** argArray)
{
    cout << "Code Analyzer Software Package (CASP)\n";
    cout << "Current as of " << TIMESTAMP << "\n";

    string* fnArgs = NULL;
    string* codeSource = NULL;
    string sourceLanguage;
    string moduleID;
    int fnArgCount = 0;

    for (int i = 1; i < argCount; i++) 
    {
        string arg(argArray[i]);
        int type = paramType(argArray[i]);
        string value = parseParam(argArray[i], type);
        // cout << "\n" << value;
        if (type == FN_ARG) 
        {
            // TODO need to 
            fnArgCount++;
        }
        else if (type == MODULE)
        {
            moduleID = value;
        }
        else if (type == LANGUAGE)
        {
            sourceLanguage = value;
        }
        // else if (type == CODE_SOURCE)
        // {

        // }
        else
        {
            cout << "\nIgnoring unexpected parameter '" << value << "'";
        }
    }

    ControlModule control = ControlModule();
    control.Run(sourceLanguage, moduleID, codeSource, fnArgCount, fnArgs);
}

int paramType(string input) {
    int type = OTHER;
    if (input.find("/sourcelang=") == 0)
        type = LANGUAGE;
    else if (input.find("/moduleid=") == 0)
        type = MODULE;
    else if (input.find("/args=") == 0)
        type = FN_ARG;
    return type;
}

string parseParam(string input, int type) 
{
    int size = SIZES[type];
    string out = input.substr(size, input.size() - size);
    if (type == FN_ARG)
        out.assign(unescapeCharacters(out));
    return out;
}

string unescapeCharacters(string in)
{
    string out(in);
    bool escaped = false;
    for (int i = 0; i < out.size(); i++) 
    {
        if (out[i] == '\"')
        {
            if (!escaped)
                out.erase(i--, 1);
            escaped = false;
        }
        else if (out[i] == '\\')
        {
            if (!escaped)
                out.erase(i--, 1);
            escaped = !escaped;
        }
        else
            escaped = false;
    }
    return out;
}
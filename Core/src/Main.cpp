/*  
 *  Main.cpp
 *  Initiates the program
 *  
 *  Created: 1/3/2017 by Ryan Tedeschi
 */ 

#include "../shared/Helpers/Helpers.h"
#include "ControlModule.h"
#include "Timestamp.h"
#include <iostream>
#include <string>
#include <list>

#define OTHER 0
#define FN_ARG 1
#define MODULE 2
#define LANGUAGE 3
#define CODE 4
#define CODE_F 5
int SIZES[] { 0, 6, 10, 12, 6, 7 };

int paramType(string);
string parseParam(string, int);
string unescapeCharacters(string);

int main(int argCount, char** argArray)
{
    cout << "Code Analyzer Software Package (CASP)" << endl;
    cout << "Build Date: " << TIMESTAMP << endl;

    list<string> fnArgs;
    list<string> codeSource;
    string* codeArray = NULL;
    string* fnArgsArray = NULL;
    string sourceLanguage;
    string moduleID;

    for (int i = 1; i < argCount; i++) 
    {
        string arg(argArray[i]);
        int type = paramType(argArray[i]);
        string value = parseParam(argArray[i], type);
        switch (type) {
            case FN_ARG:
                fnArgs.push_back(value);
                break;
            case MODULE:
                moduleID = value;
                break;
            case LANGUAGE:
                sourceLanguage = value;
                break;
            case CODE_F:
                value = Helpers::ReadFile(value);
            case CODE:
                codeSource.push_back(value);
                break;
            default:
                cout << "\nIgnoring unexpected parameter '" << value << "'";
        }
    }

    Helpers::listToArray(codeSource, &codeArray);
    Helpers::listToArray(fnArgs, &fnArgsArray);

    ControlModule control = ControlModule();
    control.Run(sourceLanguage, moduleID, codeSource.size(), codeArray, fnArgs.size(), fnArgsArray);

    cout << "CASP - Operation Complete";

    return 0;
}

int paramType(string input) {
    int type = OTHER;
    if (input.find("/sourcelang=") == 0)
        type = LANGUAGE;
    else if (input.find("/moduleid=") == 0)
        type = MODULE;
    else if (input.find("/args=") == 0)
        type = FN_ARG;
    else if (input.find("/code=") == 0)
        type = CODE;
    else if (input.find("/codef=") == 0)
        type = CODE_F;
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
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
#define MODULE 1
#define LANGUAGE 2
#define CODE 3
#define CODE_F 4
int SIZES[] { 0, 10, 12, 6, 7 };

int paramType(string);
string parseParam(string, int);
bool stripArgData(string, string*, string*);
string unescapeCharacters(string);

int main(int argCount, char** argArray)
{
    cout << "Code Analyzer Software Package (CASP)" << endl;
    cout << "Build Date: " << TIMESTAMP << endl;

    vector<arg> fnArgs;
    vector<string> codeSource;
    string sourceLanguage;
    string moduleID;

    for (int i = 1; i < argCount; i++) 
    {
        int type = paramType(argArray[i]);
        string value = parseParam(argArray[i], type);
        switch (type) {
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
                string t, v;
                if (stripArgData(argArray[i], &t, &v)) {
                    arg a(t, v);
                    fnArgs.push_back(a);
                }
        }
    }

    ControlModule control = ControlModule();
    control.Run(sourceLanguage, moduleID, codeSource, fnArgs);

    cout << "CASP - Operation Complete";

    return 0;
}

int paramType(string input) {
    int type = OTHER;
    if (input.find("/sourcelang=") == 0)
        type = LANGUAGE;
    else if (input.find("/moduleid=") == 0)
        type = MODULE;
    else if (input.find("/code=") == 0)
        type = CODE;
    else if (input.find("/codef=") == 0)
        type = CODE_F;
    return type;
}

bool stripArgData(string input, string* id, string* value) {
    int slash = input.find("/");
    int eq = input.find("=");

    if (slash != 0) {
        *id = "";
        *value = "";
        return false;
    }

    if (eq == -1) {
        *id = input.substr(1, input.size());
        *value = "";
        return true;
    }

    *id = input.substr(1, eq - 1);
    *value = input.substr(eq + 1, input.size());
    return true;
}

string parseParam(string input, int type) 
{
    int size = SIZES[type];
    string out = input.substr(size, input.size() - size);
    return out;
}

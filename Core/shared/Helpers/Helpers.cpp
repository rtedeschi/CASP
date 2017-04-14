/*  
 *  Helpers.h
 *  Defines Helper functions for the application
 *      
 *  
 *  Created: 2/7/2017 by Ryan Tedeschi
 */ 

#include "Helpers.h"

using namespace std;

namespace Helpers {
    string ReadFile(string filename) {
        FILE* fp = fopen(filename.c_str(), "r");
        string filetext = "";

        if (fp != NULL) {
            char c;
            while ((c = fgetc(fp)) != EOF) {
                filetext += c;
            }
        }

        return filetext;
    }

    string DupStr(string str, int count) {
        string s= "";
        for (int i = 0; i < count; i++) {
            s += str;
        }
        return s;
    }

    string toLower(string str) {
        string c(str);
        std::transform(c.begin(), c.end(), c.begin(), ::tolower);
        return c;
    }
    string toUpper(string str) {
        string c(str);
        std::transform(c.begin(), c.end(), c.begin(), ::toupper);
        return c;
    }

    vector<string> ParseArrayArgument(string tag, vector<arg> args) {
        tag = toLower(tag);
        vector<string> ls;

        for (int i = 0; i < args.size(); i++) {
            if (args[i].id == tag)
                ls.push_back(args[i].value);
        }

        return ls;
    }

    string ParseArgument(string tag, vector<arg> args) {
        tag = toLower(tag);
        string s = "";

        for (int i = 0; i < args.size(); i++) {
            if (args[i].id == tag) {
                s = args[i].value;
                break;
            }
        }

        return s;
    }
}


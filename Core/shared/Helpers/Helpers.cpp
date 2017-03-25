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

    list<string> ParseArrayArgument(string tag, string* args, int argCt) {
        list<string> ls;

        for (int i = 0; i < argCt; i++) {
            if (args[i].find("/" + tag + "=") == 0) {
                int size = tag.size() + 2;
                string out = args[i].substr(size, args[i].size() - size);
                ls.push_back(out);
            }
        }

        return ls;
    }

    string ParseArgument(string tag, string* args, int argCt) {
        string str;

        for (int i = 0; i < argCt; i++) {
            if (args[i].find("/" + tag + "=") == 0) {
                int size = tag.size() + 2;
                str = args[i].substr(size, args[i].size() - size);
            }
        }

        return str;
    }
}


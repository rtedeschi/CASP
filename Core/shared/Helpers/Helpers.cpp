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

}

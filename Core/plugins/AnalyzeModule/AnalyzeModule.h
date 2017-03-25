/*  
 *  AnalyzeModule.h
 *  
 *  
 *  Created: 3/24/2017 by Ryan Tedeschi
 */

#ifndef ANALYZEMODULE_H
#define ANALYZEMODULE_H

#include <string>
#include <iostream>
#include "../../shared/CASP_Plugin/CASP_Plugin.h"

using namespace std;

class AnalyzeModule : public CASP_Plugin {
    public:
        AnalyzeModule();

        virtual void Execute(Markup*, string*, int);

};

#endif
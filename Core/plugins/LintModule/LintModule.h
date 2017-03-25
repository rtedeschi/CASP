/*  
 *  LintModule.h
 *  
 *  
 *  Created: 3/24/2017 by Ryan Tedeschi
 */

#ifndef LINTMODULE_H
#define LINTMODULE_H

#include <string>
#include <iostream>
#include "../../shared/CASP_Plugin/CASP_Plugin.h"

using namespace std;

class LintModule : public CASP_Plugin {
    public:
        LintModule();

        virtual void Execute(Markup*, string*, int);

};

#endif
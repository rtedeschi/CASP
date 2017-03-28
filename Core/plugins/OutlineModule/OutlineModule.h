/*  
 *  OutlineModule.h
 *  
 *  
 *  Created: 3/24/2017 by Ryan Tedeschi
 */

#ifndef OUTLINEMODULE_H
#define OUTLINEMODULE_H

#include <string>
#include <iostream>
#include "../../shared/CASP_Plugin/CASP_Plugin.h"

using namespace std;

class OutlineModule : public CASP_Plugin {
    public:
        OutlineModule();
        virtual void Execute(Markup*, string*, int);
};

#endif
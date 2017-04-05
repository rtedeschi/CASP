/*  
 *  TranslateModule.h
 *  
 *  
 *  Created: 3/24/2017 by Ryan Tedeschi
 */

#ifndef TRANSLATEMODULE_H
#define TRANSLATEMODULE_H

#include <string>
#include <iostream>
#include "../../shared/CASP_Plugin/CASP_Plugin.h"

using namespace std;

class TranslateModule : public CASP_Plugin {
    public:
        TranslateModule();

        virtual CASP_Return* Execute(Markup*, string*, int);

};

#endif
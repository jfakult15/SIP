//
//  save_state.h
//  SIP
//
//  Created by Jacob Fakult on 12/22/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#ifndef __SIP__save_state__
#define __SIP__save_state__

#include <stdio.h>
#include <iostream>

#include "function_object.h"
#include "object.h"

//using namespace std;

struct SaveState
{
    int nestDepth = 0;
    vector<vector<Object> > definedVariables = vector<vector<Object> >(); //multidimensional because we keep track for every nested layer
    vector<FunctionObject> definedFunctions = vector<FunctionObject>();
    //map<string, FunctionObject> definedFunctions;
};

#endif /* defined(__SIP__save_state__) */

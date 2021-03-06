//
//  function_object.h
//  SIP
//
//  Created by Jacob Fakult on 12/24/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#ifndef __SIP__function_object__
#define __SIP__function_object__

#include <stdio.h>
#include <iostream>

#include <vector>
//#include "err.h"
#include "save_state.h"

//#include <string>

using namespace std;

class FunctionObject
{
public:
    FunctionObject(string newName, int newStartLine, int newEndLine, int numParams/*, string newLocation*/);
    
    string name;
    //Object returnValue;
    int startLine;
    int endLine;
    int numParams;
    string location;
};

//bool functionExists(vector<string> line, class SaveState &ss);

#endif /* defined(__SIP__function_object__) */

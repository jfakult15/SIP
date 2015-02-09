//
//  object.h
//  SIP
//
//  Created by Jacob Fakult on 9/17/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#ifndef __SIP__object__
#define __SIP__object__

#include <stdio.h>
#include <iostream>

#include "helpers.h"

using namespace std;

bool isProperVarName(string varName);
string determineType(string value);

class Object
{
public:
    Object();
    Object(string type, string name, string value);
    string getType();
    int getIntValue();
    double getDoubleValue();
    string getStringValue();
    char getCharValue();
    bool getBoolValue();
    bool isCorrectDataFormat();
    bool isIntFormat();
    bool isDoubleFormat();
    bool isStringFormat();
    bool isBoolFormat();
    
    string type;
    string name;
    string value;
};

Object getObjectByName(vector<Object> v, string name);

#endif /* defined(__SIP__object__) */

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
#include <sstream>
#include <map>

#include "helpers.h"
#include "err.h"

using namespace std;

bool isProperVarName(string varName);
string determineType(string value);

class Object
{
public:
    Object();
    Object(string type, string name, string value);
    string getValue();
    string getType();
    int getIntValue();
    double getDoubleValue();
    string getStringValue();
    string toString();
    char getCharValue();
    bool getBoolValue();
    bool isCorrectDataFormat();
    bool isIntFormat();
    bool isDoubleFormat();
    bool isStringFormat();
    bool isBoolFormat();
    bool clean();
    
    string type;
    string name;
    string value;
    
    int isArray = 0;
    map<string, string> values;
    struct errVar getArrayValue(string key);
    string getMapValue();
};

Object getObjectByName(vector<Object> &v, string name);
Object getAnyObjectNamed(vector<vector<Object> > &v, string name);
bool objectExistsWithName(vector<vector<Object> > &v, string name);
bool setObjectWithName(vector<vector<Object> > &v, string name, string value);
bool setMapObjectWithName(vector<vector<Object> > &v, string name, string key, string value);

string compareTypes(string t1, string t2);
int objectTypeValue(string t);

#endif /* defined(__SIP__object__) */

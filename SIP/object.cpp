//
//  object.cpp
//  SIP
//
//  Created by Jacob Fakult on 9/17/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "object.h"
#include "err.h"

Object::Object()
{
    type="int";
    name="x";
    value="1.4";
}

Object::Object(string newType, string newName, string newValue)
{
    type=newType;
    name=newName;
    value=newValue;
}

string Object::getType()
{
    return type;
}

int Object::getIntValue()
{
    return stoi(value);
}

double Object::getDoubleValue()
{
    return atof(value.c_str());
}

string Object::getStringValue()
{
    return value;
}

char Object::getCharValue()
{
    return value[0];
}

bool Object::getBoolValue()
{
    return value.length()>0 && value!="0";
}
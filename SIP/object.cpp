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
    value="1.4";
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
//
//  object.cpp
//  SIP
//
//  Created by Jacob Fakult on 9/17/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "object.h"
#include "err.h"
#include <algorithm>

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
    return determineType(value);
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

bool Object::isCorrectDataFormat()
{
    if (type=="int") return isIntFormat();
    if (type=="double") return isDoubleFormat();
    if (type=="string") return isStringFormat();
    if (type=="bool") return isBoolFormat();
    return false;
}

bool Object::isIntFormat()
{
    for (int i=0; i<value.length(); i++)
    {
        if (!isnumber(int(value[i]))) return false;
    }
    return true;
}

bool Object::isDoubleFormat()
{
    //if (value[value.length()-1]=='.') value+="0"; //should I keep this?
    if (value[value.length()-1]=='.') return false; //or keep this?
    if (count(value.begin(), value.end(), '.')>1) return false; //too many decimal points
    
    for (int i=0; i<value.length(); i++)
    {
        if (!isnumber(int(value[i])) && value[i]!='.') return false;
    }
    return true;
}

bool Object::isStringFormat() //how?
{
    string tempVal=value;
    if ((value[0]=='"' && value[value.length()-1]=='"') || (value[0]=='\'' && value[value.length()-1]=='\'')) //lets not assume a string will have literal embedded quotes
    {
        tempVal=tempVal.substr(1,tempVal.length()-1);
    }
    
    //tempVal=parseString(tempVal);
    
    if (tempVal.length()>0 && parseString(tempVal).length()==0) return false;
    
    return true;
}

bool Object::isBoolFormat()
{
    return value=="true" || value=="false";
}

bool isProperVarName(string varName)
{
    if (!isLetter(int(varName[0]))) return false;
    for (int i=1; i<varName.length(); i++)
    {
        if (!isLetter(int(varName[i])) && !isNumber(int(varName[i])))
        {
            if (varName[i]!='-' && varName[i]!='_')
                return false;
        }
    }
    
    return true;
}

string determineType(string value) //very simplistic, I hope this will satisfy all conditions. More error checking later will determine the validity of the values
{
    if (value.find("\"")!=string::npos || value.find("'")!=string::npos) return "string";
    if (value.find(".")!=string::npos) return "double";
    if (value=="true" || value=="false") return "bool";
    return "int";
}
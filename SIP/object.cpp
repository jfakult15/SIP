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
    type="string";
    name="invalid object name";
    value="Jacob Fakult";
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
    if ((value[0]=='"' && value[value.length()-1]=='"') || (value[0]=='\'' && value[value.length()-1]=='\'')) //lets not assume a string will have literal embedded quotes
    {
        return value.substr(1, value.length()-2);
    }
    return value;
}

char Object::getCharValue()
{
    return value[0];
}

bool Object::getBoolValue()
{
    return value.length()>0 && (value!="0" && value != "false");
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
    if (isNum(value)) return "int";
    return "invalid type";
}

Object getObjectByName(vector<Object> &v, string name)
{
    for (int i=0; i<v.size(); i++)
    {
        if (v[i].name==name)
        {
            return v[i];
        }
    }
    Object o;
    o.name="invalid object name";
    return o;
}

Object getAnyObjectNamed(vector<vector<Object> > &v, string name)
{
    Object o;
    for (int i=0; i<v.size(); i++)
    {
        o = getObjectByName(v[i], name);
        //cout << o.name <<"==\n";
        
        if (o.name!="invalid object name") //we found a variable for this value!
        {
            return o;
        }
    }
    return o;
}

bool objectExistsWithName(vector<vector<Object> > &v, string name)
{
    Object o = getAnyObjectNamed(v, name);
    if (o.name != "invalid object name")
    {
        return true;
    }
    return false;
}

bool setObjectWithName(vector<vector<Object> > &v, string name, string value)
{
    for (int i=v.size()-1; i>=0; i--)
    {
        for (int j=0; j<v[i].size(); j++)
        {
            if (v[i][j].name == name)
            {
                v[i][j].value = value;
                v[i][j].type = v[i][j].getType();
                return true;
            }
        }
    }
    return false;
}

int objectTypeValue(string t) //casting hierarchy helper
{
    if (t=="bool") return 1;
    if (t=="int") return 2;
    if (t=="double") return 3;
    if (t=="string") return 4;
    return 5; //invalid type
    
}

string compareTypes(string t1, string t2) //casting hierarchy
{
    if (objectTypeValue(t1)>objectTypeValue(t2)) return t1;
    return t2;
}



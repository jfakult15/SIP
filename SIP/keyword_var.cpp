//
//  keyword_var.cpp
//  SIP
//
//  Created by Jacob Fakult on 12/20/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "keyword_var.h"

errVar syntaxVar(vector<string> tokens)
{
    errVar err;
    err.errorPos=-1;
    
    if (tokens[0]!="var") //I know this seems unnecessary, but vars can be declared in for loops etc.
    {
        err.errorPos=0;
        err.message="Unknown variable";
        return err;
    }
    
    Object obj;
    
    string varName=tokens[1];
    if (!isProperVarName(varName))
    {
        err.errorPos=1;
        err.message="Invalid variable name";
    }
    
    obj.name=varName;
    
    //cout << tokens[1] << "--\n";
    
    if (tokens[2]!="=")
    {
        err.errorPos=2;
        err.message="Invalid assignment operator";
        return err;
    }
    
    string varValue=tokens[3];
    
    string type=determineType(varValue);
    if (type=="null")
    {
        err.errorPos=3;
        err.message="Couldn't determine data type";
        return err;
    }
    
    obj.type=type;
    obj.value=varValue;
    
    if (!obj.isCorrectDataFormat())
    {
        err.errorPos=3;
        err.message="Data type wasn't parsable";
        return err;
    }
    
    if (tokens.size()>5) // {var, x, =, 10, ;} (there should be at least 5 values in tokens)
    {
        err.errorPos=int(tokens.size()-1);
        err.message="Var declaration improperly formated";
        return err;
    }
    
    if (tokens[tokens.size()-1]!=";")
    {
        err.errorPos=int(tokens.size()-1);
        //cout << "Error pos: " << err.errorPos << "\n";
        err.message="Did you forget a semicolon (;)?";
        return err;
    }
    
    if (tokens.size()!=5) // {var, x, =, 10, ;} (there should be at least 5 values in tokens)
    {
        err.errorPos=int(tokens.size()-1);
        err.message="Var declaration improperly formated";
        return err;
    }
    
    //cout << "Obj: " << obj.type << "\n";
    //objects.push_back(obj); //should this be declared in the syntax checker? NO
    //cout << "Obj2: " << objects[0].type << "\n";
    
    return err;
}
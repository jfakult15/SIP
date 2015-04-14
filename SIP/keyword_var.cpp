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
    
    if (tokens.size() == 2)
    {
        return err;
    }
    
    if (tokens[tokens.size()-1] == ";")
    {
        err.errorPos = tokens.size()-1;
        err.message = "SIP no longer uses semicolons";
        return err;
    }
    
    if (tokens.size()>2 && tokens[2]=="[") //array declaration
    {
        if (tokens.size()>3 && tokens[3] != "]")
        {
            err.errorPos = 3;
            err.message = "Invalid array declaration\nShould be in format: " + varName + "[]";
            return err;
        }
        if (tokens.size()==4) return err;
        if (tokens[4] != "=")
        {
            err.errorPos = 4;
            err.message = "Invalid assignment operator";
            return err;
        }
        if (tokens.size()<=5)
        {
            err.errorPos = 4;
            err.message = "Need to assign a value if you have an equals sign";
            return err;
        }
        if (tokens[tokens.size()-1] == ",")
        {
            err.errorPos = tokens.size()-1;
            err.message = "Can't have a comma ending an array creation";
            return err;
        }
        return err;
        /*
        bool isVar = true;
        for (int i=5; i<tokens.size(); i++)
        {
            if (isVar)
            {
                Object o;
                o.value = tokens[i];
                string type = o.getType();
                if (type == "string")
                {
                    int l = type.length()-1;
                    if ((type[0] != '"' || type[l] != '"') || (type[0] != '\'' || type[l] != '\''))
                    {
                        type = "";
                    }
                }
                if (!isProperVarName(tokens[i]) || type == "")
                {
                    err.errorPos = i;
                    err.message = "Unexpected string. Expected value in array creation\nFormat: var a[] = 10,2,4";
                    return err;
                }
            }
            else
            {
                if (tokens[i] != ",")
                {
                    err.errorPos = i;
                    err.message = "Please use a comman to seperate array values\nFormat: var a[] = 10,2,4";
                    return err;
                }
                if (i == tokens.size()-1)
                {
                    err.errorPos = i;
                    err.message = "Can't have a comma ending an array creation";
                    return err;
                }
            }
            isVar = !isVar;
        }*/
    }
    
    obj.name=varName;
    
    //cout << tokens[1] << "--\n";
    //cout << vectorToString(tokens) << "\n";
    
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
    
    /*if (tokens.size()>4) // {var, x, =, 10, ;} (there should be at least 5 values in tokens)
    {
        err.errorPos=4;
        err.message="Unexpected symbol";
        if (tokens[4]==";") err.message += "\nSIP no longer uses semicolons";
        return err;
    }*/
    
    //remove semi
    /*
    if (tokens[tokens.size()-1] != ";")
    {
        err.errorPos=int(tokens.size()-1);
        err.message="Did you forget a semicolon (;)?";
        return err;
    }*/
    
    /*if (tokens.size()!=5) // {var, x, =, 10, ;} (there should be at least 5 values in tokens)
    {
        err.errorPos=int(tokens.size()-1);
        err.message="Var declaration improperly formated";
        return err;
    }*/
    
    //cout << "Obj: " << obj.type << "\n";
    //objects.push_back(obj); //should this be declared in the syntax checker? NO
    //cout << "Obj2: " << objects[0].type << "\n";
    
    return err;
}

errVar executeVar(vector<string> tokens, SaveState &ss, ExecutionOutput &output, vector<string> &code)
{
    errVar err;
    
    Object obj;
    
    obj.name=tokens[1];
    
    if (tokens[2]=="[") //we have an array!
    {
        obj.isArray = 1;
        if (tokens.size() == 4) //uninitialized array declaration
        {
            while (ss.nestDepth>=ss.definedVariables.size())
            {
                ss.definedVariables.push_back(vector<Object>());
            }
            ss.definedVariables[ss.nestDepth].push_back(obj);
            
            return err;
        }
        else
        {
            map<string, string> values;
            
            int key = 1;
            for (int i=5; i<tokens.size(); i+=2)
            {
                values[to_string(key)] = tokens[i];
                key++;
            }
            
            obj.values = values;
            
            while (ss.nestDepth>=ss.definedVariables.size())
            {
                ss.definedVariables.push_back(vector<Object>());
            }
            ss.definedVariables[ss.nestDepth].push_back(obj);
            
            return err;
        }
    }
    
    vector<string> varValue;
    string type = "string";
    if (tokens.size() > 2)
    {
        varValue=vector<string>(tokens.begin()+3, tokens.end());//tokens[3]);
    }
    
    err = anyEval(varValue, ss, output, code);
    //cout << vectorToString(varValue) << " " << err.message << "==\n";
    if (err.errorPos != -1)
    {
        err.errorPos = 3;
        return err;
    }
    
    type=determineType(err.message);
    obj.type=type;
    obj.value = err.message;
    obj.value = obj.getStringValue();
    
    
    while (ss.nestDepth>=ss.definedVariables.size())
    {
        ss.definedVariables.push_back(vector<Object>());
    }
    ss.definedVariables[ss.nestDepth].push_back(obj);
    
    return err;
}



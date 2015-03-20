//
//  keyword_input.cpp
//  SIP
//
//  Created by Jacob Fakult on 2/9/15.
//  Copyright (c) 2015 jfakult15. All rights reserved.
//

#include "keyword_input.h"

errVar syntaxInput(vector<string> tokens)
{
    errVar e;
    
    if (tokens.size()<3)
    {
        e.errorPos = tokens.size()-1;
        e.message = "Improper input syntax (format: input knownVariable1, knownVariable2, ...;)";
    }
    else if (tokens.size() > 3)
    {
        bool properSyntax = true;
        for (int i=1; i<tokens.size()-1; i++)
        {
            if (i % 2 == 1) //expect a variable
            {
                string varName = tokens[i];
                if (!isProperVarName(varName))
                {
                    e.errorPos = i;
                    e.message = "Improper input syntax: expected variable, found: '" + varName + "'";
                    return e;
                }
            }
            else //expect a comma to seperate variables
            {
                string temp = tokens[i];
                if (temp != ",")
                {
                    e.errorPos = i;
                    e.message = "Improper input syntax: expected comma seperated variables, found '" + temp + "'";
                    return e;
                }
            }
        }
    }
    else if (tokens[tokens.size()-1] != ";")
    {
        e.errorPos = tokens.size()-1;
        e.message = "Missing semicolon (;)";
    }
    
    return e;
}

errVar executeInput(vector<string> tokens, SaveState &ss)
{
    errVar err;
    err.errorPos=-1;
    
    for (int i=1; i<tokens.size()-1; i++)
    {
        if (!objectExistsWithName(ss.definedVariables, tokens[i]))
        {
            err.errorPos = i;
            err.message = "Unable to input undefined variable";
            return err;
        }
    }
    
    for (int i=1; i<tokens.size()-1; i++)
    {
        string temp;
        cin >> temp;
        
        if (setObjectWithName(ss.definedVariables, tokens[i], temp))
        {
            //variable was properly set!
        }
        else
        {
            err.errorPos = i;
            err.message = "Variable was unable to be input for an unknown reason";
        }
    }
    
    return err;
}
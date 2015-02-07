//
//  keyword_print.cpp
//  SIP
//
//  Created by Jacob Fakult on 12/20/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "keyword_print.h"

using namespace std;

errVar syntaxPrint(vector<string> tokens)
{
    errVar err;
    err.errorPos=-1;
    
    for (int i=1; i<tokens.size()-1; i++)
    {
        Object temp;
        temp.value = tokens[i];
        string value = temp.value;
        bool isString = false;
        
        if ((tokens[i][0]=='"' && tokens[i][tokens[i].length()-1]=='"') || (tokens[i][0]=='\'' && tokens[i][tokens[i].length()-1]=='\''))
        {
            isString = true;
        }
        if (!isString)
        {
            if (tokens[i] == "+")
            {
                continue;
            }
            /*if (tokens[i]!="+")
            {
                err.errorPos = i;
                err.message = "Bad concatenation operator";
                return err;
            }
            else
            {
                err.errorPos = i;
                string type = temp.getType();
                err.message = "Unable to cast '" + type + "' to string";
                return err;
            }*/
        }
    }
    
    if (tokens[tokens.size()-1] != ";")
    {
        err.errorPos = int(tokens.size())-1;
        err.message = "Missing a semicolon";
    }
    
    return err;
}

#include "object.h"

errVar executePrint(vector<string> tokens, ExecutionOutput &output, SaveState &ss)
{
    errVar err;
    err.errorPos=-1;
    
    string printStr = "";
    
    for (int i=1; i<tokens.size()-1; i++)
    {
        Object temp;
        temp.value = tokens[i];
        string value = temp.value;
        
        bool isString = ((tokens[i][0]=='"' && tokens[i][tokens[i].length()-1]=='"') || (tokens[i][0]=='\'' && tokens[i][tokens[i].length()-1]=='\''));
        
        if (!isString)
        {
            if (tokens[i] == "+")
            {
                continue;
            }
            if (tokens[i]!="+")
            {
                Object o;
                bool tokenIsObject=false;
                
                for (int depth=0; depth<ss.definedVariables.size(); i++)
                {
                    o = getObjectByName(ss.definedVariables[depth], tokens[i]);
                    if (o.name!="invalid object name") //we found a variable for this value!
                    {
                        tokenIsObject = true;
                        depth = ss.nestDepth;
                    }
                }
                if (!tokenIsObject)
                {
                    err.errorPos = i;
                    err.message = "Unknown variable";
                }
            }
            else
            {
                err.errorPos = i;
                string type = temp.getType();
                err.message = "Unable to cast '" + type + "' to string";
            }
        }
        else
        {
            printStr+=tokens[i].substr(1, tokens[i].length()-2);
        }
    }
    
    if (err.errorPos == -1)
    {
        output.info.push_back(printStr);
    }
    /*else
    {
        output.err.push_back(err.message);
    }*/
    return err;
}




//
//  keyword_function.cpp
//  SIP
//
//  Created by Jacob Fakult on 2/6/15.
//  Copyright (c) 2015 jfakult15. All rights reserved.
//

#include "keyword_function.h"
#include "helpers.h"

using namespace std;

errVar syntaxFunction(vector<string> tokens)
{
    errVar e;
    e.errorPos=-1;
    
    if (tokens.size() < 5)
    {
        e.errorPos = 0;
        e.message = "Improper function declaration syntax: Not enough arguments\nExpected: 'function name(variable1, variable2, ...)";
        return e;
    }
    
    string funcName = tokens[1];
    if (!isProperVarName(funcName))
    {
        e.errorPos = 1;
        e.message = "Invalid function name. Found: '" + funcName + "'";
        return e;
    }
    
    if (tokens[2] != "(")
    {
        e.errorPos = 2;
        e.message = "Improper function declaration syntax: Missing opening parenthesis\nExpected: 'function name(variable1, variable2, ...)";
        return e;
    }
    
    bool isVar = true; //in function declaration we
    bool lastWasByref = false;
    for (int i=3; i<tokens.size()-1; i++) //inside the parentheses
    {
        //cout << tokens[i] << " " << isVar << "\n";
        if (isVar)
        {
            //cout << tokens[i] << "==\n";
            string varName = tokens[i];
            if (toLowerCase(varName) == "byref")
            {
                if (lastWasByref)
                {
                    e.errorPos = i;
                    e.message = "Invalid variable name: '" + tokens[i] + "'\nByref is a reserved word";
                }
                if (!lastWasByref && tokens[i+1] == ",")
                {
                    e.errorPos = i;
                    e.message = "Invalid variable name: '" + tokens[i] + "'\nByref is a reserved word";
                }
                lastWasByref = true;
                continue;
            }
            lastWasByref = false;
            if (!isProperVarName(varName))
            {
                e.errorPos = i;
                e.message = "Invalid variable name: '" + varName +"'";
                return e;
            }
        }
        else
        {
            string comma = tokens[i];
            if (i==tokens.size()-2) //last value in function should not be a comma
            {
                e.errorPos = i+1;
                e.message = "Invalid function syntax: Expected a variable after last comma";
            }
            if (comma != ",")
            {
                e.errorPos = i;
                e.message = "Invalid function syntax: variables should be seperated by commas\nFound: '" + comma + "'";
            }
        }
        isVar = !isVar;
    }
    
    if (tokens[tokens.size()-1] != ")")
    {
        e.errorPos = int(tokens.size()-1);
        e.message = "Improper function declaration syntax: Missing closing parenthesis\nExpected: 'function name(variable1, variable2, ...)";
        return e;
    }
    
    if (tokens[tokens.size()-1] == ";")
    {
        e.errorPos = int(tokens.size())-1;
        e.message = "Function declarations should not have a semicolon";
        return e;
    }
    
    
    
    return e;
}

errVar executeFunction(vector<string> &line, vector<string> &code, ExecutionOutput &output, int &curLine, SaveState &ss)
{
    
    errVar e;
    
    int firstLine = curLine;
    //string keyword = line[0];
    
    //line.erase(line.begin()); //we don't need keyword parts of the statements here
    int blockEnd = getClosingBraceLine(code, curLine+1, 0);
    curLine = blockEnd;
    
    //FunctionObject(string newName, int newStartLine, int newEndLine, int newNumParams)
    cout << "hi :)\n";
    
    return e;
}
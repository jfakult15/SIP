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
    
    if (tokens.size() < 4)
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
            if (i<tokens.size()-2) //if we are passing in an array
            {
                if (tokens[i+1] == "[")
                {
                    if (tokens[i+2]=="]")
                    {
                        tokens.erase(tokens.begin()+i+1);
                        tokens.erase(tokens.begin()+i+1);
                    }
                }
            }
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
            /*if (!isProperVarName(varName)) //commented out because we can pass in hardcoded values
            {
                e.errorPos = i;
                e.message = "Invalid variable name: '" + varName +"'";
                return e;
            }*/
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
        //cout << "yes\n";
        e.errorPos = int(tokens.size()-1);
        e.message = "Improper function declaration syntax: Missing closing parenthesis\nExpected: 'function name(variable1, variable2, ...)";
        return e;
    }
    
    //remove semi
    /*
    if (tokens[tokens.size()-1] == ";")
    {
        e.errorPos = int(tokens.size())-1;
        e.message = "Function declarations should not have a semicolon";
        return e;
    }*/
    
    
    
    return e;
}

//handles declarations and executions! Magical!!
errVar executeFunction(vector<string> &line, vector<string> &code, ExecutionOutput &output, int &curLine, SaveState &ss)
{
    errVar e;
    
    if (line[0] == "function") //declaration
    {
        int firstLine = curLine;
        int blockEnd = getClosingBraceLine(code, curLine+2, 0);
        curLine = blockEnd+1;
        e = createFunction(line, firstLine, blockEnd, ss);
    }
    else // execution
    {
        string funcName = line[0];
        int numParams = 0;
        for (int i=2; i<line.size()-1; i++)
        {
            if (line[i] != ",") numParams++;
        }
        
        bool createdFunction = false;
        if (!funcExists(funcName, numParams, ss)) //haven't seen this function before. Let's find it
        {
            for (int i=0; i<code.size(); i++)
            {
                string start = code[i].substr(0, 8);
                //trim(start);
                if (start == "function")
                {
                    vector<string> temp = tokenize(code[i]);
                    if (temp.size()>1)
                    {
                        if (temp[1] == funcName)
                        {
                            int firstLine = i;

                            int blockEnd = getClosingBraceLine(code, i+2, 0);
                            createFunction(temp, firstLine, blockEnd, ss);
                            
                            if (funcExists(funcName, numParams, ss))
                            {
                                createdFunction = true;
                                i = code.size();
                            }
                        }
                    }
                    else
                    {
                        //bad function dec?
                        //cout << ":(";
                    }
                }
            }
        }
        else
        {
            createdFunction = true;
        }
        
        if (!createdFunction)
        {
            e.errorPos = 0;
            e.message = "Undefined function: '" + line[0] + "'";
            return e;
        }
        
        vector<string> temp = line;
        temp.insert(temp.begin(), "function");
        //temp.erase(temp.end()-1);
        e = syntaxFunction(temp);
        if (e.errorPos >= 0)
        {
            return e;
        }
        
        nest(ss);
        
        FunctionObject f = getFunctionNamed(line[0], ss);
        
        e = createTempVars(line, tokenize(code[f.startLine]), ss);
        
        vector<string> block(code.begin()+f.startLine+2, code.begin()+f.endLine);
        //cout << vectorToString(block);
        //bug: add variables to pass in here, parallel vector<Object>
        //execute(block, output, 0);
        executeCode(block, output);
        un_nest(ss);
        e.message = output.returnVal;
        
        curLine++;
        
        un_nest(ss);
    }
    
    return e;
}

errVar createTempVars(vector<string> line, vector<string> funcLine, SaveState &ss)
{
    errVar e;
    
    vector<vector<Object> > oldVars = vector<vector<Object> >(ss.definedVariables.begin(), ss.definedVariables.end()-1);
    
    for (int i=2; i<line.size()-1; i++)
    {
        if (line[i] != ",")
        {
            Object o = getAnyObjectNamed(oldVars, line[i], ss.nestDepth);
            if (o.name == "invalid object name")
            {
                e.message = "Unknown variable passed into function: " + line[i];
                e.errorPos = i;
                return e;
            }
            //cout << vectorToString(funcLine) << " " << i << "--\n";
            o.name = funcLine[i+1];
            ss.definedVariables[ss.nestDepth].push_back(o);
        }
    }
    
    
    return e;
}

errVar createFunction(vector<string> line, int firstLine, int blockEnd, SaveState &ss)
{
    errVar e;
    
    string funcName = line[1];
    
    int numParams = 0;
    //cout << vectorToString(line) << "==\n";
    for (int i=3; i<line.size()-1; i++)
    {
        if (line[i] != "," && line[i] != "[" && line[i] != "]") numParams++;
    }
    
    //cout << funcName << " " << numParams << "--\n";
    
    if (funcExists(funcName, numParams, ss) && !seenFuncBefore(funcName, firstLine, ss))
    {
        e.errorPos = 1;
        e.message = "Function " + funcName + " is already defined";
        return e;
    }
    
    ss.definedFunctions.push_back(FunctionObject(funcName, firstLine, blockEnd, numParams));
    
    return e;
}

bool funcExists(string name, int numParams, SaveState &ss)
{
    for (int i=0; i<ss.definedFunctions.size(); i++)
    {
        if (ss.definedFunctions[i].name == name && ss.definedFunctions[i].numParams == numParams)
        {
            return true;
        }
    }
    
    return false;
}

bool seenFuncBefore(string name, int startLine, SaveState &ss)
{
    for (int i=0; i<ss.definedFunctions.size(); i++)
    {
        if (ss.definedFunctions[i].name == name && ss.definedFunctions[i].startLine == startLine)
        {
            return true;
        }
    }
    
    return false;
}

FunctionObject getFunctionNamed(string name, SaveState &ss)
{
    for (int i=0; i<ss.definedFunctions.size(); i++)
    {
        if (ss.definedFunctions[i].name == name)
        {
            return ss.definedFunctions[i];
        }
    }
    return FunctionObject("invalid function name", 0, 0, 0);
}


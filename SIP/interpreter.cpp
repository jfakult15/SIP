//
//  interpreter.cpp
//  SIP
//
//  Created by Jacob Fakult on 12/22/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "interpreter.h"

using namespace std;

map<string, KeywordObject> keywords=initKeywordObjects();

void interpreter(SaveState &ss, vector<string> &code, vector<string> line, ExecutionOutput &output, int &curLine)
{
    string keyword = line[0];
    
    if (keywords.find(keyword) != keywords.end()) //keyword exists (is defined)
    {
        if (keywords[keyword].isBlockStatement)
        {
            if (keyword=="if" || keyword=="while")
            {
                string temp="";
                for (int i=1; i<line.size(); i++)
                {
                    temp += line[i];
                }
                cout << "bool: " << boolEval(temp, ss) << "\n";
                curLine++;
            }
            else if (keyword=="function")
            {
                
            }
        }
        else if (keywords[keyword].isSingleWordStatement) //break or continue mostly
        {
            //do something
            analyzeLine(line, ss, output, curLine);
            curLine++;
        }
        else if (keywords[keyword].isSingleLineStatement)
        {
            analyzeLine(line, ss, output, curLine);
            curLine++;
        }
        else
        {
            output.err.push_back("wtf kind of statement is this?");
        }
    }
    else //probably a variable that is starting the line
    {
        analyzeLine(line, ss, output, curLine);
        curLine++;
    }
}

void analyzeLine(vector<string> line, SaveState &ss, ExecutionOutput &output, int &curLine)
{
    if (line.size()==0)
    {
        return;
    }
    
    errVar err;
    err.errorPos = -1;
    
    string keyword = line[0];
    
    if (keyword=="print")
    {
        err = executePrint(line, output, ss);
    }
    else if (keyword=="var")
    {
        err = executeVar(line, ss);
    }
    else
    {
        Object o;
        bool tokenIsObject=false;
        
        for (int depth=0; depth<ss.definedVariables.size(); depth++)
        {
            o = getObjectByName(ss.definedVariables[depth], keyword);
            
            if (o.name!="invalid object name") //we found a variable for this value!
            {
                tokenIsObject = true;
                depth = ss.nestDepth;
            }
        }
        if (!tokenIsObject)
        {
        }
        else
        {
            err.errorPos = 0;
            err.message = "Undefined variable";
        }
    }
    
    if (err.errorPos != -1)
    {
        output.err.push_back("Runtime error: line "+to_string(curLine+1));
        output.err.push_back("\"" + line[err.errorPos] + "\"\n" );
        output.err.push_back("Got error: " + err.message);
    }
}
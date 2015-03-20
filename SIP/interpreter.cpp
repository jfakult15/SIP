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

errVar interpreter(SaveState &ss, vector<string> &code, vector<string> line, ExecutionOutput &output, int &curLine)
{
    string keyword = line[0];
    errVar e;
    
    if (keywords.find(keyword) != keywords.end()) //keyword exists (is defined)
    {
        if (keywords[keyword].isBlockStatement)
        {
            if (keyword=="if")
            {
                e = executeIf(line, code, output, curLine, ss);
            }
            else if (keyword=="while")
            {
                e = executeWhile(line, code, output, curLine, ss);
            }
            else if (keyword=="for")
            {
                
            }
            else if (keyword=="function")
            {
                e = executeFunction(line, code, output, curLine, ss);
            }
        }
        else if (keywords[keyword].isSingleWordStatement) //break or continue mostly
        {
            //do something
            e = analyzeLine(line, ss, output, curLine);
            curLine++;
        }
        else if (keywords[keyword].isSingleLineStatement)
        {
            e = analyzeLine(line, ss, output, curLine);
            curLine++;
        }
        else
        {
            output.err.push_back("What kind of statement is this?");
        }
    }
    else //probably a variable that is starting the line
    {
        analyzeLine(line, ss, output, curLine);
        curLine++;
    }
    
    return e;
}

errVar analyzeLine(vector<string> line, SaveState &ss, ExecutionOutput &output, int &curLine)
{
    if (line.size()==0)
    {
        return errVar();
    }
    
    errVar err;
    err.errorPos = -1;
    
    string keyword = line[0];
    
    if (keyword=="print")
    {
        err = executePrint(line, output, ss, true);
    }
    else if (keyword=="printf")
    {
        err = executePrint(line, output, ss, false);
    }
    else if (keyword=="var")
    {
        err = executeVar(line, ss);
    }
    else if (keyword=="input")
    {
        err = executeInput(line, ss);
    }
    else if (keyword=="import")
    {
        err = executeImport(line, output, ss);
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
                depth = ss.definedVariables.size();
            }
        }
        if (tokenIsObject)
        {
            if (line[1] != "=")
            {
                err.errorPos = 1;
                err.message = "Invalid assignment operator (i.e use "+keyword+" = value";
                return err;
            }
            string expr = vectorToString(vector<string>(line.begin()+2, line.end()-1));
            //cout << expr << "=\n";
            expr = anyEval(tokenize(expr), ss).message;
            Object o;
            o.value = expr;
            //cout << o.value << " yes==\n";
            o.type = o.getType();
            if (o.type == "invalid type")
            {
                err.errorPos = 2;
                err.message = "Invalid assignment value";
                return err;
            }
            if (line[line.size()-1] != ";")
            {
                err.errorPos = 3;
                err.message = "Variable assignment missing semicolon";
                return err;
            }
            setObjectWithName(ss.definedVariables, keyword, o.value);
        }
        else
        {
            err.errorPos = 0;
            err.message = "Undefined variable";
        }
    }
    
    /*if (err.errorPos != -1)
    {
        output.err.push_back("Runtime error: line "+to_string(curLine+1));
        output.err.push_back("\"" + line[err.errorPos] + "\"\n" );
        output.err.push_back("Got error: " + err.message);
    }*/
    return err;
}
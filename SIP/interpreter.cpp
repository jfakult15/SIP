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
    errVar e;
    
    string keyword = line[0];
    
    if (keyword[0] == '#') //comment, ignore
    {
        curLine++;
        return e;
    }
    
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
                e = executeFor(line, code, output, curLine, ss);
                clean(ss.definedVariables[0]);
            }
            else if (keyword=="function")
            {
                e = executeFunction(line, code, output, curLine, ss);
            }
        }
        else if (keywords[keyword].isSingleWordStatement) //break or continue mostly
        {
            //do something
            e = analyzeLine(line, ss, output, curLine, code);
            curLine++;
        }
        else if (keywords[keyword].isSingleLineStatement)
        {
            e = analyzeLine(line, ss, output, curLine, code);
            curLine++;
        }
        else
        {
            output.err.push_back("What kind of statement is this?");
        }
    }
    else //A variable assignment or function invocation
    {
        if (isAssignment(line)) //dealing with a variable
        {
            if (line[1] != "=") //e.g convert x += 6; --> x = x + 6;
            {
                char op = line[1][0];
                line[1] = "=";
                line.insert(line.begin()+2, line[0]);
                line.insert(line.begin()+3, string(1,op));
            }
            errVar eval = anyEval(vector<string>(line.begin()+2, line.end()-1), ss, output, code);
            if (e.errorPos >=0)
            {
                return e;
            }
            setObjectWithName(ss.definedVariables, line[0], eval.message);
            curLine++;
        }
        else if (line[1] == "(") //dealing with a function
        {
            //cout << "Exec func\n";
            vector<string> temp = line;
            temp.insert(temp.begin(), "function");
            if (temp[temp.size()-1] != ";")
            {
                e.errorPos = int(temp.size()-1);
                e.message = "Function invocation needs semicolon (;)";
                return e;
            }
            temp.erase(temp.end()-1);
            e = syntaxFunction(temp);
            //cout << e.message << "==\n";
            if (e.errorPos != -1) //improper function invocation syntax
            {
                return e;
            }
            
            string funcName = temp[1];
            int numParams = 0;
            for (int i=3; i<temp.size()-1; i++)
            {
                if (temp[i] != ",") numParams++;
            }
            
            
            if (!funcExists(funcName, numParams, ss))
            {
                bool foundFunc = false;
                for (int i=curLine; i<code.size(); i++)
                {
                    if (code[i].find("function " + funcName) != string::npos) //we found the function! Add it to our known functions
                    {
                        int startLine = i;
                        //cout << code[curLine] << "===\n";
                        int endLine = getClosingBraceLine(code, i+2, 0);
                        createFunction(tokenize(code[i]), startLine, endLine, ss);
                        i=int(code.size());
                        foundFunc = true;
                    }
                }
                if (!foundFunc)
                {
                    e.errorPos = 0;
                    e.message = "Unknown function. Function was never declared";
                    return e;
                }
            }
            
            FunctionObject f = getFunctionNamed(funcName, ss);
            if (f.name == "invalid function name") //this shouldn;t ever happen of course
            {
                e.errorPos = 0;
                e.message = "Fatal error. Quitting...";
                return e;
            }
            
            vector<string> block(code.begin()+f.startLine+2, code.begin()+f.endLine);
            ss.nestDepth++;
            //cout << vectorToString(block);
            //bug: add variables to pass in here, parallel vector<Object>
            execute(block, output);
            ss.nestDepth--;
            //cout << output.returnVal << "==\n";
            
            curLine++;
        }
        else
        {
            //cout << curLine << " " << line.size() << "--\n";
            if (!isProperVarName(line[1]))
            {
                e.errorPos = 1;
                e.message = "Unknown assigment operator (use =, +=, -=, *=, /=, or ^=)";
            }
            else
            {
                e.errorPos = 0;
                e.message = "Unexpected token";// (this is not a function invocation or a variable assignment)";
            }
            return e;
        }
        
        //analyzeLine(line, ss, output, curLine);
    }
    
    return e;
}

errVar analyzeLine(vector<string> line, SaveState &ss, ExecutionOutput &output, int &curLine, vector<string> &code)
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
        err = executePrint(line, output, ss, true, code);
    }
    else if (keyword=="printf")
    {
        err = executePrint(line, output, ss, false, code);
    }
    else if (keyword=="var")
    {
        err = executeVar(line, ss, output, code);
    }
    else if (keyword=="input")
    {
        err = executeInput(line, ss);
    }
    else if (keyword=="import")
    {
        err = executeImport(line, output, ss);
    }
    else if (keyword=="return")
    {
        err = executeReturn(line, output, ss, code);
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
                depth = int(ss.definedVariables.size());
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
            expr = anyEval(tokenize(expr), ss, output, code).message;
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
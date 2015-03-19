//
//  execute.cpp
//  SIP
//
//  Created by Jacob Fakult on 9/10/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//


/*
 This is the main file. It ensures the execution of all tokenizing, syntax checking, and interpretation/execution aspects of the code
*/

#include "execute.h"

using namespace std;

//map<string, KeywordObject> keywords=initKeywordObjects();

struct SaveState ss;

void execute(vector<string> code, ExecutionOutput &output)
{
    output.info.push_back("Executing...\n");
    
    //syntax checking. We check the syntax first so we don't have to worry about it on execution
    for (int i=0; i<code.size(); i++)
    {
        string line=code[i];
        vector<string> tokens=tokenize(line);
        errVar syntaxError=checkSyntax(tokens, output);
        
        if (syntaxError.errorPos>=0) //add error messages
        {
            output.err.push_back("Syntax error: line " + to_string(i+1));
            output.err.push_back("\"" + tokens[syntaxError.errorPos] + "\"\n" );
            output.err.push_back("Got error: " + syntaxError.message);
            return;
        }
    }
    
    output.info.push_back("Syntax Okay!");
    
    executeCode(code, output);
}

//the int this (and all other syntax functions) return the character position of the error or a value less than zero on no errors
errVar checkSyntax(vector<string> tokens, ExecutionOutput &output) //returns first character that caused the issue
{
    output.info.push_back("Checking syntax...");
    
    errVar err;
    
    if (tokens.size()==0)
    {
        output.info.push_back("No tokens found... quitting :(");
        exit(1);
    }
    
    /*
    if (keywords.find(tokens[0])==keywords.end())// && getObjectNamed(objects, tokens[0])<0) //the first word isnt a keyword or a variable
    {
        err.errorPos=0;
        err.message="Unknown keyword";
        return err;
    }*/
    
    if (tokens[0]=="var")
    {
        err=syntaxVar(tokens);
    }
    else if (tokens[0]=="for")
    {
        err=syntaxFor(tokens);
    }
    else if (tokens[0]=="else" &&  tokens[1]=="if") //else if functions the same way as if... I think
    {
        tokens.erase(tokens.begin());
    }
    else if (tokens[0]=="if" || tokens[0]=="while")
    {
        err=syntaxIfWhile(tokens);
    }
    else if (tokens[0] == "function")
    {
        err = syntaxFunction(tokens);
    }
    else if (tokens[0] == "print")
    {
        err = syntaxPrint(tokens);
    }
    else if (tokens[0] == "printf")
    {
        err = syntaxPrint(tokens);
    }
    else if (tokens[0] == "input")
    {
        err = syntaxInput(tokens);
    }
    else if (tokens[0][0] == '#') //comment
    {
        //do nothing
    }
    
    return err;
}

int getObjectNamed(vector<Object> &objects, string name)
{
    for (int i=0; i<objects.size(); i++)
    {
        if (objects[i].name==name) return i;
    }
    
    return -1;
}

void executeCode(vector<string> code, ExecutionOutput &output)
{
    output.info.push_back("Executing code...");
    output.info.push_back("Output:");
    
    errVar e;
    
    //actual execution
    int curLine=0; //which line we are curently executing
    while (curLine<code.size())
    {
        vector<string> tokens = tokenize(code[curLine]);
        e = interpreter(ss, code, tokens, output, curLine);
        if (e.errorPos != -1)
        {
            output.err.push_back("Runtime error: line " + to_string(curLine) + "\nToken: '" + tokens[e.errorPos] + "'\n\nGot error: " + e.message);
            return;
        }
    }
}


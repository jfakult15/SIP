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

bool verbose = false;

void execute(vector<string> code, ExecutionOutput &output, int numImports)
{
    output.info.push_back("Executing...\n");
    
    //syntax checking. We check the syntax first so we don't have to worry about it on execution
    for (int i=0; i<code.size(); i++) //loop through every line in the program
    {
        string line=code[i];
        trim(line); //remove surrounding whitespace
        vector<string> tokens=tokenize(line); //lexicographically splits the line into tokens
        
        errVar syntaxError=checkSyntax(tokens, output); //first phase of semi-interpeted language
        
        if (syntaxError.errorPos>=0) //add error messages
        {
            output.err.push_back("Syntax error: line " + to_string(i+1-numImports));
            output.err.push_back("Line: " + line);
            output.err.push_back("\"" + tokens[syntaxError.errorPos] + "\"\n" );
            output.err.push_back("Got error: " + syntaxError.message);
            return;
        }
    }
    
    output.info.push_back("Syntax Okay!"); //output.info is displayed in verbose mode (-v CLi flag)
    
    executeCode(code, output); //second phase of language
}

//the int this (and all other syntax functions) return the character position of the error or a value less than zero on no errors
errVar checkSyntax(vector<string> tokens, ExecutionOutput &output) //returns first character that caused the issue
{
    output.info.push_back("Checking syntax...");
    
    errVar err;
    
    if (tokens.size()==0)
    {
        output.info.push_back("No tokens found... quitting :("); //this shouldn't ever happen of course
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
    else if (tokens[0] == "import")
    {
        err = syntaxImport(tokens);
    }
    else if (tokens[0] == "return")
    {
        err = syntaxReturn(tokens);
    }
    else if (tokens[0][0] == '#') //comment
    {
        //do nothing
    }
    else
    {
        //we catch unknown things on the runtime side
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
    //cout << vectorToString(code) << "==\n";
    int curLine=0; //which line we are curently executing
    while (curLine<code.size())
    {
        //cout << ss.nestDepth << " " << code[curLine] << "==\n";
        vector<string> tokens = tokenize(code[curLine]);
        //cout << code[curLine] << "-=";
        e = interpreter(ss, code, tokens, output, curLine);
        //cout << curLine << "=-\n";
        //cout << code[curLine] << "--" << getObjectByName(ss.definedVariables[0], "x").value << "--\n";
        if (e.errorPos != -1)
        {
            if (code.size() == 1) curLine = 0;
            output.err.push_back("Runtime error: line " + to_string(curLine) + "\nLine: " + code[curLine] + "\nToken: '" + tokens[e.errorPos] + "'\n\nGot error: " + e.message);
            //output.err.push_back(":(");
            return;
        }
    }
    //cout << vectorToString(code) << "==\n";
}


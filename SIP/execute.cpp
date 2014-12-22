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

set<string> keywords={ "var", "if", "while", "else", "for", "print" };

SaveState ss;

vector<string> execute(vector<string> code)
{
    cout << "executing...\n";
    
    vector<string> output;
    
    //syntax checking. We check the syntax first so we don't have to worry about it on execution
    for (int i=0; i<code.size(); i++)
    {
        string line=code[i];
        vector<string> tokens=tokenize(line);
        errVar syntaxError=checkSyntax(tokens);
        
        if (syntaxError.errorPos>=0) //add error messages
        {
            output.push_back("Syntax error: line " + to_string(i+1)+"\n");
            output.push_back("\"" + tokens[syntaxError.errorPos] + "\"" );
            output.push_back("Got error: " + syntaxError.message);
            return output;
        }
    }
    
    cout << "Syntax OK!\n";
    
    executeCode(code, output);
    
    return output;
}

vector<string> tokenize(string line) //split the line into words, spaces, equals signs, and whatever else
{
    vector<string> output;
    vector<string> splits = { " ", "=", "'", "\"", ";", "(", ")" };
    vector<string> removables = {" "};
    
    //split and seperate chunks (i.e tokenize them)
    output = seperateAll(line, splits);
    
    //recombine necessary parts
    recombine(output, "=");
    recombineBetween(output, "'");
    recombineBetween(output, "\"");
    
    removeVectorParts(output, removables);
    
    return output;
}

//the int this (and all other syntax functions) return the character position of the error or a value less than zero on no errors
errVar checkSyntax(vector<string> tokens) //returns first character that caused the issue
{
    errVar err;
    
    cout << "Checking syntax\n";
    if (tokens.size()==0)
    {
        cout << "No tokens found... quitting :(\n";
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
    
    if (tokens[0]=="for")
        err=syntaxFor(tokens);
    
    if (tokens[0]=="else" &&  tokens[1]=="if") //else if functions the same way as if I think
    {
        tokens.erase(tokens.begin());
    }
    
    if (tokens[0]=="if" || tokens[0]=="while")
    {
        err=syntaxIfWhile(tokens);
    }
    
    if (tokens[0] == "print")
    {
        err = syntaxPrint(tokens);
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

void executeCode(vector<string> code, vector<string> &output)
{
    cout << "Executing code\n";
    
    //actual execution
    int curLine=0; //which line we are curently executing
    while (curLine<code.size())
    {
        interpreter(ss, tokenize(code[curLine]));
    }
}


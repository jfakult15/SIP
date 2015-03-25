//
//  keyword_for.cpp
//  SIP
//
//  Created by Jacob Fakult on 12/20/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "keyword_for.h"

using namespace std;

//confirm syntax for for loops is valid
errVar syntaxFor(vector<string> tokens)
{
    errVar err;
    
    //there should be a variable declared in the for statement
    vector<string> varTokens(tokens.begin()+1, tokens.begin()+5);
    varTokens.push_back(";");
    
    err=syntaxVar(varTokens);
    if (err.errorPos>=0) //we had an error in the for variable declaration
    {
        err.errorPos+=1;
        err.message="Error in iterator declaration: "+err.message;
        return err;
    }
    
    if (tokens[5]!="to")
    {
        err.errorPos=5;
        err.message="Bad 'for' syntax. Did you mean 'to'?";
        return err;
    }
    
    Object forVar;
    forVar.value=tokens[6];
    forVar.type=determineType(forVar.value);
    
    if (forVar.type=="null")
    {
        err.errorPos=6;
        err.message="Couldn't determine data type";
        return err;
    }
    
    if (!forVar.isCorrectDataFormat())
    {
        err.errorPos=6;
        err.message="Bad " + forVar.type + " value";
        return err;
    }
    
    if (tokens.size()>7) //we are adding in a custom increment value (format: 'by x')
    {
        if (tokens[7]!="by")
        {
            err.errorPos=7;
            err.message="Invalid 'for' increment parameter: '" +tokens[7] + "'. Did you mean 'by'?";
            return err;
        }
        
        if (tokens.size()==8)
        {
            err.errorPos=0;
            err.message="For: missing step parameter";
            return err;
        }
        
        forVar.value=tokens[8];
        forVar.type=forVar.getType();
        if (forVar.type!="int" && forVar.type!="double")
        {
            err.errorPos=8;
            err.message="For 'by' requires integer or floating point parameter. Found: '"+forVar.type +"'";
            return err;
        }
    }
    
    return err;
}

struct errVar executeFor(vector<string> &line, vector<string> &code, ExecutionOutput &output, int &curLine, struct SaveState &ss)
{
    /*
     In reality all we are doing is converting this into a while loop
    */
    errVar e;
    
    string varName = line[2];
    string val = line[4];
    string endVal = line[6];
    string by = "1";
    
    if (line.size()>8) //have by parameter
    {
        by = line[8];
    }
    
    Object forVar("string", "for_var_garbage_666f725f7661725f67617262616765_1234567890987654321a", val); //not my proudest moment as a programmer...
    ss.definedVariables[0].insert(ss.definedVariables[0].begin(), forVar);
    
    string comp = "<=";
    string type = forVar.getType();
    if (type=="int" || type=="double")
    {
        double value = forVar.getDoubleValue();
        double endValue = 
    }
    
    int firstLine = curLine;
    int blockEnd = getClosingBraceLine(code, curLine+2, 0);
    code[curLine] = "while (for_var_garbage_666f725f7661725f67617262616765_1234567890987654321a " + comp + endVal + ")";
    cout << code[curLine] << "\n";
    code.insert(code.begin()+blockEnd, "for_var_garbage_666f725f7661725f67617262616765_1234567890987654321a += " + by);
    curLine--;
    
    return e;
}
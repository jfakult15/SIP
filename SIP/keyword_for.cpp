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
    
    //lets get rid of any extra parentheses just for good error checking. Will have to redo on execution (tokens not passed byref)
    while (tokens[1]=="(" && tokens[tokens.size()-1] == ")")
    {
        tokens.erase(tokens.end()-1);
        tokens.erase(tokens.begin()+1);
    }
    
    //there should be a variable declared in the for statement
    vector<string> varTokens(tokens.begin()+1, tokens.begin()+5);
    //varTokens.push_back(";");
    
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

/*
 In reality all we are doing is converting this into a while loop
 */
struct errVar executeFor(vector<string> &line, vector<string> &code, ExecutionOutput &output, int &curLine, struct SaveState &ss)
{
    errVar e;
    
    //lets get rid of any extra parentheses just for good error checking
    while (line[1]=="(" && line[line.size()-1] == ")")
    {
        line.erase(line.end()-1);
        line.erase(line.begin()+1);
    }
    
    string varName = line[2];
    string val = line[4];
    string endVal = line[6];
    string by = "1";
    
    if (line.size()>8) //have by parameter
    {
        by = line[8];
    }
    
    Object forVar("string", varName, val);
    
    string comp = "<=";
    string type = forVar.getType();
    if (type=="int" || type=="double")
    {
        double value = forVar.getDoubleValue();
        double endValue = Object("a", "a", endVal).getDoubleValue();
        double byValue = Object("a", "a", by).getDoubleValue();
        
        //need more error checking, friendly runtime errors
        if (byValue < 0)
        {
            comp = ">=";
        }
    }
    
    //remove semi
    int firstLine = curLine;
    int blockEnd = getClosingBraceLine(code, curLine+2, 0);
    code[curLine] = "while (" + varName + " " + comp + " " + endVal + ")";

    //execute({"var " + varName + " = " + val}, output, 0);
    code.insert(code.begin()+firstLine, "var " + varName + " = " + val);
    code.insert(code.begin()+blockEnd+1, varName + " = " + varName + " + " + by);

    //curLine -= 1; //no need for this because the insert shifts if for us
    //cout << vectorToString(code) << "--\n";
    
    e.message = varName;
    
    return e;
}
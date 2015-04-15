//
//  keyword_while_if.cpp
//  SIP
//
//  Created by Jacob Fakult on 12/20/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "keyword_while_if.h"
#include "execute.h"

using namespace std;

errVar syntaxIfWhile(vector<string> tokens)
{
    errVar err;
    /*if (tokens[1]!="(")
    {
        err.errorPos=1;
        err.message="Expected boolean expression";
        return err;
    }*/
    
    return err;
}

errVar executeIf(vector<string> &line, vector<string> &code, ExecutionOutput &output, int &curLine, SaveState &ss)
{
    string keyword = line[0];
    
    //lets get rid of any extra parentheses just for good error checking
    while (line[1]=="(" && line[line.size()-1] == ")")
    {
        line.erase(line.end()-1);
        line.erase(line.begin()+1);
    }
    
    line.erase(line.begin()); //we don't need keyword parts of the statements here
    errVar e = boolEval(line, ss);
    if (e.errorPos>=0)
    {
        //output.err.push_back(e.message);
        curLine++;
        return e;
    }
    else
    {
        curLine++;
        //trim(code[curLine]);
        if (code[curLine]!="{")
        {
            //cout << curLine << " " << code[curLine] << " " << vectorToString(code) << "==\n";
            //output.err.push_back("If statement requires block (starting with opening bracket)");
            e.message = "If statement requires block (starting with opening bracket)";
            e.errorPos = curLine;
            return e;
        }
        int blockEnd = getClosingBraceLine(code, curLine+1, 0);
        if (blockEnd==-1)
        {
            //output.err.push_back("If statement missing closing brace '}'");
            e.message = "If statement missing closing brace '}'";
            e.errorPos = curLine;
            return e;
        }
        curLine++;
        if (e.message=="false")
        {
            curLine=blockEnd+1;
            return e;
        }
        vector<string> block(code.begin()+curLine, code.begin()+blockEnd);
        ss.nestDepth++;
        //execute(block, output, 0);
        executeCode(block, output);
        un_nest(ss);
        curLine=blockEnd+1;
        //execute code in block!
    }
    
    return e;
}

errVar executeWhile(vector<string> &line, vector<string> &code, ExecutionOutput &output, int &curLine, SaveState &ss)
{
    int firstLine = curLine;
    string keyword = line[0];
    
    //lets get rid of any extra parentheses just for good error checking
    while (line[1]=="(" && line[line.size()-1] == ")")
    {
        line.erase(line.end()-1);
        line.erase(line.begin()+1);
    }
    
    line.erase(line.begin()); //we don't need keyword parts of the statements here
    errVar e;// = boolEval(line, ss);
    int blockEnd = getClosingBraceLine(code, curLine+2, 0);
    //cout << vectorToString(code) << "==" << blockEnd << "==\n";
    
    if (e.errorPos>=0)
    {
        return e;
    }
    int temp = 0;
    while (true)//e.message!="false")
    {
        errVar t; t.message = "true";
        //if (temp > 100000) t.message = "false";
        //e = t;//boolEval(line, ss);
        e = boolEval(line, ss);
        temp++;
        //cout << getAnyObjectNamed(ss.definedVariables, "x").value << " " << e.message << "\n";
        //cout << e.message << " " << vectorToString(line) << "--1\n";
        if (e.message != "true" && e.message != "false")
        {
            e.errorPos = curLine;
            curLine = blockEnd;
            return e;
        }
        //cout << e.message << " " << vectorToString(line) << "--\n";
        if (e.message!="true")
        {
            break;
        }
        
        //cout << getAnyObjectNamed(ss.definedVariables, "x").value << " " << e.message << "\n";
        
        curLine++;
        //trim(code[curLine]);
        //cout << vectorToString(code) << "==\n";
        if (code[curLine]!="{")
        {
            //output.err.push_back("If statement requires block (starting with opening bracket)");
            e.message = "While statement requires block (starting with opening bracket)";
            e.errorPos = curLine;
            return e;
        }
        blockEnd = getClosingBraceLine(code, curLine+1, 0);
        if (blockEnd==-1)
        {
            //output.err.push_back("If statement missing closing brace '}'");
            e.message = "While statement missing closing brace '}'";
            e.errorPos = curLine;
            return e;
        }
        curLine++;
        /*if (e.message=="false")
        {
            curLine=blockEnd+1;
            return e;
        }*/
        vector<string> block(code.begin()+curLine, code.begin()+blockEnd);
        ss.nestDepth++;
        //cout << getAnyObjectNamed(ss.definedVariables, "x").value << " " << e.message << "\n";
        //execute(block, output, 0);
        executeCode(block, output);
        un_nest(ss);
        //execute code in block!
        curLine = firstLine;
    }
    
    if (e.message!="false")
    {
        return e;
    }
    
    curLine = blockEnd + 1;
    return e;
}
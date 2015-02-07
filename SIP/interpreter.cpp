//
//  interpreter.cpp
//  SIP
//
//  Created by Jacob Fakult on 12/22/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "interpreter.h"

using namespace std;

void interpreter(SaveState &ss, vector<string> &code, vector<string> line, ExecutionOutput &output, int &curLine)
{
    analyzeLine(line, ss, output, curLine);
    curLine++;
    
}

void analyzeLine(vector<string> line, SaveState &ss, ExecutionOutput &output, int curLine)
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
    
    if (err.errorPos != -1)
    {
        output.err.push_back("Runtime error: line "+to_string(curLine+1));
        output.err.push_back("\"" + line[err.errorPos] + "\"\n" );
        output.err.push_back("Got error: " + err.message);
    }
}
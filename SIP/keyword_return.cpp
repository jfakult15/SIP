//
//  keyword_return.cpp
//  SIP
//
//  Created by Jacob Fakult on 3/21/15.
//  Copyright (c) 2015 jfakult15. All rights reserved.
//

#include "keyword_return.h"

errVar syntaxReturn(vector<string> tokens)
{
    errVar err;
    err.errorPos=-1;
    
    for (int i=1; i<tokens.size()-1; i++)
    {
        Object temp;
        temp.value = tokens[i];
        string value = temp.value;
        bool isString = false;
        
        if ((tokens[i][0]=='"' && tokens[i][tokens[i].length()-1]=='"') || (tokens[i][0]=='\'' && tokens[i][tokens[i].length()-1]=='\''))
        {
            isString = true;
        }
        if (!isString)
        {
            if (tokens[i] == "+")
            {
                continue;
            }
        }
    }
    
    if (tokens[tokens.size()-1] != ";")
    {
        err.errorPos = int(tokens.size())-1;
        err.message = "Missing a semicolon";
    }
    
    return err;
}

errVar executeReturn(vector<string> tokens, ExecutionOutput &output, SaveState &ss)
{
    errVar err;
    err.errorPos=-1;
    
    string printStr = "";
    
    bool concatFromString = false;;
    
    for (int i=1; i<tokens.size()-1; i++)
    {
        //cout << tokens[i] << "==\n";
        Object temp;
        temp.value = tokens[i];
        string value = temp.value;
        
        bool isString = ((tokens[i][0]=='"' && tokens[i][tokens[i].length()-1]=='"') || (tokens[i][0]=='\'' && tokens[i][tokens[i].length()-1]=='\''));
        
        //cout << tokens[i] << "==\n";
        if (isString)
        {
            //cout << tokens[i] << "--\n";
            concatFromString = true;
            printStr+=tokens[i].substr(1, tokens[i].length()-2);
            continue;
        }
        
        vector<string> eval;
        while (!isString && i<tokens.size()-1)
        {
            if (concatFromString)
            {
                concatFromString = false;
                i++;
                continue;
            }
            eval.push_back(tokens[i]);
            i++;
            isString = ((tokens[i][0]=='"' && tokens[i][tokens[i].length()-1]=='"') || (tokens[i][0]=='\'' && tokens[i][tokens[i].length()-1]=='\''));
        }
        i--;
        
        
        if (eval.size() > 0)
        {
            //cout << vectorToString(eval) << "==\n";
            Object o;
            errVar e = anyEval(eval, ss);
            //cout << e.message << "==\n";
            if (e.errorPos == -1)
            {
                printStr += e.message;
            }
            else
            {
                //output.err.push_back("Runtime error: \nToken: '" + eval[e.errorPos] + "'\n\nGot error: " + e.message);
                output.err.push_back("Runtime error: unable to evaluate return expression");
            }
        }
    }
    
    if (err.errorPos == -1)
    {
        output.returnVal = printStr;
    }
    return err;
}
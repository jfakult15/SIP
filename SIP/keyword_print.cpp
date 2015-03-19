//
//  keyword_print.cpp
//  SIP
//
//  Created by Jacob Fakult on 12/20/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "keyword_print.h"
#include "object.h"

using namespace std;

errVar syntaxPrint(vector<string> tokens)
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
            /*if (tokens[i]!="+")
            {
                err.errorPos = i;
                err.message = "Bad concatenation operator";
                return err;
            }
            else
            {
                err.errorPos = i;
                string type = temp.getType();
                err.message = "Unable to cast '" + type + "' to string";
                return err;
            }*/
        }
    }
    
    if (tokens[tokens.size()-1] != ";")
    {
        err.errorPos = int(tokens.size())-1;
        err.message = "Missing a semicolon";
    }
    
    return err;
}

errVar executePrint(vector<string> tokens, ExecutionOutput &output, SaveState &ss, bool lineBreak)
{
    errVar err;
    err.errorPos=-1;
    
    string printStr = "";
    
    bool concatFromString = false;;
    
    for (int i=1; i<tokens.size()-1; i++)
    {
        Object temp;
        temp.value = tokens[i];
        string value = temp.value;
        
        bool isString = ((tokens[i][0]=='"' && tokens[i][tokens[i].length()-1]=='"') || (tokens[i][0]=='\'' && tokens[i][tokens[i].length()-1]=='\''));
        
        if (isString)
        {
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
        
        
        if (eval.size() > 0)
        {
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
                output.err.push_back("Runtime error: unable to evaluate print expression");
            }
        }
    }
    
    if (err.errorPos == -1)
    {
        output.output.push_back(printStr);
        cout << printStr;
        if (lineBreak) cout << "\n";
    }
    /*else
    {
        output.err.push_back(err.message);
    }*/
    //cout << printStr;
    return err;
}




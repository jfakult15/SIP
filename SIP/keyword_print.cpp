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
    //cout << "hi\n";
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
                break;
            }
            if (tokens[i]!="+")
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
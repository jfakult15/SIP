//
//  one_word_statement.cpp
//  SIP
//
//  Created by Jacob Fakult on 12/22/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "one_word_statement.h"

using namespace std;

errVar syntaxOneWordStatement(vector<string> tokens)
{
    errVar err;
    err.errorPos=-1;
    
    if (tokens.size()>2)
    {
        err.message = "Extra token in " + tokens[0] + " statement. ";
        err.errorPos = 1;
    }
    if (tokens[tokens.size()-1] != ";")
    {
        if (err.message.length()==0)
        {
            err.errorPos = int(tokens.size())-1;
        }
        err.message += "\n Missing semicolon";
    }
    
    return err;
}
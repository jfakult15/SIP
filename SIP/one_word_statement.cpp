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
        err.message = "Unexpected token in " + tokens[0] + " statement";
        if (tokens[2]==";")
        {
            err.message += "\nSIP no longer uses semicolons";
        }
        err.errorPos = 1;
    }
    //remove semi
    
    return err;
}
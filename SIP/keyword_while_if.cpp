//
//  keyword_while_if.cpp
//  SIP
//
//  Created by Jacob Fakult on 12/20/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "keyword_while_if.h"

using namespace std;

errVar syntaxIfWhile(vector<string> tokens)
{
    errVar err;
    if (tokens[1]!="(")
    {
        err.errorPos=1;
        err.message="Expected boolean expression";
        return err;
    }
    
    return err;
}
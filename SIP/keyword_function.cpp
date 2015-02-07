//
//  keyword_function.cpp
//  SIP
//
//  Created by Jacob Fakult on 2/6/15.
//  Copyright (c) 2015 jfakult15. All rights reserved.
//

#include "keyword_function.h"

using namespace std;

errVar syntaxFunction(vector<string> tokens)
{
    //cout << "hi\n";
    errVar err;
    err.errorPos=-1;
    
    for (int i=1; i<tokens.size()-1; i++)
    {
        
    }
    
    if (tokens[tokens.size()-1] == ";")
    {
        err.errorPos = int(tokens.size())-1;
        err.message = "Function declarations should not have a semicolon";
    }
    
    return err;
}
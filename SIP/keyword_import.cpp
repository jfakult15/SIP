//
//  keyword_import.cpp
//  SIP
//
//  Created by Jacob Fakult on 3/20/15.
//  Copyright (c) 2015 jfakult15. All rights reserved.
//

#include "keyword_import.h"

errVar syntaxImport(vector<string> tokens)
{
    errVar e;
    
    bool isString = ((tokens[1][0]=='"' && tokens[1][tokens[1].length()-1]=='"') || (tokens[1][0]=='\'' && tokens[1][tokens[1].length()-1]=='\'')) && tokens[1].length() > 1;
    if (!isString)
    {
        e.errorPos = 1;
        e.message = "Import statement expects string parameter";
        return e;
    }
    
    if (tokens[2] != ";")
    {
        e.errorPos = 2;
        e.message = "Import statement expects semicolon (;)";
    }
    
    return e;
}

errVar executeImport(vector<string> tokens, SaveState &ss)
{
    errVar e;
    
    
    
    return e;
}
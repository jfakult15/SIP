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
    
    //remove semi
    /*
    if (tokens[2] != ";")
    {
        e.errorPos = 2;
        e.message = "Import statement expects semicolon (;)";
    }*/
    
    return e;
}

errVar executeImport(vector<string> tokens, ExecutionOutput &output, SaveState &ss)
{
    errVar e;
    
    executeFile(tokens[1], output);
    
    return e;
}

errVar executeFile(string path, ExecutionOutput &output)
{
    errVar e;
    
    vector<string> input;
    
    path = path.substr(1, path.length()-2); //remove enclosing quotations
    
    if (path.length() < 5 || path.substr(path.length()-4, 4) != ".sip")
    {
        path += ".sip";
    }
    
    ifstream file;
    file.open("./import/"+path);
    
    if (!file.is_open())
    {
        if (path == "sip_core.sip")
        {
            output.warnings.push_back("Unable to locate file 'sip_core.sip' in directory 'import'.\nThis directory must be in the same location as the SIP executable");
        }
        
        file.open(path);
    }
    
    if (!file.is_open())
    {
        e.errorPos = 1;
        e.message = "Unable to find import file with error: " + string(strerror(errno));
        return e;
    }
    
    input=readFile(file);
    execute(input, output, 0);
    if (verbose && output.info.size()>0)
    {
        for (int i=0; i<output.info.size(); i++)
        {
            cout << output.info[i] << "\n";
        }
    }
    if (verbose && output.warnings.size()>0)
    {
        for (int i=0; i<output.warnings.size(); i++)
        {
            cout << output.warnings[i] << "\n";
        }
    }
    if (output.err.size()>0)
    {
        for (int i=0; i<output.err.size(); i++)
        {
            cout << output.err[i] << "\n";
        }
        exit(0);
    }
    
    file.close();
    
    return e;
}
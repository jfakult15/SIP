//
//  execute.cpp
//  SIP
//
//  Created by Jacob Fakult on 9/10/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "execute.h"
#include "object.h"

#include <iostream>
#include <vector>
#include <set>

using namespace std;

int checkSyntax(vector<string> line);

set<string> keywords={ "var", "if", "while", "else", "for" };
vector<string> tokenize(string line);
vector<Object> objects;
set<string> objectNames;

vector<string> execute(vector<string> code)
{
    return tokenize(code[0]);
    vector<string> output;
    
    for (int i=0; i<code.size(); i++)
    {
        string line=code[i];
        vector<string> tokens=tokenize(line);
        int syntaxError=checkSyntax(tokens);
        if (syntaxError>=0)
        {
            output={"Syntax error: line " + to_string(i+1)+"\n"};
            output.push_back(tokens[syntaxError]);
            return output;
        }
    }
    
    return output;
}

vector<string> tokenize(string line) //split the line into words, spaces, equals signs, and whatever else
{
    vector<string> output;
    output.push_back("");
    
    for (int i=0; i<line.length(); i++)
    {
        int ch = toupper(int(line[i]));
        
        if (ch==92) //backquote or escape character
        {
            output[output.size()-1]+=line[i];
            continue;
        }
        
        if ((ch>=65 && ch<=97) || (ch>=48 && ch<=57) || ch==46 || ch==39 || ch==34) //46=. 34=" 39='
        {
            output[output.size()-1]+=line[i];
        }
        else
        {
            if (ch!=32 && output[output.size()-1].length()>0)
                output.push_back(string(1,line[i]));
            else if (ch!=32) //sloppy, I know. Trims whitespace
                output[output.size()-1]+=string(1,line[i]);
            
            output.push_back("");
        }
    }
    output.pop_back();
    
    //for (int i=0; i<output.size(); i++) cout << "'" << output[i] << "'\n";
    
    return output;
}

int checkSyntax(vector<string> tokens) //returns first character that caused the issue
{
    if (keywords.find(tokens[0])==keywords.end() && objectNames.find(tokens[0])==objectNames.end()) //the first word isnt a keyword or a variable
    {
        return 0;
    }
    
    
    
    return -1;
}





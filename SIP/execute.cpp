//
//  execute.cpp
//  SIP
//
//  Created by Jacob Fakult on 9/10/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "execute.h"
#include "object.h"
//#include "err.h"

#include <iostream>
#include <vector>
#include <set>

using namespace std;

set<string> keywords={ "var", "if", "while", "else", "for" };

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
        errVar syntaxError=checkSyntax(tokens);
        if (syntaxError.errorPos>=0) //add error messages
        {
            output={"Syntax error: line " + to_string(i+1)+"\n"};
            output.push_back('"' + tokens[syntaxError.errorPos] + '"' );
            output.push_back("Got error: " + syntaxError.message);
            return output;
        }
    }
    
    return output;
}

vector<string> tokenize(string line) //split the line into words, spaces, equals signs, and whatever else
{
    removeSpaces(line);
    //cout << "Line is: '" << line << "'\n";
    
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
        
        int minus=1;
        while (minus<output.size() && output[output.size()-minus].length()==0)
        {
            minus++;
        }
        int lch=toupper(int(output[output.size()-minus][output[output.size()-1].length()-1])); //lastChar
        //cout << lch << "--\n";
        
        //cout << lastChar << " " << output[output.size()-1] << "--\n";
        //cout << char(lch) << " " << char(ch) << "--\n";
        
        if (isLetter(ch) || (ch>=48 && ch<=57) || ch==46 || ch==39 || ch==34 || char(ch)=='=') //46=. 34=" 39='
        {
            if (char(ch)=='=' && char(lch)!='=')
                output.push_back("");
            
            if (ch==lch && char(ch)=='=')
                output[output.size()-1]+=line[i];
            else if (((lch>90 || lch<65) && (lch<48 || lch>57)) && (lch!=46 && lch!=34 && lch!=39)) //output.size()>0 && output[output.size()-1].length()==0)
                output.push_back(string(1,line[i]));
            else
                output[output.size()-1]+=line[i];
        }
        //else if (i<line.length() && line[i]=='=' && lastChar=='=')
        //{
        //    output[output.size()-1]+=line[i];
        //}
        else if (ch!=32)
        {
            //cout << "last char1: '" << output[output.size()-1] << " " << line[i] << "'\n";
            if (output[output.size()-1].length()>0)
                output.push_back(string(1,line[i]));
            
            //output.push_back("");
        }
        else if (output.size()>0 && output[output.size()-1].length()>0)
            output.push_back("");
        
        //cout << "last char: '" << output[output.size()-1] << "'\n";
    }
    
    output.erase(output.begin());
    
    //for (int i=0; i<output.size(); i++) cout << "'" << output[i] << "'\n";
    
    return output;
}

void removeSpaces(string &line) //remove spaces between non-letter characters
{
    for (int i=0; i<line.length()-2; i++)
    {
        int thisCh=toupper(line[i]);
        int nextCh=toupper(line[i+1]);
        int twoCh=toupper(line[i+2]);
        
        if (!isLetter(thisCh) && !isNumber(thisCh) && nextCh==32)
        {
            line=line.substr(0,i+1)+line.substr(i+2);
            i--;
        }
        
        if (isLetter(thisCh) || isNumber(thisCh))
        {
            if (nextCh==32)
            {
                if (!isLetter(twoCh) && !isNumber(twoCh))
                {
                    line=line.substr(0,i+1)+line.substr(i+2);
                    i--;
                }
            }
        }
    }
}
            
bool isNumber(int ch)
{
    return toupper(ch)<=57 && toupper(ch)>=48;
}

bool isLetter(int ch)
{
    return toupper(ch)<=90 && toupper(ch)>=65;
}

//the int this (and all other syntax functions) return the character position of the error or a value less than zero on no errors
errVar checkSyntax(vector<string> tokens) //returns first character that caused the issue
{
    errVar err;
    if (keywords.find(tokens[0])==keywords.end() && objectNames.find(tokens[0])==objectNames.end()) //the first word isnt a keyword or a variable
    {
        return err;
    }
    
    if (tokens[0]=="var")
    {
        err=syntaxVar(tokens);
    }
    
    if (tokens[0]=="else" &&  tokens[1]=="if") //else if functions the same way as if I think
    {
        tokens.erase(tokens.begin());
    }
    
    if (tokens[0]=="if" || tokens[0]=="while")
    {
        err=syntaxIfWhile(tokens);
    }
    
    return err;
}

errVar syntaxVar(vector<string> tokens)
{
    errVar err;
    err.errorPos=-1;
    
    if (tokens[tokens.size()]!=";")
    {
        err.errorPos=tokens.size()-1;
        err.message="Did you forget a semicolon? (;)";
        return err;
    }
    if (tokens[tokens.size()]!=";")
    {
        err.errorPos=tokens.size()-1;
        err.message="Did you forget a semicolon? (;)";
        return err;
    }
    
    return err;
}

errVar syntaxIfWhile(vector<string> tokens)
{
    errVar err;
    return err;
}

bool boolEval(vector<string> line)
{
    return true;
}



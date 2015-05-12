//
//  main.cpp
//  SIP -- Students Introduction to Programming
//
//  Created by Jacob Fakult on 9/10/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "execute.h"
#include "object.h"
#include "err.h"
#include "helpers.h"
#include "levenshtein.h"
#include "io.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

#include<cstdlib>
#include<string.h>
#include<dirent.h>

using namespace std;

vector<string> input;
ExecutionOutput output;
//vector<Object> s;
vector<string> readFile(ifstream &file);
int importAll(vector<string> &input);

int main(int argc, const char * argv[])
{
    /*
    SaveState temp;
    ExecutionOutput output;
    vector<string> code;
    errVar e = anyEval(tokenize("((5+6) + 1==1)+2"), temp, output, code);
    cout << e.message << "--\n";
    exit(0);*/
    
    argc = 2;
    argv[1] = "/Users/jfakult/Desktop/temp.sip";
    //I am trying to be flexible with command line errors. Ill check every arg until one is a good file
    for (int i=1; i<argc; i++)
    {
        ifstream file(argv[1]);
        if (file.is_open())
        {
            input=readFile(file);
            int numImports = importAll(input);
            fullCode = input;
            //input.insert(input.begin(), "import \"sip_core.sip\";");
            
            execute(input, output, numImports);
            
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
            }
            else //everything is printed dynamically?
            {
                /*for (int i=0; i<output.output.size(); i++)
                {
                    cout << output.output[i] << "\n";
                }*/
            }
            
            return 0; //we will exit as soon as we find and run a readable file
        }
        file.close();
    }
    
    //if we found a good file, the program will have terminated by this point
    //hence we couldn't find the file in any of the parameters
    cout << "Couldn't find a file!\n";
    
    return 0;
}

int importAll(vector<string> &input)
{
    int num=0;
    DIR *pDIR;
    struct dirent *entry;
    if((pDIR=opendir("import")))
    {
        while((entry = readdir(pDIR)))
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && entry->d_name[0] != '.' )
            {
                input.insert(input.begin(), "import \"" + string(entry->d_name) + "\"");
                num++;
            }
        }
        closedir(pDIR);
    }
    return num;
}
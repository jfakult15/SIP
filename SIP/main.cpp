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

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

using namespace std;

vector<string> input;
ExecutionOutput output;
//vector<Object> s;
vector<string> readFile(ifstream &file);

int main(int argc, const char * argv[])
{
    bool verbose = false;
    
    vector<string> temp = tokenize("((7+1>5) && true && (5+(2*6)>3)|| ('byebye'=='hello'))");
    SaveState ss;
    boolEval(temp, ss);
    exit(0);
    
    argc = 2;
    argv[1] = "/Users/jfakult/Desktop/SIP_test.txt";
    //I am trying to be flexible with command line errors. Ill check every arg until one is a good file
    for (int i=1; i<argc; i++)
    {
        ifstream file(argv[1]);
        if (file.is_open())
        {
            input=readFile(file);
            execute(input, output/*, verbose*/);
            
            if (verbose)
            {
                for (int i=0; i<output.info.size(); i++)
                {
                    cout << output.info[i] << "\n";
                }
            }
            if (output.warnings.size()>0)
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
            else
            {
                for (int i=0; i<output.output.size(); i++)
                {
                    cout << output.output[i] << "\n";
                }
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

vector<string> readFile(ifstream &file)
{
    vector<string> tempLines;
    while (!file.eof())
    {
        string temp;
        getline(file, temp);
        
        if (temp.length()>0)
        {
            tempLines.push_back(temp);
        }
    }
    return tempLines;
}
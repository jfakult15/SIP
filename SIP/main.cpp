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

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

using namespace std;

vector<string> input;
vector<string> output;
vector<Object> s;
vector<string> readFile(ifstream &file);


int main(int argc, const char * argv[])
{
    input={"var x = 10;"};
    output=execute(input);
    
    for (int i=0; i<output.size(); i++)
    {
        cout << output[i] << "\n";
    }
    return 0;
    
    if (argc==1)
    {
        cout << "No file provided" << "\n";
        return 1;
    }
    //I am trying to be flexible with command line errors. Ill check every arg until one is a good file
    for (int i=1; i<argc; i++)
    {
        ifstream file(argv[1]);
        if (file.is_open())
        {
            input=readFile(file);
            input={"var x=10;"};
            output=execute(input);
            
            return 0; //we will exit as soon as we find a readable file
        }
        /*else
        {
            cout << "Couldn't find it!\n";
        }*/
        file.close();
    }
    
    //if we found a good file, the program will have terminated by this point
    //hence we couldn't find the file in any of the parameters
    cout << "Couldn't find it!\n";
    
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
//
//  main.cpp
//  SIP
//
//  Created by Jacob Fakult on 9/10/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<string> lines;
vector<string> readFile(ifstream &file);

int main(int argc, const char * argv[])
{
    if (argc==1)
    {
        cout << "Usage: stuff" << "\n";
        return 1;
    }
    
    for (int i=1; i<argc; i++)
    {
        ifstream file(argv[1]);
        if (file.is_open())
        {
            lines=readFile(file);
            return 0; //we will exit as soon as we find a
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
            cout << temp << "\n";
            tempLines.push_back(temp);
        }
    }
    return tempLines;
}
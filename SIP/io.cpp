//
//  io.cpp
//  SIP
//
//  Created by Jacob Fakult on 3/20/15.
//  Copyright (c) 2015 jfakult15. All rights reserved.
//

#include "io.h"

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
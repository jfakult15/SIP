//
//  object.h
//  SIP
//
//  Created by Jacob Fakult on 9/17/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#ifndef __SIP__object__
#define __SIP__object__

#include <iostream>

using namespace std;

class Object
{
public:
    Object();
    Object(string type, string value);
    string getType();
    int getIntValue();
    double getDoubleValue();
    string getStringValue();
    char getCharValue();
    bool getBoolValue();
    
private:
    string type;
    string value;
};

#endif /* defined(__SIP__object__) */

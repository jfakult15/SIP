//
//  err.h
//  SIP
//
//  Created by Jacob Fakult on 9/17/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#ifndef __SIP__err__
#define __SIP__err__

#include <iostream>

using namespace std;

//using namespace std;

void err(string e);

struct errVar
{
    int errorPos=-1;
    string message;
};

#endif /* defined(__SIP__err__) */

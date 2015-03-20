//
//  io.h
//  SIP
//
//  Created by Jacob Fakult on 3/20/15.
//  Copyright (c) 2015 jfakult15. All rights reserved.
//

#ifndef __SIP__io__
#define __SIP__io__

#include <stdio.h>

#include <vector>
#include <fstream>

using namespace std;

vector<string> readFile(ifstream &file);

#endif /* defined(__SIP__io__) */

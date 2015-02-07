//
//  execution_startup_manager.h
//  SIP
//
//  Created by Jacob Fakult on 2/6/15.
//  Copyright (c) 2015 jfakult15. All rights reserved.
//

#ifndef __SIP__execution_startup_manager__
#define __SIP__execution_startup_manager__

#include <stdio.h>
#include <vector>

using namespace std;

void insertImportFunctions(vector<string> &code);
void parseFunctions(vector<string> &code);

#endif /* defined(__SIP__execution_startup_manager__) */

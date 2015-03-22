//
//  keyword_return.h
//  SIP
//
//  Created by Jacob Fakult on 3/21/15.
//  Copyright (c) 2015 jfakult15. All rights reserved.
//

#ifndef __SIP__keyword_return__
#define __SIP__keyword_return__

#include <stdio.h>

#include <vector>
#include <string>

#include "output.h"
#include "save_state.h"

using namespace std;

errVar syntaxReturn(vector<string> tokens);
errVar executeReturn(vector<string> tokens, ExecutionOutput &output, SaveState &ss);

#endif /* defined(__SIP__keyword_return__) */

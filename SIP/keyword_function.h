//
//  keyword_function.h
//  SIP
//
//  Created by Jacob Fakult on 2/6/15.
//  Copyright (c) 2015 jfakult15. All rights reserved.
//

#ifndef __SIP__keyword_function__
#define __SIP__keyword_function__

#include <stdio.h>
#include <iostream>
#include <vector>

#include "err.h"
#include "keyword_var.h"

struct errVar syntaxFunction(vector<string> tokens);
struct errVar executeFunction(vector<string> &line, vector<string> &code, ExecutionOutput &output, int &curLine, SaveState &ss);
struct errVar createFunction(vector<string> line, int firstLine, int blockEnd, struct SaveState &ss);
bool funcExists(string name, int numParams, struct SaveState &ss);
bool seenFuncBefore(string name, int startLine, struct SaveState &ss);
class FunctionObject getFunctionNamed(string name, struct SaveState &ss);
struct errVar createTempVars(vector<string> line, vector<string> funcLine, SaveState &ss);

#endif /* defined(__SIP__keyword_function__) */

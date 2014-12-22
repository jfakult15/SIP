//
//  execute.h
//  SIP
//
//  Created by Jacob Fakult on 9/10/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#ifndef __SIP__execute__
#define __SIP__execute__

#include <iostream>
#include <vector>
#include <set>

#include "err.h"
#include "object.h"
#include "save_state.h"
#include "interpreter.h"
#include "execute.h"
#include "keywords.h"
#include "output.h"

ExecutionOutput execute(vector<string> code);
int executeLine(string line, int curLine);
errVar checkSyntax(vector<string> line);
vector<string> tokenize(string line);
int getObjectNamed(vector<Object> &objects, string name);
void executeCode(vector<string> code, vector<string> &output);

#endif /* defined(__SIP__execute__) */

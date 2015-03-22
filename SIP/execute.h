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
#include <map>

#include "err.h"
#include "object.h"
#include "save_state.h"
#include "interpreter.h"
#include "execute.h"
#include "keywords.h"
#include "output.h"
#include "keyword_object.h"

void execute(vector<string> code, ExecutionOutput &output);
int executeLine(string line, int curLine);
struct errVar checkSyntax(vector<string> line, ExecutionOutput &output);
int getObjectNamed(vector<class Object> &objects, string name);
void executeCode(vector<string> code, ExecutionOutput &output);

extern bool verbose;

#endif /* defined(__SIP__execute__) */

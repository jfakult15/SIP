//
//  interpreter.h
//  SIP
//
//  Created by Jacob Fakult on 12/22/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#ifndef __SIP__interpreter__
#define __SIP__interpreter__

#include <stdio.h>
#include <vector>
#include "save_state.h"
#include "output.h"
#include "err.h"
#include "keywords.h"

void interpreter(SaveState &ss, vector<string> &code, vector<string> line, ExecutionOutput &output, int &curLine);
void analyzeLine(vector<string> line, SaveState &ss, ExecutionOutput &output, int curLine);

#endif /* defined(__SIP__interpreter__) */
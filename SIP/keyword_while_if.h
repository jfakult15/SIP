//
//  keyword_while_if.h
//  SIP
//
//  Created by Jacob Fakult on 12/20/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#ifndef __SIP__keyword_while_if__
#define __SIP__keyword_while_if__

#include <stdio.h>
#include "err.h"
#include <vector>

errVar syntaxIfWhile(vector<string> tokens);
errVar executeIf(vector<string> &line, vector<string> &code, ExecutionOutput &output, int &curLine, SaveState &ss);
errVar executeWhile(vector<string> &line, vector<string> &code, ExecutionOutput &output, int &curLine, SaveState &ss);

#endif /* defined(__SIP__keyword_while_if__) */

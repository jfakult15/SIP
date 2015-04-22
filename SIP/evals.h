//
//  evals.h
//  SIP
//
//  Created by Jacob Fakult on 4/22/15.
//  Copyright (c) 2015 jfakult15. All rights reserved.
//

#ifndef __SIP__evals__
#define __SIP__evals__

#include <stdio.h>
#include <cmath>
#include <vector>

#include "err.h"
#include "save_state.h"
#include "keywords.h"
#include "object.h"

struct errVar boolEval(vector<string> expr, struct SaveState &ss); //-1 for error, 0 for false, 1 for true
string eval(string val);
string findAfter(string val, int pos);
string findBefore(string val, int pos);
//bool compare(string left, string right, string comparator, string type);
errVar compare(string left, string right, string comp);
errVar anyEval(vector<string> expr, SaveState &ss, ExecutionOutput &output, vector<string> &code);
errVar stringEval(vector<string> expr, SaveState &ss);
string determineEvalType(vector<string> expr, SaveState &ss);

#endif /* defined(__SIP__evals__) */

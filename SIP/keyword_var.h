//
//  keyword_var.h
//  SIP
//
//  Created by Jacob Fakult on 12/20/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#ifndef __SIP__keyword_var__
#define __SIP__keyword_var__

#include <stdio.h>
#include <vector>

#include "err.h"
#include "object.h"
#include "save_state.h"

struct errVar syntaxVar(vector<string> tokens);
struct errVar executeVar(vector<string> tokens, struct SaveState &ss, ExecutionOutput &output, vector<string> &code);

#endif /* defined(__SIP__keyword_var__) */

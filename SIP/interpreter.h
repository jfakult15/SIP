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
//#include "keywords.h"

void interpreter(SaveState &ss, vector<string> line, ExecutionOutput &output);

#endif /* defined(__SIP__interpreter__) */

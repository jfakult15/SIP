//
//  keyword_print.h
//  SIP
//
//  Created by Jacob Fakult on 12/20/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#ifndef __SIP__keyword_print__
#define __SIP__keyword_print__

#include <stdio.h>
#include <vector>

#include "err.h"
#include "object.h"

errVar syntaxPrint(vector<string> tokens);
errVar executePrint(vector<string> tokens, ExecutionOutput &output, SaveState &ss);

#endif /* defined(__SIP__keyword_print__) */
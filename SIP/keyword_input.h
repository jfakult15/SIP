//
//  keyword_input.h
//  SIP
//
//  Created by Jacob Fakult on 2/9/15.
//  Copyright (c) 2015 jfakult15. All rights reserved.
//

#ifndef __SIP__keyword_input__
#define __SIP__keyword_input__

#include <stdio.h>

#include <vector>

#include "err.h"

errVar syntaxInput(vector<string> tokens);
errVar executeInput(vector<string> tokens, SaveState &ss);

#endif /* defined(__SIP__keyword_input__) */

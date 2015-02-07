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

errVar syntaxVar(vector<string> tokens);
errVar executeVar(vector<string> tokens, SaveState &ss);

#endif /* defined(__SIP__keyword_var__) */

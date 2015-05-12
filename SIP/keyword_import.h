//
//  keyword_import.h
//  SIP
//
//  Created by Jacob Fakult on 3/20/15.
//  Copyright (c) 2015 jfakult15. All rights reserved.
//

#ifndef __SIP__keyword_import__
#define __SIP__keyword_import__

#include <stdio.h>

#include <iostream>

#include "execute.h"
#include "err.h"
#include "io.h"

errVar syntaxImport(vector<string> tokens);
errVar executeImport(vector<string> tokens, ExecutionOutput &output, SaveState &ss, int curLine);
errVar executeFile(string path, ExecutionOutput &output, int &curLine);

#endif /* defined(__SIP__keyword_import__) */

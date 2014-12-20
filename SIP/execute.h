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
#include "err.h"
#include "object.h"

using namespace std;

vector<string> execute(vector<string> code);
int executeLine(string line, int curLine);


errVar syntaxIfWhile(vector<string> tokens);
errVar checkSyntax(vector<string> line);
errVar syntaxVar(vector<string> tokens);
errVar syntaxFor(vector<string> tokens);
errVar syntaxIfWhile(vector<string> tokens);

vector<string> tokenize(string line);

int getObjectNamed(vector<Object> &objects, string name);

#endif /* defined(__SIP__execute__) */

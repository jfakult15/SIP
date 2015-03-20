//
//  levenshtein.h
//  SIP
//
//  Created by Jacob Fakult on 3/20/15.
//  Copyright (c) 2015 jfakult15. All rights reserved.
//

#ifndef __SIP__levenshtein__
#define __SIP__levenshtein__

#include <stdio.h>

#include <vector>
#include <iostream>

using namespace std;

string levenshtein(string s1, vector<string> choices, int threshhold); //match s1 to closest string if it is closer than threshhold


#endif /* defined(__SIP__levenshtein__) */

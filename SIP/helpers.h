//
//  helpers.h
//  SIP
//
//  Created by Jacob Fakult on 10/8/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#ifndef __SIP__helpers__
#define __SIP__helpers__

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

string parseString(string str); //parse backquotes and whatnot. Returns "" if error
int boolEval(string expr); //-1 for error, 0 for false, 1 for true
string eval(string val);
string findAfter(string val, int pos);
string findBefore(string val, int pos);
bool isNum(string num);
vector<string> split(string str, string delim);
vector<string> seperateAll(string str, vector<string> delims);
void removeVectorParts(vector<string> &strings, vector<string> delims);
void trim(string &str);
bool isLetter(int ch);
bool isNumber(int ch);
void recombine(vector<string> &output, string str);
void recombineBetween(vector<string> &output, string str);

#endif /* defined(__SIP__helpers__) */
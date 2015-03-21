//
//  helpers.h
//  SIP
//
//  Created by Jacob Fakult on 10/8/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#ifndef __SIP__helpers__
#define __SIP__helpers__

#include <stdio.h>
#include <iostream>

#include <cmath>
#include <vector>

#include "err.h"
#include "save_state.h"

//using namespace std;

string parseString(string str); //parse backquotes and whatnot. Returns "" if error
struct errVar boolEval(vector<string> expr, struct SaveState &ss); //-1 for error, 0 for false, 1 for true
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
void recombine(vector<string> &output, string str1);
void recombineBetween(vector<string> &output, string str, bool hasEscapeQuote);
int countOfChars(string str, string match);
string getFirstParentheses(string val);
vector<string> tokenize(string line);
bool compare(string left, string right, string comparator, string type);
int getClosingBraceLine(vector<string> &code, int curLine, int startPos);
errVar anyEval(vector<string> expr, SaveState &ss);
errVar stringEval(vector<string> expr, SaveState &ss);
string determineEvalType(vector<string> expr, SaveState &ss);
string vectorToString(vector<string> v);
string toLowerCase(string s);
bool isAssignment(vector<string> line);

#endif /* defined(__SIP__helpers__) */

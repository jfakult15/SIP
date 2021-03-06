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

#include "evals.h"

//using namespace std;

string parseString(string str); //parse backquotes and whatnot. Returns "" if error
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
int getClosingBraceLine(vector<string> &code, int curLine, int startPos);
string vectorToString(vector<string> v);
string toLowerCase(string s);
bool isAssignment(vector<string> line);
bool clean(vector<class Object> &o);
bool un_nest(class SaveState &ss);
bool nest(class SaveState &ss);

#endif /* defined(__SIP__helpers__) */

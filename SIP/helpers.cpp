//
//  helpers.cpp
//  SIP
//
//  Created by Jacob Fakult on 10/8/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "helpers.h"
#include <cmath>

using namespace std;

string parseString(string str)
{
    return "hi";
}

string vectorToString(vector<string> v)
{
    string temp="";
    for (int i=0; i<v.size(); i++)
    {
        temp+=v[i];
    }
    return temp;
}

bool isComparator(string c)
{
    return c=="==" || c=="!=" || c=="<" || c==">" || c=="<=" || c==">=";
}

bool isMathFunc(string c)
{
    return c=="+" || c=="-" || c=="*" || c=="/" || c=="^";
}

string determineEvalType(vector<string> expr, SaveState &ss)
{
    errVar e;
    
    string lastType="bool"; //lowest level data type according to the casting hierarchy defined in object.cpp
    bool currentIsVar=true; //in format e.g x+5+6+y (we are changing between values and concatenation symbols (i.e '+')
    
    for (int i=0; i<expr.size(); i++)
    {
        if (currentIsVar)
        {
            string val = expr[i];
            
            if (val=="(" || val==")")
            {
                continue;
            }
            
            if (isProperVarName(val))
            {
                Object o = getAnyObjectNamed(ss.definedVariables, val, ss.nestDepth);
                if (o.name != "invalid object name")
                {
                    val = o.value;
                }
            }
            Object o;
            o.value = val;
            o.type = o.getType();
            if (objectTypeValue(o.type) > objectTypeValue(lastType))
            {
                lastType = o.type;
            }
        }
        else
        {
            string v = expr[i];
            //cout << v << "--\n";
            if (v!="+" && v!="-" && v!="*" && v!="/" && v!="^" && v!="+=" && v!="-=" && v!="*=" && v!="/=" && v!="^=")
            {
                lastType = "Couldn't parse expression";
            }
        }
        currentIsVar = !currentIsVar;
    }
    
    return lastType;
}

string getFirstParentheses(string val) //under the assumption that val[0] == "("
{
    val = val.substr(1);
    int openCount = 1;
    int closeCount = 0;
    int pos=0;
    while (pos<val.length() && openCount>closeCount)
    {
        if (val[pos]=='(')
        {
            openCount++;
        }
        else if (val[pos]==')')
        {
            closeCount++;
        }
        pos++;
    }
    
    return val.substr(0, pos-1);
}

vector<string> split(string str, string delim)
{
    vector<string> parts;
    
    for (int i=0; i<str.length(); i++)
    {
        int pos=int(str.find(delim, i));
        if (pos != -1)
        {
            parts.push_back(str.substr(0,pos));
            i--;
            str=str.substr(pos+delim.length());
        }
        else
        {
            parts.push_back(str);
            str="";
        }
    }
    
    return parts;
}

vector<string> seperateAll(string str, vector<string> delims)
{
    vector<string> output;
    
    int pos = 0;
    while (pos<str.length())
    {
        //cout << str.substr(pos, delims[j].length()) << " " << delims[j] << "--\n";
        for (int j=0; j<delims.size(); j++)
        {
            if (str.length()>=pos+delims[j].length() && str.substr(pos, delims[j].length())==delims[j])
            {
                if (pos>=delims[j].length()) //we have a part on the front (before the delimiter)
                {
                    output.push_back(str.substr(0, pos));
                    trim(output[output.size()-1]);
                }
                output.push_back(delims[j]);
                str=str.substr(pos+delims[j].length());
                pos=0;
                j=-1; //so it goes back to 0 on the loop around. Weird trick, never done this before!
            }
        }
        pos++;
        //cout << pos << "\n";
    }
    
    if (str.length()>0)
    {
        trim(str);
        output.push_back(str);
    }
    
    return output;
}

void removeVectorParts(vector<string> &strings, vector<string> delims)
{
    for (int i=int(delims.size())-1; i>=0; i--)
    {
        auto findPos=find(strings.begin(), strings.end(), delims[i]);
        while (findPos!=strings.end())
        {
            int pos = int(distance(strings.begin(), findPos));
            strings.erase(strings.begin()+pos);
            findPos=find(strings.begin(), strings.end(), delims[i]);
        }
    }
}

void trim(string &str)
{
    if (str.length()==0)
    {
        return;
    }
    str.erase(0, str.find_first_not_of(' ')); //spaces
    str.erase(str.find_last_not_of(' ')+1);
    str.erase(0, str.find_first_not_of(9)); //tabs
    str.erase(str.find_last_not_of(9)+1);
}

bool isNumber(int ch)
{
    return toupper(ch)<=57 && toupper(ch)>=48;
}

bool isLetter(int ch)
{
    return toupper(ch)<=90 && toupper(ch)>=65;
}

int countOfChars(string str, string match)
{
    int count = 0;
    for (int i=0; i<str.length()-match.length()+1; i++)
    {
        if (str.substr(i, match.length())==match)
        {
            count++;
        }
    }
    
    return count;
}

void recombine(vector<string> &output, string str1, string str2)
{
    if (output.size()<=1) //nothing to combine
    {
        return;
    }
    
    string lastStr = output[0];
    string thisStr;// = output[1];
    
    for (int i=1; i<output.size(); i++)
    {
        thisStr = output[i];
        if (lastStr == str1 && thisStr == str2)
        {
            output[i-1] += str2;
            output.erase(output.begin()+i);
        }
        
        lastStr = thisStr;
    }
}

void recombineBetween(vector<string> &output, string str, bool hasEscapeQuote) //for example combine all parts between two quotes (if str=='"')
{
    bool delimOpen = false;
    for (int i=0; i<output.size()-1; i++)
    {
        bool escape = (hasEscapeQuote && i>0);
        if (output[i] == str && find(output.begin()+i+1, output.end(), str)!=output.end())
        {
            if (escape)
            {
                if (output[i-1]!="\\")
                {
                    delimOpen = true;
                }
            }
            else
            {
                delimOpen = true;
            }
        }
        else if (delimOpen && output[i+1] == str)
        {
            if (escape)
            {
                if (output[i][output[i].length()-1] != '\\')
                {
                    delimOpen = false;
                    output[i]+=output[i+1];
                    output.erase(output.begin()+(i+1));
                    i--;
                }
            }
            else
            {
                delimOpen = false;
                output[i]+=output[i+1];
                output.erase(output.begin()+(i+1));
                i--;
            }
        }
        
        if (delimOpen)
        {
            output[i]+=output[i+1];
            output.erase(output.begin()+(i+1));
            i--;
        }
    }
}

vector<string> tokenize(string line) //split the line into words, spaces, equals signs, and whatever else
{
    vector<string> output;
    vector<string> splits = { " ", "=", "'", "\"", ";", "(", ")", "+", "-", "*", "/", "<", ">", "<=", ">=", ",", "[", "]", "{", "}" };
    vector<string> removables = {" ", string(1, char(8))};
    
    //split and seperate chunks (i.e tokenize them)
    output = seperateAll(line, splits);
    
    //recombine necessary parts
    recombine(output, "=", "=");
    recombine(output, "<", "=");
    recombine(output, ">", "=");
    recombine(output, "+", "=");
    recombine(output, "-", "=");
    recombine(output, "*", "=");
    recombine(output, "/", "=");
    recombine(output, "^", "=");
    //recombine(output, "\"", "\"", true);
    recombineBetween(output, "'", true);
    recombineBetween(output, "\"", true);
    
    removeVectorParts(output, removables);
    
    /*for (int i=0; i<output.size(); i++)
    {
         cout << output[i] << "\n";
    }*/
    
    return output;
}
/*
bool compare(string left, string right, string comparator, string type)
{
    if (comparator=="==")
    {
        if (type=="bool")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getBoolValue() == o2.getBoolValue();
        }
        else if (type=="int")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getIntValue() == o2.getIntValue();
        }
        else if (type=="double")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getDoubleValue() == o2.getDoubleValue();
        }
        else if (type=="string")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getStringValue() == o2.getStringValue();
        }
        else
        {
            return false;
        }
    }
    else if (comparator=="<")
    {
        //no bool
        if (type=="int")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getIntValue() < o2.getIntValue();
        }
        else if (type=="double")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getDoubleValue() < o2.getDoubleValue();
        }
        else if (type=="string")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getStringValue() < o2.getStringValue();
        }
        else
        {
            return false;
        }
    }
    else if (comparator==">")
    {
        //no bool
        if (type=="int")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getIntValue() > o2.getIntValue();
        }
        else if (type=="double")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getDoubleValue() > o2.getDoubleValue();
        }
        else if (type=="string")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getStringValue() > o2.getStringValue();
        }
        else
        {
            return false;
        }
    }
    else if (comparator=="<=")
    {
        //cout << left << " " << right << "\n";
        //no bool
        if (type=="int")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getIntValue() <= o2.getIntValue();
        }
        else if (type=="double")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getDoubleValue() <= o2.getDoubleValue();
        }
        else if (type=="string")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getStringValue() <= o2.getStringValue();
        }
        else
        {
            return false;
        }
    }
    else if (comparator==">=")
    {
        //no bool
        if (type=="int")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getIntValue() >= o2.getIntValue();
        }
        else if (type=="double")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getDoubleValue() >= o2.getDoubleValue();
        }
        else if (type=="string")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getStringValue() >= o2.getStringValue();
        }
        else
        {
            return false;
        }
    }
    else if (comparator=="!=")
    {
        if (type=="bool")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getBoolValue() != o2.getBoolValue();
        }
        else if (type=="int")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getIntValue() != o2.getIntValue();
        }
        else if (type=="double")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getDoubleValue() != o2.getDoubleValue();
        }
        else if (type=="string")
        {
            Object o1;
            o1.value = left;
            Object o2;
            o2.value = right;
            
            return o1.getStringValue() != o2.getStringValue();
        }
        else
        {
            return false;
        }
    }
    else //unknown comparator?
    {
        return false;
    }
    
    return false;
}*/

int getClosingBraceLine(vector<string> &code, int curLine, int startPos)
{
    if (curLine>=code.size()) return -1;
    int numOpen = 1;
    int numClosed = 0;
    for (int i=curLine; i<code.size(); i++)
    {
        string temp = code[i];
        //cout << temp << " " << numClosed << " " << numOpen << "--\n";
        if (i==0) temp = temp.substr(startPos);
        
        for (int j=0; j<temp.length(); j++)
        {
            if (temp[j]=='{')
            {
                numOpen++;
            }
            else if (temp[j]=='}')
            {
                numClosed++;
                //cout << numClosed << " " << numOpen << "\n";
                if (numClosed==numOpen)
                {
                    return i;
                }
            }
        }
    }
    
    return -1;
}

string toLowerCase(string s)
{
    for (int i=0; i<s.length(); i++)
    {
        if (int(s[i]) >= 65 && int(s[i]) <= 90)
        {
            s[i] = char(int(s[i])+32);
        }
    }
    return s;
}

//check: expressions inside arrays
bool isAssignment(vector<string> line)
{
    if (line[1] == "[")
    {
        if (line[2] == "]")
        {
            return true;
        }
        if (line[3] == "]")
        {
            return true;
        }
        return false;
    }
    return line[1] == "=" || line[1] == "+=" || line[1] == "*=" || line[1] == "/=" || line[1] == "-=" || line[1] == "^=";
    //vector<string>::iterator a = line.begin();
    //vector<string>::iterator b = line.end();
    
    //return (find(a, b, "=") != b) || (find(a, b, "+=") != b) || (find(a, b, "-=") != b) || (find(a, b, "*=") != b) || (find(a, b, "/=") != b) || (find(a, b, "^=") != b);
}

bool clean(vector<Object> &o)
{
    if (o[0].name == "for_var_garbage_666f725f7661725f67617262616765_1234567890987654321a")
    {
        o.erase(o.begin());
        return true;
    }
    
    return false;
}

bool un_nest(class SaveState &ss)
{
    if (ss.nestDepth <= 0) return false;
    //cout << ss.definedVariables.size() << " " << ss.nestDepth << "\n";
    while (ss.definedVariables.size()>ss.nestDepth)
    {
        ss.definedVariables.erase(ss.definedVariables.end()-1);
    }
    
    ss.nestDepth--;
    
    return true;
}

bool nest(SaveState &ss)
{
    ss.nestDepth++;
    while (ss.definedVariables.size()<ss.nestDepth+1)
    {
        ss.definedVariables.push_back(vector<Object>());
    }
    return true;
}
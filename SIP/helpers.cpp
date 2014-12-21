//
//  helpers.cpp
//  SIP
//
//  Created by Jacob Fakult on 10/8/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "helpers.h"
#include <cmath>

int countOfChars(string str, string match);

using namespace std;

string parseString(string str)
{
    return "hi";
}

int boolEval(string expr) //we will assume that parameters will be passed in with the outmost parentheses
{/*
    expr=expr.substr(1, expr.length()-1);
    vector<string> parts=split(expr, "==");
    
    //if (isBool(val))
        //return val;//atof(val.c_str());
    
    //if (val[0]=='(')
    for (int i=0; i<val.length(); i++)
    {
        if (val[i]=='(')
        {
            //cout << getFirstParentheses(val.substr(i)) << "\n";
            string subParenth = getFirstParentheses(val.substr(i));
            string append = "";
            if (val.length()>i+subParenth.length())
            {
                append = val.substr(i+subParenth.length()+2);
            }
            //cout << val << "\n";
            val = val.substr(0, i) + eval(subParenth) + append;
        }
    }
    
    */return -1;
}

bool isValidBool(string expr)
{
    if (countOfChars(expr, "(") != countOfChars(expr, ")"))
    {
        return false;
    }
    
    return true;
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

string eval(string val)
{
    if (isNum(val))
        return val;//atof(val.c_str());
    
    //if (val[0]=='(')
    for (int i=0; i<val.length(); i++)
    {
        if (val[i]=='(')
        {
            //cout << getFirstParentheses(val.substr(i)) << "\n";
            string subParenth = getFirstParentheses(val.substr(i));
            string append = "";
            if (val.length()>i+subParenth.length())
            {
                append = val.substr(i+subParenth.length()+2);
            }
            //cout << val << "\n";
            val = val.substr(0, i) + eval(subParenth) + append;
        }
    }
    
    //cout << val << "\n";
    while (val.find("^") != string::npos)
    {
        int pos=val.find("^");
        string before=findBefore(val, pos-1);
        string after=findAfter(val, pos+1);
        double temp = pow(atof(before.c_str()), atof(after.c_str()));
        string pow=to_string(temp);
        val=val.substr(0,pos-int(before.length()))+pow+val.substr(pos+int(after.length())+1);
    }
    //cout << val << "\n";
    while (val.find("*") != string::npos)
    {
        int pos=val.find("*");
        string before=findBefore(val, pos-1);
        string after=findAfter(val, pos+1);
        string mult=to_string(int(round(atof(before.c_str()) * atof(after.c_str()))));
        val=val.substr(0,pos-int(before.length()))+mult+val.substr(pos+int(after.length())+1);
    }
    //cout << val << "\n";
    while (val.find("/") != string::npos)
    {
        int pos=val.find("/");
        string before=findBefore(val, pos-1);
        string after=findAfter(val, pos+1);
        string div=to_string(atof(before.c_str()) / atof(after.c_str()));
        val=val.substr(0,pos-int(before.length()))+div+val.substr(pos+int(after.length())+1);
    }
    //cout << val << "\n";
    while (val.find("+") != string::npos)
    {
        int pos=val.find("+");
        string before=findBefore(val, pos-1);
        string after=findAfter(val, pos+1);
        string add=to_string(int(round(atof(before.c_str()) + atof(after.c_str()))));
        val=val.substr(0,pos-int(before.length()))+add+val.substr(pos+int(after.length())+1);
    }
    //cout << val << "\n";
    while (val.find("-") != string::npos)
    {
        int pos=val.find("-");
        string before=findBefore(val, pos-1);
        string after=findAfter(val, pos+1);
        string sub=to_string(atof(before.c_str()) - atof(after.c_str()));
        val=val.substr(0,pos-int(before.length()))+sub+val.substr(pos+int(after.length())+1);
    }
    
    return val;//atof(val.c_str());  //note, for int return type: return int(round(atof(val.c_str())));
}

bool isNum(string num)
{
    string temp="";
    try
    {
        temp=to_string(stoi(num));   //use c++ to confert string to int
        
        if (temp.length()<1)
            temp="0";
        
        if (num.find(".") == string::npos)          //if it is an int
            return (temp.length() == num.length());
        else
        {
            string decimal=num.substr(num.find(".")+1);
            return (temp.length()+decimal.length()+1 == num.length());
        }
    }
    catch (exception e) { return false; }
}

string findBefore(string val, int pos)
{
    if (pos<0)
        return "0";
    
    string retVal="";
    for (int i=pos; i>=0; i--)
    {
        if ((int(val[i])>=48 && int(val[i])<=57) || val[i] == '.')
            retVal=val[i]+retVal;
        else
            break;
    }
    
    return retVal;
}

string findAfter(string val, int pos)
{
    if (pos>=val.length())
        return "0";
    
    string retVal="";
    for (int i=pos; i<val.length(); i++)
    {
        if ((int(val[i])>=48 && int(val[i])<=57) || val[i] == '.')
            retVal=retVal+val[i];
        else
            break;
    }
    
    return retVal;
}

vector<string> split(string str, string delim)
{
    vector<string> parts;
    
    for (int i=0; i<str.length(); i++)
    {
        int pos=str.find(delim, i);
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
    for (int i=delims.size()-1; i>=0; i--)
    {
        auto findPos=find(strings.begin(), strings.end(), delims[i]);
        while (findPos!=strings.end())
        {
            int pos = distance(strings.begin(), findPos);
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
    for (int i=0;str.length()-match.length(); i++)
    {
        if (str.substr(i, match.length())==match)
        {
            count++;
        }
    }
    
    return count;
}

void recombine(vector<string> &output, string str)
{
    if (output.size()<=1) //nothing to combine
    {
        return;
    }
    
    string lastStr = output[0];
    string thisStr = output[1];
    
    for (int i=1; i<output.size(); i++)
    {
        
    }
}

void recombineBetween(vector<string> &output, string str)
{
    
}


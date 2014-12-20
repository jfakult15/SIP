//
//  helpers.cpp
//  SIP
//
//  Created by Jacob Fakult on 10/8/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "helpers.h"

using namespace std;

string parseString(string str)
{
    return "hi";
}

int boolEval(string expr) //we will assume that parameters will be passed in with the outmost parentheses
{
    expr=expr.substr(1, expr.length()-1);
    vector<string> parts=split(expr, "==");
    
    
    return -1;
}

string eval(string val)
{
    if (isNum(val))
        return val;//atof(val.c_str());
    
    //double sum=0.0;
    while (!isNum(val))
    {
        while (val.find("*") != string::npos)
        {
            int pos=val.find("*");
            string before=findBefore(val, pos-1);
            string after=findAfter(val, pos+1);
            string mult=to_string(int(round(atof(before.c_str()) * atof(after.c_str()))));
            val=val.substr(0,pos-int(before.length()))+mult+val.substr(pos+int(after.length())+1);
            ///*
            cout << "from 0 for " << pos-int(before.length()) << "\n";
            cout << "from " << pos+int(after.length())+1 << " for " << val.length() << "\n";
            cout << "before: " << before << "\n";
            cout << "after: " << after << "\n";
            cout << "val: " << val << "\n";
            //*/
        }
        cout << val << "\n";
        while (val.find("/") != string::npos)
        {
            int pos=val.find("/");
            string before=findBefore(val, pos-1);
            string after=findAfter(val, pos+1);
            string div=to_string(atof(before.c_str()) / atof(after.c_str()));
            val=val.substr(0,pos-int(before.length()))+div+val.substr(pos+int(after.length())+1);
        }
        cout << val << "\n";
        while (val.find("+") != string::npos)
        {
            int pos=val.find("+");
            string before=findBefore(val, pos-1);
            string after=findAfter(val, pos+1);
            string add=to_string(int(round(atof(before.c_str()) + atof(after.c_str()))));
            val=val.substr(0,pos-int(before.length()))+add+val.substr(pos+int(after.length())+1);
        }
        cout << val << "\n";
        while (val.find("-") != string::npos)
        {
            int pos=val.find("-");
            string before=findBefore(val, pos-1);
            string after=findAfter(val, pos+1);
            string sub=to_string(atof(before.c_str()) - atof(after.c_str()));
            val=val.substr(0,pos-int(before.length()))+sub+val.substr(pos+int(after.length())+1);
        }
        //cout << val << "\n";
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
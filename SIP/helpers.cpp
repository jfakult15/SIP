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
        if (pos!=string::npos)
        {
            parts.push_back(str.substr(0,pos));
            i--;
            str=str.substr(pos+delim.length());
        }
        else
        {
            parts.push_back(str.substr(pos+delim.length()-1));
            str="";
        }
    }
    
    return parts;
}


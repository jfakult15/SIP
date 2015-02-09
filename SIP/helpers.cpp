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

int quickEval(string expr, SaveState &ss) //should be just a single bool part (no parenths, &&'s , etc)
{
    //cout << expr << "=\n";
    if (expr=="true") return 1;
    if (expr=="false") return 0;
    
    return -1;
}

int boolEval(string expr, SaveState &ss) //we will assume that parameters will be passed in with the outmost parentheses
{
    //cout << "=" << expr << "=\n";
    
    //cout << countOfChars(expr, "(") << "-" << countOfChars(expr, ")") << "\n";
    
    if (countOfChars(expr, "(") != countOfChars(expr, ")"))
    {
        return -1;
    }
    
    for (int i=0; i<ss.definedVariables.size(); i++)
    {
        Object o = getObjectByName(ss.definedVariables[i], expr);
        //cout << o.name << "=" << expr << "=\n";
        if (o.name != "invalid object name")
        {
            //cout << o.value << " " << o.getB
            bool temp = o.getBoolValue();
            if (temp) expr="true";
            else expr="false";
        }
    }
    
    cout << "=" << expr << "=\n";
    
    if (expr=="true") return 1;
    if (expr=="false") return 0;
    
    bool result=true;
    
    for (int i=0; i<expr.length(); i++)
    {
        if (expr[i]=='(')
        {
            string subParenth = getFirstParentheses(expr.substr(i));
            string append = "";
            if (expr.length()>i+subParenth.length())
            {
                append = expr.substr(i+subParenth.length()+2);
            }
            
            int temp2 = quickEval(expr.substr(0, i), ss);
            int temp3 = quickEval(append, ss);
            //cout << "=" << subParenth << "1=\n";
            int temp1 = boolEval(subParenth, ss);
            if (temp1==-1 && subParenth.length()!=0) return -1;
            if (temp2==-1  && expr.substr(0,i).length()!=0) return -1;
            if (temp3==-1 && append.length()!=0) return -1;
            
            bool boolVal1 = temp1==1 || subParenth.length()==0;
            bool boolVal2 = temp2==1 || expr.substr(0,i).length()==0;
            bool boolVal3 = temp3==1 || append.length()==0;
            
            //cout << boolVal1 << "=" << boolVal2 << "=" << boolVal3 << "=\n";
            
            result = result && boolVal2 && boolVal1 && boolVal3;
            //cout << result << "==\n";
            if (!result) return false;
        }
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
        int pos=int(val.find("^"));
        string before=findBefore(val, pos-1);
        string after=findAfter(val, pos+1);
        double temp = pow(atof(before.c_str()), atof(after.c_str()));
        string pow=to_string(temp);
        val=val.substr(0,pos-int(before.length()))+pow+val.substr(pos+int(after.length())+1);
    }
    //cout << val << "\n";
    while (val.find("*") != string::npos)
    {
        int pos=int(val.find("*"));
        string before=findBefore(val, pos-1);
        string after=findAfter(val, pos+1);
        string mult=to_string(int(round(atof(before.c_str()) * atof(after.c_str()))));
        val=val.substr(0,pos-int(before.length()))+mult+val.substr(pos+int(after.length())+1);
    }
    //cout << val << "\n";
    while (val.find("/") != string::npos)
    {
        int pos=int(val.find("/"));
        string before=findBefore(val, pos-1);
        string after=findAfter(val, pos+1);
        string div=to_string(atof(before.c_str()) / atof(after.c_str()));
        val=val.substr(0,pos-int(before.length()))+div+val.substr(pos+int(after.length())+1);
    }
    //cout << val << "\n";
    while (val.find("+") != string::npos)
    {
        int pos=int(val.find("+"));
        string before=findBefore(val, pos-1);
        string after=findAfter(val, pos+1);
        string add=to_string(int(round(atof(before.c_str()) + atof(after.c_str()))));
        val=val.substr(0,pos-int(before.length()))+add+val.substr(pos+int(after.length())+1);
    }
    //cout << val << "\n";
    while (val.find("-") != string::npos)
    {
        int pos=int(val.find("-"));
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

void recombine(vector<string> &output, string str)
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
        if (lastStr == thisStr && thisStr == str)
        {
            output[i-1] += str;
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


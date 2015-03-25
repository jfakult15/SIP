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

string simplifyExpr(string expr, vector<string> parts)
{
    //cout << "Evaluating line: " << expr << "\n";
    string lastType="bool"; //lowest level data type according to the casting hierarchy defined in object.cpp
    
    string first=""; string last=""; string comparator="";
    bool gotLeftHandOperation=false;
    for (int i=0; i<parts.size(); i++)
    {
        if (parts[i]!="(" && parts[i]!=")")
        {
            if (parts[i]=="<" || parts[i]==">" || parts[i]=="==" || parts[i]==">=" || parts[i]=="<=" || parts[i]=="!=")
            {
                comparator = parts[i];
                gotLeftHandOperation = true;
                continue;
            }
            Object o;
            o.value = parts[i];
            o.type = o.getType();
            if (!gotLeftHandOperation)
            {
                first += parts[i];
            }
            else
            {
                last += parts[i];
            }
            if (o.type != "invalid type")
            {
                lastType = compareTypes(lastType, o.type);
            }
        }
    }
    //cout << first << " " << comparator << " " << last << " " << lastType << "\n";
    
    if (lastType=="int" || lastType=="double")
    {
        first = eval(first);
        last = eval(last);
    }
    bool temp=compare(first, last, comparator, lastType);
    //cout << "Result was: " << temp << "\n";
    if (temp==true) return "true";
    
    return "false";
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
                Object o = getAnyObjectNamed(ss.definedVariables, val);
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

errVar stringEval(vector<string> expr, SaveState &ss)
{
    errVar e;
    
    string build = "";
    bool currentIsConcat = false; //i.e '+' symbol
    for (int i=0; i<expr.size(); i++)
    {
        string val = expr[i];
        
        if (!currentIsConcat)
        {
            Object o;
            o.value = val;
            o.type = o.getType();
            
            if (o.type != "invalid type")
            {
                build += o.getStringValue();
            }
            else
            {
                //cout << "Got: " << val << "\n";
                e.errorPos = i;
                e.message = "Unable to parse data type for string";
                return e;
            }
        }
        else
        {
            if (val != "+")
            {
                e.message = "You can only add strings: i.e add using '+' operator";
                e.errorPos = i;
                return e;
            }
        }
        currentIsConcat = !currentIsConcat;
    }
    e.message = build;
    
    return e;
}

errVar anyEval(vector<string> expr, SaveState &ss, ExecutionOutput &output, vector<string> &code) //will evaluate expressions of any type using helper eval functions of course
{
    errVar e;
    
    string exprType = determineEvalType(expr, ss);;
    
    for (int i=0; i<expr.size(); i++)
    {
        if (isProperVarName(expr[i]))
        {
            if (expr[i+1] == "(") //function
            {
                vector<string> temp = expr;
                temp.insert(temp.begin(), "function");
                //cout << vectorToString(temp) << "--\n";
                if (temp[temp.size()-1] == ";")
                {
                    temp.erase(temp.end()-1);
                }
                e = syntaxFunction(temp);
                //cout << e.message << "==\n";
                if (e.errorPos != -1) //improper function invocation syntax
                {
                    return e;
                }

                string funcName = temp[1];
                int numParams = 0;
                for (int i=3; i<temp.size()-1; i++)
                {
                    if (temp[i] != ",") numParams++;
                }
                
                if (!funcExists(funcName, numParams, ss))
                {
                    bool foundFunc = false;
                    for (int i=0; i<code.size(); i++)
                    {
                        if (code[i].find("function " + funcName) != string::npos) //we found the function! Add it to our known functions
                        {
                            int startLine = i;
                            //cout << code[curLine] << "===\n";
                            int endLine = getClosingBraceLine(code, i+2, 0);
                            createFunction(tokenize(code[i]), startLine, endLine, ss);
                            i=int(code.size());
                            foundFunc = true;
                        }
                    }
                    if (!foundFunc)
                    {
                        e.errorPos = 0;
                        e.message = "Unknown function. Function was never declared";
                        return e;
                    }
                }
                
                FunctionObject f = getFunctionNamed(funcName, ss);
                if (f.name == "invalid function name") //this shouldn;t ever happen of course
                {
                    e.errorPos = 0;
                    e.message = "Fatal error. Quitting...";
                    return e;
                }
                
                vector<string> block(code.begin()+f.startLine+2, code.begin()+f.endLine);
                ss.nestDepth++;
                //cout << vectorToString(block);
                //bug: add variables to pass in here, parallel vector<Object>
                execute(block, output);
                ss.nestDepth--;
                e.message = output.returnVal;
            }
            else
            {
                Object o = getAnyObjectNamed(ss.definedVariables, expr[i]);
                if (o.name != "invalid object name")
                {
                    expr[i] = o.value;
                    //cout << expr[i] << "--\n";
                    exprType = o.type;
                }
                else
                {
                    e.errorPos = i;
                    e.message = "Undefined variable";
                    return e;
                }
            }
        }
    }
    
    //cout << exprType << "==\n";
    if (exprType == "bool")
    {
        errVar temp = boolEval(expr, ss);
        e.message = temp.message;
    }
    else if (exprType == "int" || exprType == "double")
    {
        for (int i=0; i<expr.size(); i++)
        {
            if (expr[i]=="true") expr[i]="1";
            if (expr[i]=="false") expr[i]="0";
        }
        string temp = eval(vectorToString(expr));
        e.message = temp;
    }
    else if (exprType == "string")
    {
        for (int i=0; i<expr.size(); i++)
        {
            if (expr[i]=="true") expr[i]="1";
            if (expr[i]=="false") expr[i]="0";
        }
        errVar temp = stringEval(expr, ss);
        e.message = temp.message;
    }
    //cout << e.message << "-\n";
    
    return e;
}

errVar boolEval(vector<string> parts, SaveState &ss) //we will assume that parameters will be passed in with the outermost parentheses
{
    errVar e;
    
    int openPar = 0;
    int closePar = 0;
    for (int i=0; i<parts.size(); i++)
    {
        openPar += countOfChars(parts[i], "(");
        closePar += countOfChars(parts[i], ")");
        Object o = getAnyObjectNamed(ss.definedVariables, parts[i]);
        //cout << parts[i] << "==\n";
        if (o.name != "invalid object name")
        {
            parts[i] = o.value;
        }
        else
        {
            Object o;
            o.value = parts[i];
            o.type = o.getType();
            if (o.type=="invalid type" && parts[i]!="(" && parts[i]!=")" && parts[i]!="&&" && parts[i]!="||" && !isComparator(parts[i]) &&!isMathFunc(parts[i]))
            {
                e.errorPos = i;
                e.message = "Undefined variable in boolean expression";
                return e;
            }
        }
    }
    
    if (openPar != closePar)
    {
        e.errorPos = 0;
        e.message = "Different number of closing and opening parentheses in boolean expression";
    }
    
    vector<vector<string> > chunks;// = vector<vector<string> >();
    chunks.push_back(vector<string>());
    
    int chunkNum = 0;
    
    for (int i=0; i<parts.size(); i++)
    {
        string s = parts[i];
        
        if (s!="&&" && s!="||")
        {
            if (i>0 && (parts[i-1]=="+" && s[0]=='('))
            {
                chunks[chunkNum].push_back(s);
                continue;
            }
            
            if (s=="(" && find(chunks[chunkNum].begin(), chunks[chunkNum].end(), "(")!=chunks[chunkNum].end())
            {
                chunkNum++;
                chunks.push_back(vector<string>());
            }
            chunks[chunkNum].push_back(s);
            if (s==")" && i>0 && !isComparator(parts[i+1]))
            {
                chunkNum++;
                chunks.push_back(vector<string>());
            }
        }
        else
        {
            chunkNum++;
            chunks.push_back(vector<string>());
            chunks[chunkNum].push_back(s);
            
            chunkNum++;
            chunks.push_back(vector<string>());
        }
    }
    
    /*for (int i=0; i<chunks.size(); i++)
    {
        for (int j=0; j<chunks[i].size(); j++)
        {
            cout << chunks[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";*/
    
    bool seenFirstResult=false;
    
    bool result=false;
    bool lastResult=false;
    string comparator="";
    
    for (int i=0; i<chunks.size(); i++)
    {
        if (chunks[i].size()==0)
        {
            chunks.erase(chunks.begin()+i);
            i--;
            continue;
        }
        
        string temp = vectorToString(chunks[i]);
        
        
        if (temp!="&&" && temp!="||" && temp!="(" && temp!=")")//temp[0]=='(' && temp[temp.length()-1]==')') //chunk starts and ends with parentheses
        {
            chunks[i] = tokenize(simplifyExpr(temp, chunks[i]));
        }
        for (int j=0; j<chunks[i].size(); j++)
        {
            string temp = chunks[i][j];
            if (temp=="true")
            {
                lastResult = true;
            }
            else if (temp=="false")
            {
                lastResult = false;
            }
            
            if (comparator=="&&")
            {
                if (!seenFirstResult)
                {
                    seenFirstResult=true;
                    result=true;
                }
                result = result && lastResult;
                comparator = "";
            }
            else if (comparator=="||")
            {
                if (!seenFirstResult)
                {
                    seenFirstResult=true;
                    result=false;
                }
                result = result || lastResult;
                comparator = "";
            }
            else
            {
                if (!seenFirstResult)
                {
                    result = true;
                    seenFirstResult=true;
                }
                result = result && lastResult;
            }
            
            if (temp=="&&" || temp=="||")
            {
                comparator = temp;
            }
            
            
            //e.message += chunks[i][j] + " ";
            //cout << chunks[i][j] << " ";
        }
        //e.message += "\n";
        //cout << "\n";
    }
    
    //cout << result << "==\n";
    if (result) e.message = "true";
    else e.message = "false";
    
    return e;
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
    vector<string> splits = { " ", "=", "'", "\"", ";", "(", ")", "+", "-", "*", "/", "<", ">", "<=", ">=", "," };
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
}

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

bool isAssignment(vector<string> line)
{
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
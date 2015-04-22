//
//  evals.cpp
//  SIP
//
//  Created by Jacob Fakult on 4/22/15.
//  Copyright (c) 2015 jfakult15. All rights reserved.
//

#include "evals.h"

string eval(string val)
{
    //cout << val << " " << isNum(val) << "--\n";
    if (isNum(val))
        return val;//atof(val.c_str());
    //cout << "hi\n";
    
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
    
    //cout << val << "--1\n";
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
    //cout << val << "--\n";
    while (val.find("+") != string::npos)
    {
        //cout << "Val: " << val << "\n";
        int pos=int(val.find("+"));
        string before=findBefore(val, pos-1);
        string after=findAfter(val, pos+1);
        
        Object b; b.value = before;
        Object a; a.value = after;
        bool isString = (a.getType() == "string" || b.getType() == "string");
        //isString = false;
        
        if (isString)
        {
            string add = b.getStringValue() + a.getStringValue();
            val=val.substr(0,pos-int(before.length()))+add+val.substr(pos+int(after.length())+1);
        }
        else
        {
            //cout << 1<<"--\n";
            string add=to_string(atof(before.c_str()) + atof(after.c_str()));
            val=val.substr(0,pos-int(before.length()))+add+val.substr(pos+int(after.length())+1);
        }
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
    
    //cout << val << "==2\n";
    
    return val;//atof(val.c_str());  //note, for int return type: return int(round(atof(val.c_str())));
}

bool isNum(string num)
{
    if (num[num.length()-1] == '.') return false;
    int numPeriods = 0;
    for (int i=0; i<num.length(); i++)
    {
        if (num[i]=='.')
        {
            numPeriods++;
            if (numPeriods > 1) return false;
        }
        if (!isNumber(num[i])) return false;
    }
    return true;
}

string findBefore(string val, int pos)
{
    if (pos<0)
        return "0";
    
    string retVal="";
    for (int i=pos; i>=0; i--)
    {
        //if ((int(val[i])>=48 && int(val[i])<=57) || val[i] == '.')
        retVal=val[i]+retVal;
        //else
        //    break;
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
    
    if (expr.size() == 0)
    {
        e.message = "0";
        return e;
    }
    
    vector<string> chunks;
    //simplify all parenthese expressions
    for (int i=0; i<expr.size(); i++)
    {
        string s = expr[i];
        
        if (s=="true") s="1";
        if (s=="false") s="0";
        if (isProperVarName(s))
        {
            Object o = getAnyObjectNamed(ss.definedVariables, s, ss.nestDepth);
            if (o.name != "invalid object name")
            {
                s = o.value;
            }
        }
        
        if (s == "(")
        {
            int openPar = i;
            int parsOpen = 1;
            i++;
            while (parsOpen>0 && i<expr.size())
            {
                if (expr[i] == ")")
                {
                    parsOpen--;
                }
                else if (expr[i]=="(")
                {
                    parsOpen++;
                }
                i++;
            }
            i--;
            if (parsOpen != 0) //missing a closing parenthesis, syntax error
            {
                e.errorPos = openPar;
                e.message = "Expecting a closing parenthesis";
                return e;
            }
            errVar temp = anyEval(vector<string>(expr.begin()+openPar+1, expr.begin()+i), ss, output, code);
            if (temp.errorPos>=0)
            {
                return temp;
            }
            chunks.push_back(temp.message);
        }
        else
        {
            chunks.push_back(s);
        }
    }
    
    //Should only reach here recursively. Check for inequalities (i.e a boolean expression)
    set<string> compSet = {"==", "!=", "<", ">", "<=", ">="};
    bool isBoolEval = false;
    for (int i=0; i<chunks.size(); i++)
    {
        if (compSet.find(chunks[i]) != compSet.end())
        {
            isBoolEval = true;
            i = int(chunks.size());
        }
    }
    
    if (isBoolEval)
    {
        errVar temp = boolEval(chunks, ss);
        //cout << isBoolEval << " " << (isBoolEval == true) << "\n";
        if (temp.errorPos >= 0) return temp;
        if (temp.message == "true")
        {
            temp.message = "1";
        }
        else if (temp.message == "false")
        {
            temp.message = "0";
        }
        return temp;
    }
    
    string value = eval(vectorToString(chunks));
    Object o; o.value = value; o.type = o.getType();
    e.message = o.getValue();
    
    return e;
}

//Outdated, about a .6s difference in time over 100,000 loops
errVar anyEval2(vector<string> expr, SaveState &ss, ExecutionOutput &output, vector<string> &code) //will evaluate expressions of any type using helper eval functions of course
{
    errVar e;
    
    //This section splits and reduces the complex eval expression into its individual operand parts (i.e square(6)+1 -> 36 + 1)
    vector<string> splits = {"+","-","*","/","&&","||"};
    vector<string> parts;
    parts = vector<string>();
    
    int startPos = 0;
    for (int i=0; i<expr.size(); i++)
    {
        if (find(splits.begin(), splits.end(), expr[i]) != splits.end()) //we have a plus or minus operand etc
        {
            //cout << vectorToString(parts) << "==\n";
            if (i==0)
            {
                e.message = "Unable to evaluate statement: Unexpected first character: '" + expr[i] + "'";
                e.errorPos = i;
                return e;
            }
            e = anyEval(parts, ss, output, code);
            if (e.errorPos >= 0)
            {
                e.message = "Couldn't evaluate part: " + parts[0] + "\nGot error: " + e.message;
                return e;
            }
            expr[startPos] = e.message;
            for (int i=0; i<parts.size()-1; i++)
            {
                expr.erase(expr.begin()+startPos+1);
            }
            parts = vector<string>();
            startPos = i+1;
        }
        else
        {
            parts.push_back(expr[i]);
        }
    }
    
    string exprType = determineEvalType(expr, ss);
    //check
    //cout << exprType << "-" << vectorToString(expr) << "==\n";
    for (int i=0; i<expr.size(); i++)
    {
        if (isProperVarName(expr[i]))
        {
            //cout << expr[i+1] << " " << expr[i] << "==\n";
            if (i<expr.size()-1 && expr[i+1] == "(") //function
            {
                int n = -1;
                e = executeFunction(expr, code, output, n, ss);
                
                if (e.errorPos >= 0)
                {
                    return e;
                }
                
                i = int(expr.size());
            }
            else
            {
                Object o = getAnyObjectNamed(ss.definedVariables, expr[i], ss.nestDepth);
                //cout << o.value << "==\n";
                if (o.name != "invalid object name")
                {
                    if (o.isArray == 1)
                    {
                        if (i>expr.size()-4)
                        {
                            e.errorPos = i;
                            e.message = "Can't evaluate array: no index specified";
                            return e;
                        }
                        if (expr[i+1] != "[")
                        {
                            e.errorPos = i+1;
                            e.message = "Can't evaluate array: no index specified";
                            return e;
                        }
                        if (expr[i+3] != "]")
                        {
                            e.errorPos = i+1;
                            e.message = "Array expects closing bracket";
                            return e;
                        }
                        string key = expr[i+2];
                        Object temp2;
                        temp2.value = key;
                        int l = key.length()-1;
                        if (((key[0] != '"' || key[l] != '"') || (key[0] != '\'' || key[l] != '\'')) && (temp2.getType() == "string"))
                        {
                            Object tempObj = getAnyObjectNamed(ss.definedVariables, key, ss.nestDepth);
                            if (tempObj.name != "invalid object name")
                            {
                                key = tempObj.getValue();
                            }
                            else
                            {
                                e.errorPos = i+2;
                                e.message = "Array does not contain this index";
                                return e;
                            }
                        }
                        
                        errVar temp = o.getArrayValue(key);
                        if (temp.errorPos >= 0)
                        {
                            e.errorPos = i+2;
                            e.message = "Array does not contain this index";
                            return e;
                        }
                        expr.erase(expr.begin()+i);
                        expr.erase(expr.begin()+i);
                        expr.erase(expr.begin()+i);
                        expr[i] = temp.message;
                        temp2.value = temp.message;
                        exprType = temp2.getType();
                    }
                    else
                    {
                        expr[i] = o.getValue();
                        //cout << expr[i] << "--\n";
                        exprType = o.type;
                    }
                }
                else
                {
                    e.errorPos = i;
                    e.message = "Undefined variable";
                    return e;
                }
            }
        }
        else
        {
            //cout << expr[i] <<"\n";
        }
    }
    //cout << vectorToString(expr) << "==\n";
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
        //cout << vectorToString(expr) << "hi\n";
        string temp = eval(vectorToString(expr));
        //cout << temp << "==\n";
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
    Object o;
    o.value = e.message; //parse all data correctly. I.e we don't want ints displaying as doubles
    o.type = o.getType();
    e.message = o.getValue();
    
    return e;
}

errVar compare(string left, string right, string comp)
{
    errVar e;
    
    Object l; l.value = eval(left);
    Object r; r.value = eval(right);
    
    bool isString = (l.getType() == "string" || r.getType() == "string");
    
    if (comp=="==")
    {
        if (isString)
        {
            e.message = to_string(l.getStringValue() == r.getStringValue());
        }
        else
        {
            //cout << (l.getDoubleValue() == r.getDoubleValue()) << "\n";
            e.message = to_string(l.getDoubleValue() == r.getDoubleValue());
        }
        return e;
    }
    else if (comp=="!=")
    {
        if (isString)
        {
            e.message = to_string(l.getStringValue() != r.getStringValue());
        }
        else
        {
            e.message = to_string(l.getDoubleValue() == r.getDoubleValue());
        }
        return e;
    }
    else if (comp=="<=")
    {
        if (isString)
        {
            e.message = to_string(l.getStringValue() <= r.getStringValue());
        }
        else
        {
            e.message = to_string(l.getDoubleValue() <= r.getDoubleValue());
        }
        return e;
    }
    else if (comp==">=")
    {
        if (isString)
        {
            e.message = to_string(l.getStringValue() >= r.getStringValue());
        }
        else
        {
            e.message = to_string(l.getDoubleValue() >= r.getDoubleValue());
        }
        return e;
    }
    else if (comp=="<")
    {
        if (isString)
        {
            e.message = to_string(l.getStringValue() < r.getStringValue());
        }
        else
        {
            e.message = to_string(l.getDoubleValue() < r.getDoubleValue());
        }
        return e;
    }
    else if (comp==">")
    {
        if (isString)
        {
            e.message = to_string(l.getStringValue() > r.getStringValue());
        }
        else
        {
            e.message = to_string(l.getDoubleValue() > r.getDoubleValue());
        }
        return e;
    }
    else
    {
        e.errorPos = 0;
        e.message = "Unknown comparator: " + comp;
    }
    return e;
}

errVar compareEval(vector<string> boolChunk, SaveState &ss)
{
    //simplify all comparator operations
    string left = "";
    string right = "";
    string comp = "";
    bool onLeft = true;
    set<string> compVec = {"==", "!=", "<", ">", ">=", "<="};
    for (int i=0; i<boolChunk.size(); i++)
    {
        string s = boolChunk[i];
        if (s=="true") s="1";
        if (s=="false") s="0";
        if (isProperVarName(s))
        {
            Object o = getAnyObjectNamed(ss.definedVariables, s, ss.nestDepth);
            if (o.name != "invalid object name")
            {
                s = o.value;
            }
        }
        if (compVec.find(s) != compVec.end())
        {
            //handle comparator
            comp = s;
            onLeft = false;
        }
        else
        {
            if (onLeft)
            {
                left += s;
            }
            else
            {
                right += s;
            }
        }
    }
    
    if (right.length() == 0)
    {
        errVar e;
        if (left=="0" || left=="false") e.message = "0";
        else e.message = "1";
        return e;
    }
    
    return compare(left, right, comp);
}

errVar boolEval(vector<string> parts, SaveState &ss)
{
    errVar e;
    
    if (parts.size() == 0)
    {
        e.message = "0";
        return e;
    }
    
    vector<string> chunks;
    //simplify all parenthese expressions
    for (int i=0; i<parts.size(); i++)
    {
        string s = parts[i];
        if (s == "(")
        {
            int openPar = i;
            int parsOpen = 1;
            i++;
            while (parsOpen>0 && i<parts.size())
            {
                if (parts[i] != ")")
                {
                    parsOpen--;
                }
                else if (parts[i]=="(")
                {
                    parsOpen++;
                }
                i++;
            }
            i--;
            if (parsOpen != 0) //missing a closing parenthesis, syntax error
            {
                e.errorPos = openPar;
                e.message = "Expecting a closing parenthesis";
                return e;
            }
            errVar temp = boolEval(vector<string>(parts.begin()+openPar+1, parts.begin()+i), ss);
            if (temp.errorPos>=0)
            {
                return temp;
            }
            chunks.push_back(temp.message);
        }
        else
        {
            chunks.push_back(s);
        }
    }
    
    //simplify all && and || expressions
    vector<string> boolChunk;
    set<string> opVec = {"&&", "and", "||", "or"};
    vector<string> simpleChunks;
    for (int i=0; i<chunks.size(); i++)
    {
        string s = chunks[i];
        if (opVec.find(s) != opVec.end())
        {
            errVar temp = compareEval(boolChunk, ss);
            if (temp.errorPos>=0)
            {
                return temp;
            }
            simpleChunks.push_back(temp.message);
            simpleChunks.push_back(s);
            boolChunk.clear();
        }
        else
        {
            boolChunk.push_back(s);
        }
    }
    if (boolChunk.size() == 0)
    {
        e.errorPos = 0; //bug: fix this
        e.message = "Can't put and/or operator at end of bool statement";
        return e;
    }
    errVar temp = compareEval(boolChunk, ss);
    if (temp.errorPos>=0)
    {
        return temp;
    }
    simpleChunks.push_back(temp.message);
    //cout << vectorToString(simpleChunks) << "--\n";
    
    //Now we are left with something in the format of '1 && 0 || 1' etc.
    bool result = simpleChunks[0] == "1";
    for (int i=1; i<simpleChunks.size(); i+=2)
    {
        string s = simpleChunks[i];
        if (opVec.find(s) == opVec.end())
        {
            e.errorPos = i;
            e.message = "Expected and/or here";
            return e;
        }
        if (s=="&&" || s=="and")
        {
            result = result && (simpleChunks[i+1] == "1");
        }
        else if (s=="||" || s=="or")
        {
            result = result || (simpleChunks[i+1] == "1");
        }
        else
        {
            e.errorPos = i;
            e.message = "Unknown operator! " + s;
            return e;
        }
    }
    if (result) e.message = "true";
    else e.message = "false";
    //e.message = to_string(result);
    
    return e;
}
/*
 //Outdated, buggy boolEval! Over 1.5 seconds slower per 100,000 loops
 errVar boolEval2(vector<string> parts, SaveState &ss) //we will assume that parameters will be passed in with the outermost parentheses
 {
 errVar e;
 
 parts.insert(parts.begin(), "(");
 parts.insert(parts.end(), ")");
 
 if (parts.size()==0) return e;
 
 int openPar = 0;
 int closePar = 0;
 for (int i=0; i<parts.size(); i++)
 {
 openPar += countOfChars(parts[i], "(");
 closePar += countOfChars(parts[i], ")");
 Object o = getAnyObjectNamed(ss.definedVariables, parts[i], ss.nestDepth);
 
 if (o.name != "invalid object name")
 {
 parts[i] = o.value;
 //cout << o.value << "--\n";
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
 
 for (int i=0; i<parts.size()-1; i++)
 {
 if (parts[i] == "and") parts[i] = "&&";
 if (parts[i] == "or") parts[i] = "||";
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
 }*/


/*
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
 }*/
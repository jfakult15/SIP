//
//  keyword_object.cpp
//  SIP
//
//  Created by Jacob Fakult on 12/24/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "keyword_object.h"

using namespace std;

KeywordObject::KeywordObject()
{
    isSingleLineStatement=false;
    isBlockStatement=false;
    isSingleWordStatement=false;
}

KeywordObject::KeywordObject(bool line, bool block, bool word)
{
    isSingleLineStatement = line;
    isBlockStatement = block;
    isSingleWordStatement = word;
}

bool KeywordObject::isInitialized()
{
    return (isSingleLineStatement || isBlockStatement || isSingleWordStatement);
}

map<string, KeywordObject> initKeywordObjects()
{
    map<string, KeywordObject> temp;
    temp["var"] = KeywordObject(true, false, false);
    temp["if"] = KeywordObject(false, true, false);
    temp["while"] = KeywordObject(false, true, false);
    temp["else"] = KeywordObject(false, true, false);
    temp["for"] = KeywordObject(false, true, false);
    temp["print"] = KeywordObject(true, false, false);
    temp["function"] = KeywordObject(false, true, false);
    temp["input"] = KeywordObject(true, false, false);
    temp["continue"] = KeywordObject(false, false, true);
    temp["break"] = KeywordObject(false, false, true);
    temp["input"] = KeywordObject(true, false, false);
    
    return temp;
}
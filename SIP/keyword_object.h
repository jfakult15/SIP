//
//  keyword_object.h
//  SIP
//
//  Created by Jacob Fakult on 12/24/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#ifndef __SIP__keyword_object__
#define __SIP__keyword_object__

#include <stdio.h>
#include <map>
#include <string>

class KeywordObject
{
public:
    KeywordObject();
    KeywordObject(bool line, bool block, bool word);
    bool isSingleLineStatement;
    bool isBlockStatement;
    bool isSingleWordStatement;
    
    bool isInitialized();
};

std::map<std::string, KeywordObject> initKeywordObjects();

#endif /* defined(__SIP__keyword_object__) */

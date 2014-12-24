//
//  function_object.cpp
//  SIP
//
//  Created by Jacob Fakult on 12/24/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#include "function_object.h"

FunctionObject::FunctionObject(Object newReturn, string newName, int newStartLine, int newEndLine)
{
    returnValue = newReturn;
    name=newName;
    startLine = newStartLine;
    endLine = newEndLine;
}
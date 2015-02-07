//
//  output.h
//  SIP
//
//  Created by Jacob Fakult on 12/22/14.
//  Copyright (c) 2014 jfakult15. All rights reserved.
//

#ifndef SIP_output_h
#define SIP_output_h

#include <vector>

using namespace std;

struct ExecutionOutput
{
    vector<string> output;
    vector<string> warnings;
    vector<string> err;
    vector<string> info;
};

#endif

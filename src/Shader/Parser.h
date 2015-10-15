//
//  Parser.h
//  BoxWorld
//
//  Created by Han Maokun on 10/16/15.
//
//

#ifndef BoxWorld_Parser_h
#define BoxWorld_Parser_h

#include <stdio.h>
#include <string>
#include "ShaderConfig.h"

using namespace std;

class Parser {
public:
    Parser(string json_data);
    virtual ~Parser() {}
    
    typedef enum {
        NONE = 0,
        NO_JASON,
        NO_BOXWORLD,
        NO_MAINPROGRAM,
        NO_CODE,
        NO_VERT_CODE,
        NO_FRAG_CODE
    }PARSER_ERROR;
    
    PARSER_ERROR getErrorType() { return mError; }
    S_MainProgram getMainProgram() { return mMainProgram; }
    
private:
    PARSER_ERROR  mError;
    S_MainProgram mMainProgram;
};

#endif

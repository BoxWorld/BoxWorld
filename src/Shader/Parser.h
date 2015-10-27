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
#include "json.h"

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
    S_Program getMainProgram() { return mMainProgram; }
    S_Program getAudioProgram() { return mAudioProgram; }
    
private:
    S_Program decodeToProgram(const Json::Value& val);
    
    PARSER_ERROR  mError;
    S_Program mMainProgram, mAudioProgram;
};

#endif

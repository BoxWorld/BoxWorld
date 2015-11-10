//
//  ConnMsg.h
//  BoxWorld
//
//  Created by Han Maokun on 10/12/15.
//
//

#ifndef BoxWorld_Message_h
#define BoxWorld_Message_h

#include <stdio.h>
#include <string>
#include "json.h"

using namespace std;

#define BOXWORLD_MSG_TYPE_KEY "Type"
#define BOXWORLD_MSG_CONTENT_KEY "Content"

typedef void (*msgCallbackFn)(void*, int, void *);

class Message {
public:
    
    typedef enum {
        REQ_UPDATE_SHADER_CMD = 0,
        REQ_NEXT_SHADER_CMD,
        REQ_PREV_SHADER_CMD,
        RESP_INDI
    }E_BoxWorldMsgType;
 
    typedef struct {
        unsigned char *dataP;
        int length;
    }EnCodedData_S;
    
    Message () {}
    Message(E_BoxWorldMsgType msg_type, string msg_content, void *user_data, msgCallbackFn cb_fn = NULL) {
        mMsgType = msg_type;
        mMsgContent = msg_content;
        mUserData = user_data;
        mMsgCbFnP = cb_fn;
    }
    Message(Json::Value &fromJson);
    
    ~Message() {
        if(mEncodedDataInfo.dataP)
            delete mEncodedDataInfo.dataP;
    }
    
    E_BoxWorldMsgType getType() {
        return mMsgType;
    }
    
    string getContent() {
        return mMsgContent;
    }
    
    void runCb(int ret_val, void *args) {
        if(mMsgCbFnP)
            mMsgCbFnP(mUserData, ret_val, args);
    }
    
    EnCodedData_S encode();
    
    int getEncodedDataSize() { return mEncodedDataInfo.length; }
    unsigned char *getEncodedDataP() { return mEncodedDataInfo.dataP; }
    
private:
    Json::Value *toJsonFormat();

    E_BoxWorldMsgType    mMsgType;
    string               mMsgContent;
    msgCallbackFn        mMsgCbFnP;
    void                *mUserData;
    Json::Value          mJsonFormatContent;
    EnCodedData_S        mEncodedDataInfo;
};

#endif

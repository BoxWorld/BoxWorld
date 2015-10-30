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

using namespace std;

typedef void (*msgCallbackFn)(void*, int, void *);

class Message {
public:
    
    typedef enum {
        UPDATE_SHADER_CMD = 0,
        NEXT_SHADER_CMD,
        PREV_SHADER_CMD
    }E_BoxWorldMsgType;
    
    Message () {}
    Message(E_BoxWorldMsgType msg_type, string msg_content, void *user_data, msgCallbackFn cb_fn = NULL) {
        mMsgType = msg_type;
        mMsgContent = msg_content;
        mUserData = user_data;
        mMsgCbFnP = cb_fn;
    }
    
    ~Message() {}
    
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
    
private:
    E_BoxWorldMsgType    mMsgType;
    string               mMsgContent;
    msgCallbackFn        mMsgCbFnP;
    void                *mUserData;
};

#endif

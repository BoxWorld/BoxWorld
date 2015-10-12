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

class Message {
public:
    
    typedef enum {
        UPDATE_SHADER_CMD = 0,
        NEXT_SHADER_CMD,
        PREV_SHADER_CMD
    }E_BoxWorldMsgType;
    
    Message () {}
    Message(E_BoxWorldMsgType msg_type, string msg_content) {
        mMsgType = msg_type;
        mMsgContent = msg_content;
    }
    
    ~Message() {}
    
    E_BoxWorldMsgType getType() {
        return mMsgType;
    }
    
private:
    E_BoxWorldMsgType mMsgType;
    string mMsgContent;
};

#endif

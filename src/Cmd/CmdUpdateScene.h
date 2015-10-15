//
//  CmdUpdateScene.h
//  BoxWorld
//
//  Created by Han Maokun on 10/12/15.
//
//

#ifndef BoxWorld_CmdUpdateScene_h
#define BoxWorld_CmdUpdateScene_h

#include "CmdReceiverIf.h"
#include "Message.h"

class CmdUpdateScene : public CmdIf {
public:
    CmdUpdateScene(CmdReceiverIf *cmd_receiver, Message *msg) {
        mCmdReceiver = cmd_receiver;
        mMsg = msg;
    }
    
    void exec() {
        mCmdReceiver->updateScene(mMsg);
    }
    
private:
    CmdReceiverIf *mCmdReceiver;
    Message *mMsg;
};

#endif

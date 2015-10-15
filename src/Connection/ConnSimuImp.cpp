//
//  ConnSimuImp.cpp
//  BoxWorld
//
//  Created by Han Maokun on 10/12/15.
//
//

#include "ConnSimuImp.h"
#include "ofUtils.h"
#include "Message.h"

void ConnSimuImp::init() {
    isConnected = true;
    
    bool suc = mFile.open(ofToDataPath("BoxWorldShader_0.json"), ofFile::ReadWrite, false);
    
    if(suc) {
        ofAddListener(mTimer.TIMER_REACHED, this, &ConnSimuImp::scanDataDir);
        mTimer.setup(1000, false);
    }
    
    return;
}

void ConnSimuImp::scanDataDir(ofEventArgs& args)
{
    if(mConnListener) {
        Message *msg = new Message(
                                   Message::UPDATE_SHADER_CMD,
                                   *new string(mFile.readToBuffer().getBinaryBuffer()));
        mConnListener->onIncomingMsg(msg);
        delete msg;
    }
}

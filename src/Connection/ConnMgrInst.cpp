//
//  ConnMgrInst.cpp
//  BoxWorld
//
//  Created by Han Maokun on 10/12/15.
//
//

#include <stdio.h>
#include "ConnMgrInst.h"
#include "CmdUpdateScene.h"

void ConnMgrInst::onIncomingMsg(Message *conn_msg)
{
    switch(conn_msg->getType()) {
        case Message::UPDATE_SHADER_CMD: {
            CmdUpdateScene *update_scene_cmd = new CmdUpdateScene(mCmdReceiver, conn_msg);
            storeAndExecute(update_scene_cmd);
            break;
        }
            
        default:
            break;
    }
}
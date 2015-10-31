//
//  CmdReceiverIf.h
//  BoxWorld
//
//  Created by Han Maokun on 10/12/15.
//
//

#ifndef BoxWorld_CmdReceiverIf_h
#define BoxWorld_CmdReceiverIf_h

#include "Message.h"

class CmdReceiverIf {
public:
    virtual ~CmdReceiverIf() {}
    
    virtual void updateScene(Message *msg) = 0;
};

#endif

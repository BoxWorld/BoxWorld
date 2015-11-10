//
//  ConnIf.h
//  BoxWorld
//
//  Created by Han Maokun on 10/12/15.
//
//

#ifndef BoxWorld_ConnIf_h
#define BoxWorld_ConnIf_h

#include <stdio.h>
#include <list>
#include "ConnListenerIf.h"
#include "Message.h"

using namespace std;

class ConnIf {
public:
    virtual ~ConnIf() {}
    
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void sendMsg(Message *msg) = 0;
protected:
    ConnListenerIf *mConnListener;
    bool isConnected;
};

#endif

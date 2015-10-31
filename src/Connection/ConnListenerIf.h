//
//  ConnListenerIf.h
//  BoxWorld
//
//  Created by Han Maokun on 10/12/15.
//
//

#ifndef BoxWorld_ConnListenerIf_h
#define BoxWorld_ConnListenerIf_h

#include "Message.h"

class ConnListenerIf {
public:
    virtual ~ConnListenerIf() {}
    
    virtual void onIncomingMsg(Message *conn_msg) = 0;
};

#endif

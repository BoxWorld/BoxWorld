//
//  MsgBuilder.h
//  BoxWorld
//
//  Created by Han Maokun on 11/10/15.
//
//

#ifndef __BoxWorld__MsgBuilder__
#define __BoxWorld__MsgBuilder__

#include <stdio.h>
#include "Message.h"

class MsgBuilder {
public:
    static MsgBuilder* get() {
        static MsgBuilder *inst = new MsgBuilder();
        return inst;
    }
    
    Message *buildRespMsg(bool result);
};
#endif /* defined(__BoxWorld__MsgBuilder__) */

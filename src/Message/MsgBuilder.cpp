//
//  MsgBuilder.cpp
//  BoxWorld
//
//  Created by Han Maokun on 11/10/15.
//
//

#include "MsgBuilder.h"

Message *MsgBuilder::buildRespMsg(bool result)
{
    string resp_result_str = (result? string("OK") : string("ERROR"));
    return new Message(Message::RESP_INDI, resp_result_str, NULL);
}

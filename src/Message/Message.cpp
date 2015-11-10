//
//  Message.cpp
//  BoxWorld
//
//  Created by Han Maokun on 11/10/15.
//
//

#include <stdio.h>
#include "Message.h"
#include "Protocol/slip.h"

Message::Message(Json::Value &fromJson)
{
    if(fromJson.empty()) return;
    
    mMsgType = static_cast<E_BoxWorldMsgType>(fromJson[BOXWORLD_MSG_TYPE_KEY].asInt());
    mMsgContent = fromJson[BOXWORLD_MSG_CONTENT_KEY].asString();
}

Json::Value *Message::toJsonFormat()
{
    mJsonFormatContent[BOXWORLD_MSG_TYPE_KEY]    = mMsgType;
    mJsonFormatContent[BOXWORLD_MSG_CONTENT_KEY] = mMsgContent;
    
    return &mJsonFormatContent;
}

Message::EnCodedData_S Message::encode()
{
    unsigned char* src_ptr = (unsigned char*)mJsonFormatContent.toStyledString().c_str();
    int src_size = mJsonFormatContent.toStyledString().size();
    mEncodedDataInfo.dataP = new unsigned char[src_size*2];
    mEncodedDataInfo.length = slip_esc(0,
                                       src_ptr,
                                       mEncodedDataInfo.dataP,
                                       src_size);
    
    return mEncodedDataInfo;
}
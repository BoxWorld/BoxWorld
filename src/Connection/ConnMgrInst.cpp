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
#include "Protocol/slip.h"

static unsigned char rxbuf[N_RX_BUFFERS][MAX_RCV_BUF_LEN];
static volatile unsigned int rxbuf_len[N_RX_BUFFERS];

static volatile int rxbuf_first = -1, rxbuf_next = 0;

void ConnMgrInst::onIncomingMsg(Message *conn_msg)
{
    switch(conn_msg->getType()) {
        case Message::REQ_UPDATE_SHADER_CMD: {
            CmdUpdateScene *update_scene_cmd = new CmdUpdateScene(mCmdReceiver, conn_msg);
            storeAndExecute(update_scene_cmd);
            break;
        }
            
        default:
            break;
    }
}

void ConnMgrInst::sendMsg(Message *msg)
{
#ifdef BOXWORLD_DEV_MAC
    mConnBleOSX->sendMsg(msg);
#endif
}

void ConnMgrInst::protocolReceiver(const unsigned char* buf, int len)
{
    if (len > MAX_RCV_BUF_LEN) {
        cout << "Too long msg, %u" << len << endl;
        return;
    }
    
    if (!len) {
        cout << "Zero length msg" << endl;
        return;
    }
    
    if (rxbuf_next == rxbuf_first) {
        /* Consider this error serious - CPU should be fast enough.. */
        cout << "%s - overrun\n" << __FUNCTION__ << endl;
        return;
    } else {
        memcpy(rxbuf[rxbuf_next], buf, len);
        
        rxbuf_len[rxbuf_next] = len;
        
        /* rxbuf_first should point to the first message */
        if (rxbuf_first < 0)
            rxbuf_first = rxbuf_next;
        
        rxbuf_next = (rxbuf_next + 1) & (N_RX_BUFFERS - 1);
    }
}


void ConnMgrInst::update() {
#ifdef BOXWORLD_DEV_MAC
    mConnSimu->update();
    mConnBleOSX->update();
#elif defined BOXWORLD_TARGET_ARM
#endif
    
    /* Parse received protocol data. */
    processProtocol();
}

/* 
 * TODO:
 *   should runs on another thread other than UI thread.
 */
void ConnMgrInst::processProtocol() {
    if (rxbuf_first >= 0) {
        const unsigned char *data_p = rxbuf[rxbuf_first];
        unsigned int len = rxbuf_len[rxbuf_first];
        
        Json::Reader reader;
        Json::Value  value;
        
        const char* begin = (char *)data_p;
        const char* end = begin + len;
        
        if (reader.parse(begin, end, value))
        {
            onIncomingMsg(new Message(value));
        }
        
        rxbuf_first = (rxbuf_first + 1) & (N_RX_BUFFERS - 1);
        if (rxbuf_first == rxbuf_next)
            rxbuf_first = -1;        
    }
}

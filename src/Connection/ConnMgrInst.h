//
//  ConnMgrInst.h
//  BoxWorld
//
//  Created by Han Maokun on 10/12/15.
//
//

#ifndef BoxWorld_ConnMgrInst_h
#define BoxWorld_ConnMgrInst_h

#include <stdio.h>
#include <string>

#include "BoxWorldConfig.h"
#include "ConnIf.h"
#include "CmdIf.h"
#include "CmdReceiverIf.h"

#ifdef BOXWORLD_DEV_MAC
#include "ConnSimuImp.h"
#include "ConnBleOSX.h"
#endif

#define N_RX_BUFFERS	4		/* min 2, power of two : 2, 4, 8, .. */

/*
 * Connection Management
 * --Init connection;
 * --Listen to connection event, msg;
 *
 * Command Invoker
 */
class ConnMgrInst : public ConnListenerIf {
public:
    static ConnMgrInst* get() {
        static ConnMgrInst *inst = new ConnMgrInst();
        return inst;
    }
    
    /* ConnListenerIf Implementation. */
    void onIncomingMsg(Message *conn_msg);
    void sendMsg(Message *msg);
    void protocolReceiver(const unsigned char* buf, int len);
    void update();
    
    void setCmdReceiver(CmdReceiverIf* cmd_receiver) {
        mCmdReceiver = cmd_receiver;
    }
    
    /* Invoke Cmd. */
    void storeAndExecute(CmdIf *cmd) {
        mCmdHistory.insert(mCmdHistory.end(), cmd); // optional
        cmd->exec();
    }
    
    
private:
    ConnMgrInst () {
        init();
    };
    
    ~ConnMgrInst () {
        list<ConnIf *>::iterator iter_conn = mConnections.begin();
        for(; iter_conn != mConnections.end(); iter_conn++) {
            if(*iter_conn) delete *iter_conn;
        }
        
        list<CmdIf *>::iterator iter_cmd = mCmdHistory.begin();
        for(; iter_cmd != mCmdHistory.end(); iter_cmd++) {
            if(*iter_cmd) delete *iter_cmd;
        }
    };
    
    /* Connection initialization entry point. */
    void init() {
        mConnections.clear();
#ifdef BOXWORLD_DEV_MAC
        mConnSimu = new ConnSimuImp(this);
        mConnections.insert(mConnections.end(), mConnSimu);
        
        mConnBleOSX = new ConnBleOSX(this);
        mConnections.insert(mConnections.end(), mConnBleOSX);
#elif defined BOXWORLD_TARGET_LINUX
#endif
    }
    
    void processProtocol();
    
    list<ConnIf *> mConnections;
    list<CmdIf  *> mCmdHistory;
    
#ifdef BOXWORLD_DEV_MAC
    ConnSimuImp *mConnSimu;
    ConnBleOSX  *mConnBleOSX;
#endif

    CmdReceiverIf *mCmdReceiver;
};

#endif

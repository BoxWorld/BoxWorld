//
//  intelRealsenseMgr.h
//  BoxWorld
//
//  Created by Han Maokun on 1/22/16.
//
//

#ifndef intelRealsenseMgr_h
#define intelRealsenseMgr_h

#include <stdio.h>
#include <stdlib.h>
#include "BoxWorldConfig.h"
#include "rs.hpp"
#include "ThreadIf.h"
#include "ofThread.h"
#include "DepthSensorImp.h"

#define RESOLUTION_WIDTH  640
#define RESOLUTION_HEIGHT 480

class intelRealsenseThread;

class intelrsMgr {
public:
    static intelrsMgr* get() {
        static intelrsMgr *inst = new intelrsMgr();
        return inst;
    }
    
    intelrsMgr();
    virtual ~intelrsMgr();
    
    bool isValid() { return mValid; }
    void stream();
    void startCapture(DepthSensorImp *owner);
    uint8_t *getDepthBuf();
    void stopCapture();
    
private:
    bool                     mValid;
    rs::device              *mDev;
    uint8_t                  mBufIdx;
    uint8_t                 *mDepthData[2];
    uint16_t                 mOneMeter;
    intelRealsenseThread    *mRSThread;
    DepthSensorImp          *mOwner;
};

class intelRealsenseThread : public ofThread {
public:
    intelRealsenseThread(intelrsMgr *rs_mgr, BoxWorldThreadCallbackIf *callback_inst) {
        mRsMgrP = rs_mgr;
        mCallbackInst = callback_inst;
    }
    ~intelRealsenseThread() {}
    
    void threadedFunction() {
        while(isThreadRunning()) {
            /* custom work here. */
            mRsMgrP->stream();
            
            if(mCallbackInst)
                mCallbackInst->threadCallBack(NULL);
            
            stopThread();
        }
    }
private:
    intelrsMgr                  *mRsMgrP;
    BoxWorldThreadCallbackIf    *mCallbackInst;
};

#endif /* intelRealsenseMgr_h */

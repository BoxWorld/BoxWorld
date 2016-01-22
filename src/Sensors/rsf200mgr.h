#ifndef RSF200MGR_H
#define RSF200MGR_H

#include <stdio.h>
#include <stdlib.h>
#include "ivcam.h"
#include "BoxWorldConfig.h"

using namespace ivcam_env;

class rsf200mgr {
public:
    static rsf200mgr* get() {
        static rsf200mgr *inst = new rsf200mgr();
        return inst;
    }

    rsf200mgr();
    virtual ~rsf200mgr();

    bool startCapture();
    uint8_t *getDepthBuf();
    bool stopCapture();

private:
    Ivcam           mIvcam;
    IVCAM_STATUS    mStatus;
    Frame           mFrame;
    uint8_t        *mColoredDepth;
};

#endif // RSF200MGR_H

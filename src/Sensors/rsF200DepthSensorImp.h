#ifndef __RSF200DEPTHSENSORIMG_H__
#define __RSF200DEPTHSENSORIMG_H__

#include "DepthSensorImp.h"

/*
 * Realsense F200 implementation of depth sensor.
 */

class rsF200DepthSensorImp : public DepthSensorImp {
public:
        rsF200DepthSensorImp() {
                inited = false;
                mBuf = nullptr;
                init();
        }

    virtual ~rsF200DepthSensorImp() {
        inited = false;
        if(mBuf) delete mBuf;
    }

    void init() override;
    DepthSensorAttrib getAttrib() override;
    unsigned char *getDepthBufPtr() override;
    void dispose() override;
    bool isValid() override;
    int getDataType0() override;
    int getDataType1() override;

private:
        DepthSensorAttrib mDepthSensorAttrib;
        unsigned char *mBuf;
        bool inited;
};

#endif

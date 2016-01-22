//
//  RSSensorImp.h
//  BoxWorld
//
//  Created by Han Maokun on 1/22/16.
//
//

#ifndef RSSensorImp_h
#define RSSensorImp_h

#include "DepthSensorImp.h"

/*
 * Intel Realsense implementation of depth sensor.
 */

class RSSensorImp : public DepthSensorImp {
public:
    RSSensorImp() {
        inited = false;
        mBuf = nullptr;
        init();
    }
    
    virtual ~RSSensorImp() {
        inited = false;
        if(mBuf) delete mBuf;
    }
    
    void init() override;
    DepthSensorAttrib getAttrib() override;
    unsigned char *getDepthBufPtr() override;
    void dispose() override;
    bool isValid() override;
    int getDataType0() override;
    int getFormat() override;
    int getDataType1() override;
    
private:
    DepthSensorAttrib   mDepthSensorAttrib;
    unsigned char       *mBuf;
    bool                inited;
};

#endif /* RSSensorImp_h */

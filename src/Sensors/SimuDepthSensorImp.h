#ifndef __SIMUDEPTHSENSORIMG_H__
#define __SIMUDEPTHSENSORIMG_H__

#include "DepthSensorImp.h"

/*
 * Simulation implementation of depth sensor when no
 * real hardware is attached.
 */

class SimuDepthSensorImp : public DepthSensorImp {
public:
	SimuDepthSensorImp() {
		inited = false;
		mBuf = nullptr;
		init();
	}

    virtual ~SimuDepthSensorImp() {
    	inited = false;
    	if(mBuf) delete mBuf;
    }

    void init() override;
    DepthSensorAttrib getAttrib() override;
    ofTexture & getDepthBufTexture() override;
    void dispose() override;
	bool isValid() override;
    int getDataType0() override;
    int getFormat() override;
    int getDataType1() override;

private:
	DepthSensorAttrib mDepthSensorAttrib;
    ofFloatImage      mDepthFloatImage;
	unsigned char *mBuf;
	bool inited;
};

#endif

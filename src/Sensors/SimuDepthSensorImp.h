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

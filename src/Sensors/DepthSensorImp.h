#ifndef __DEPTHSENSORIMG_H__
#define __DEPTHSENSORIMG_H__

#include "DepthSensorConfig.h"

/*
 * Interface for depth sensor implementation.
 */

class DepthSensorImp {
public:
    virtual ~DepthSensorImp() {
    }

	virtual void init() = 0;
	virtual DepthSensorAttrib getAttrib() = 0;
	virtual unsigned char *getDepthBufPtr() = 0;
        virtual int getDataType0() = 0;
        virtual int getDataType1() = 0;
	virtual void dispose() = 0;
	virtual bool isValid() = 0;
};

#endif

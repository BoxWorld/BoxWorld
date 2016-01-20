#ifndef __DEPTHSENSOR_H__
#define __DEPTHSENSOR_H__

#include "DepthSensorConfig.h"
#include "DepthSensorImp.h"

/*
 * DepthSensor As uniform interface for depth sensor devices, which
 * can be Kinect, Intel Realsense or other structure sensors,
 * also there's a 'SimuDepthSensorImp' for simulation purpose,
 * when no hardware sensor is attached.
 */
class DepthSensor {
public:
	DepthSensor(DepthSensorImp *imp) {
		mImp = imp;
		init();
	}

	~DepthSensor() {
		dispose();
	}

	void setIml(DepthSensorImp *imp) {
		if(mImp != nullptr) mImp->dispose();

		mImp = imp;
		init();
	}

	void init() {
		mImp->init();
	}

	DepthSensorAttrib getAttrib() {
		return mImp->getAttrib();
	}

	unsigned char *getDepthBufPtr() {
		return mImp->getDepthBufPtr();
	}

	void dispose() {
		if(mImp != nullptr) {
			delete mImp;
			mImp = NULL;
		}
	}

protected:
	DepthSensorImp *mImp;
};

#endif

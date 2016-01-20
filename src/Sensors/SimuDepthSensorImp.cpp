#include "SimuDepthSensorImp.h"
#include "SimuDepthmapModel.h"
#include "ofMain.h"

#define SIMU_DEPTH_BUF_WIDTH  640
#define SIMU_DEPTH_BUF_HEIGHT 480

void SimuDepthSensorImp::init() {

	if(inited) return;
	
	mDepthSensorAttrib.width  = SIMU_DEPTH_BUF_WIDTH;
	mDepthSensorAttrib.height = SIMU_DEPTH_BUF_HEIGHT;

	if(!mBuf) {
		mBuf = new unsigned char[mDepthSensorAttrib.width * mDepthSensorAttrib.height];
		for(int r=0; r<SIMU_DEPTH_BUF_HEIGHT; r++){
			for(int c=0; c<SIMU_DEPTH_BUF_WIDTH; c++){
				mBuf[640*r+c] = kSimuDepthmapData[640*r+c];
			}
		}
	}
	inited = true;
}

DepthSensorAttrib SimuDepthSensorImp::getAttrib() {
    return mDepthSensorAttrib;
}

unsigned char *SimuDepthSensorImp::getDepthBufPtr() {
	if(!inited) {
		init();
	}

	for(int r=0; r<SIMU_DEPTH_BUF_HEIGHT; r++){
		for(int c=0; c<SIMU_DEPTH_BUF_WIDTH; c++){
			int rand_val = 0;//rand() % 2 + 1;
			int new_val = kSimuDepthmapData[640*r+c] + rand_val;
			if(new_val > 255) new_val = 255;
			mBuf[640*r+c] = new_val;
			//int cur_val = mBuf[640*r+c] + 1;
			//if(cur_val > 255) cur_val = 0;
			//mBuf[640*r+c] = cur_val & 0xff;
		}
	}

	return mBuf;
}

int SimuDepthSensorImp::getDataType0() { return GL_R8; }
int SimuDepthSensorImp::getDataType1() { return GL_UNSIGNED_BYTE; }

void SimuDepthSensorImp::dispose() {}

bool SimuDepthSensorImp::isValid() {
	return true;
}

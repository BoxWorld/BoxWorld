#include "SimuDepthSensorImp.h"
#include "SimuDepthmapModel.h"
#include "ofMain.h"
#include "DepthTransMgr.h"
#include "BoxWorldWindowAttrib.h"

#define SIMU_DEPTH_BUF_WIDTH  640
#define SIMU_DEPTH_BUF_HEIGHT 480

void SimuDepthSensorImp::init() {

	if(inited) return;
	
    DepthTransMgr::get();
	mDepthSensorAttrib.width  = SIMU_DEPTH_BUF_WIDTH;
	mDepthSensorAttrib.height = SIMU_DEPTH_BUF_HEIGHT;
    mDepthFloatImage.allocate(SIMU_DEPTH_BUF_WIDTH, SIMU_DEPTH_BUF_HEIGHT, OF_IMAGE_GRAYSCALE);

	if(!mBuf) {
		mBuf = new unsigned char[mDepthSensorAttrib.width * mDepthSensorAttrib.height];
		for(int r=0; r<SIMU_DEPTH_BUF_HEIGHT; r++){
			for(int c=0; c<SIMU_DEPTH_BUF_WIDTH; c++){
				mBuf[SIMU_DEPTH_BUF_WIDTH*r+c] = kSimuDepthmapData[SIMU_DEPTH_BUF_WIDTH*r+c];
			}
		}
	}
	inited = true;
}

DepthSensorAttrib SimuDepthSensorImp::getAttrib() {
    return mDepthSensorAttrib;
}

ofTexture & SimuDepthSensorImp::getDepthBufTexture() {
	if(!inited) {
		init();
	}
    
    float *depthFloatPixels = mDepthFloatImage.getPixels();

	for(int r=0; r<SIMU_DEPTH_BUF_HEIGHT; r++){
		for(int c=0; c<SIMU_DEPTH_BUF_WIDTH; c++){
            int rand_val = 0;//rand() % 11;
			int new_val = kSimuDepthmapData[SIMU_DEPTH_BUF_WIDTH*r+c] + rand_val;
			if(new_val > 255) new_val = 255;

			depthFloatPixels[SIMU_DEPTH_BUF_WIDTH*r+c] = ofMap(new_val, 0, 255, 0.0f,1.0f);
		}
	}
    mDepthFloatImage.setFromPixels(depthFloatPixels, SIMU_DEPTH_BUF_WIDTH, SIMU_DEPTH_BUF_HEIGHT, OF_IMAGE_GRAYSCALE );

    return DepthTransMgr::get()->getTransTexture(mDepthFloatImage.getTexture(),
                                                 BoxWorldWindowAttrib::getInst().minDist,
                                                 BoxWorldWindowAttrib::getInst().maxDist);
    //return mDepthFloatImage.getTexture();
}

int SimuDepthSensorImp::getDataType0() { return GL_R8; }
int SimuDepthSensorImp::getFormat(){ return GL_RED; }
int SimuDepthSensorImp::getDataType1() { return GL_UNSIGNED_BYTE; }

void SimuDepthSensorImp::dispose() {}

bool SimuDepthSensorImp::isValid() {
	return true;
}

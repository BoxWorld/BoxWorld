#include "rsF200DepthSensorImp.h"
#include "rsf200mgr.h"
#include "ofMain.h"

#define F200_DEPTH_BUF_WIDTH  640
#define F200_DEPTH_BUF_HEIGHT 480

rsf200mgr *rsf200_inst;

void rsF200DepthSensorImp::init() {
        if(inited) return;

        mDepthSensorAttrib.width  = F200_DEPTH_BUF_WIDTH;
        mDepthSensorAttrib.height = F200_DEPTH_BUF_HEIGHT;

        rsf200_inst = rsf200mgr::get();
        rsf200_inst->startCapture();

        inited = true;
}

DepthSensorAttrib rsF200DepthSensorImp::getAttrib() {
    return mDepthSensorAttrib;
}

unsigned char *rsF200DepthSensorImp::getDepthBufPtr() {
        if(!inited) {
            init();
        }

        return rsf200_inst->getDepthBuf();
}

int rsF200DepthSensorImp::getDataType0() { return GL_RGB; }
int rsF200DepthSensorImp::getDataType1() { return GL_UNSIGNED_BYTE; }

void rsF200DepthSensorImp::dispose() {
    rsf200_inst->stopCapture();
}

bool rsF200DepthSensorImp::isValid() {
        return true;
}

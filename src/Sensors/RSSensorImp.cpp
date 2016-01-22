//
//  RSSensorImp.cpp
//  BoxWorld
//
//  Created by Han Maokun on 1/22/16.
//
//

#include "RSSensorImp.h"
#include "intelRealsenseMgr.h"
#include "ofMain.h"

#define F200_DEPTH_BUF_WIDTH  640
#define F200_DEPTH_BUF_HEIGHT 480

intelrsMgr *interlrs_inst;

void RSSensorImp::init() {
    if(inited) return;
    
    mDepthSensorAttrib.width  = F200_DEPTH_BUF_WIDTH;
    mDepthSensorAttrib.height = F200_DEPTH_BUF_HEIGHT;
    
    interlrs_inst = intelrsMgr::get();
    interlrs_inst->startCapture(this);
    
    inited = true;
}

DepthSensorAttrib RSSensorImp::getAttrib() {
    return mDepthSensorAttrib;
}

unsigned char *RSSensorImp::getDepthBufPtr() {
    if(!inited) {
        init();
    }
    
    return interlrs_inst->getDepthBuf();
}

int RSSensorImp::getDataType0() { return GL_RGB; }
int RSSensorImp::getFormat() { return GL_RGB; }
int RSSensorImp::getDataType1() { return GL_UNSIGNED_BYTE; }

void RSSensorImp::dispose() {
    interlrs_inst->stopCapture();
}

bool RSSensorImp::isValid() {
    return interlrs_inst->isValid();
}


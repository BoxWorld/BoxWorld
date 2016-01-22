/*
 * DepthSensorMgr.h
 *
 *  Created on: Oct 24, 2015
 *      Author: Han Maokun
 */

#ifndef SYNTH_IDE_DEPTH_INPUT_DEPTHSENSORMGR_H_
#define SYNTH_IDE_DEPTH_INPUT_DEPTHSENSORMGR_H_

#include "ofMain.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "DepthSensor.h"
#include "SimuDepthSensorImp.h"
#include "RSSensorImp.h"

class DepthSensorMgr {
public:
	static DepthSensorMgr &getInst() {
		static DepthSensorMgr 			instance;
        return instance;
	}

	DepthSensor *getDepthSensor() {
		return mDepthSensor;
	}

	unsigned char *getDefaultDepthBufPtr() {
		return mSimuDepthSensorImp->getDepthBufPtr();
	}

	void dispose() {
		mRunning = false;
	}
    
    int getDepthmapTexId() {
        if (mTexId == 0) {
            glGenTextures(1, &mTexId);
            glBindTexture(GL_TEXTURE_2D, mTexId);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        } else {
            glBindTexture(GL_TEXTURE_2D, mTexId);
        }
        
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     mDepthSensor->getDataType0(),
                     mDepthSensor->getAttrib().width,
                     mDepthSensor->getAttrib().height,
                     0,
                     mDepthSensor->getFormat(),
                     mDepthSensor->getDataType1(),
                     mDepthSensor->getDepthBufPtr()
                     );
        
        glBindTexture(GL_TEXTURE_2D, 0);
        
        return mTexId;
    }
    
    void setImp(DepthSensorImp *depth_sensor_imp){
        cout << "use custom implementation." << endl;
        mDepthSensor->setIml(mRSDepthSensorImp);
    }

private:
	DepthSensorMgr() :
		mSimuDepthSensorImp(NULL),
		mDepthSensor(NULL){
		init();
	}

	virtual ~DepthSensorMgr() {}

	void init(){
		mSimuDepthSensorImp = new SimuDepthSensorImp();
		mDepthSensor = new DepthSensor(mSimuDepthSensorImp);
        
        mRSDepthSensorImp = new RSSensorImp();
        cout << "use simu depth implementation." << endl;
	}
    
    DepthSensorImp                  *mRSDepthSensorImp;
    DepthSensorImp 					*mSimuDepthSensorImp;
    DepthSensor 					*mDepthSensor;
    GLuint                           mTexId = 0;
    bool							 mRunning;
};



#endif /* SYNTH_IDE_DEPTH_INPUT_DEPTHSENSORMGR_H_ */

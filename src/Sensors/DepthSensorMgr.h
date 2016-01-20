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

#ifdef TARGET_LINUX
#include "rsF200DepthSensorImp.h"
#endif

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
                     GL_RED,
                     mDepthSensor->getDataType1(),
                     mDepthSensor->getDepthBufPtr()
                     );
        
        glBindTexture(GL_TEXTURE_2D, 0);
        
        return mTexId;
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
        
#ifdef TARGET_LINUX
        mF200DepthSensorImp = new rsF200DepthSensorImp();
        if(mF200DepthSensorImp->isValid()) {
                cout << "use rsF200 implementation." << endl;
                mDepthSensor->setIml(mF200DepthSensorImp);
        }else
#endif
        {
            cout << "use simu depth implementation." << endl;
        }
	}

	void useSimulation() { mDepthSensor->setIml(mSimuDepthSensorImp); }
        void useF200() { mDepthSensor->setIml(mF200DepthSensorImp); }

    DepthSensorImp 					*mSimuDepthSensorImp;
    DepthSensorImp                  *mF200DepthSensorImp;
    DepthSensor 					*mDepthSensor;
    GLuint                           mTexId = 0;
    bool							 mRunning;
};



#endif /* SYNTH_IDE_DEPTH_INPUT_DEPTHSENSORMGR_H_ */

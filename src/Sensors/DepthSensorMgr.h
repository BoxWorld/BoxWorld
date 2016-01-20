/*
 * DepthSensorMgr.h
 *
 *  Created on: Oct 24, 2015
 *      Author: Han Maokun
 */

#ifndef SYNTH_IDE_DEPTH_INPUT_DEPTHSENSORMGR_H_
#define SYNTH_IDE_DEPTH_INPUT_DEPTHSENSORMGR_H_

//#include <GL/glew.h>
//#include <GL/gl.h>
#include "ofMain.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "DepthSensor.h"
#include "SimuDepthSensorImp.h"

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
                     GL_R8,
                     mDepthSensor->getAttrib().width,
                     mDepthSensor->getAttrib().height,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
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

		cout << "use simu depth implementation." << endl;
		//mKinect1473DepthSensorImp = new Kinect1473DepthSensorImp(mUsbContextP);
		//if(mKinect1473DepthSensorImp->isValid()) {
		//	cout << "use kinect1473 implementation." << endl;
		//	mDepthSensor->setIml(mKinect1473DepthSensorImp);
		//}
	}

	void useSimulation() { mDepthSensor->setIml(mSimuDepthSensorImp); }
	//void useKinect1473() { mDepthSensor->setIml(mKinect1473DepthSensorImp); }

    DepthSensorImp 					*mSimuDepthSensorImp;
    DepthSensor 					*mDepthSensor;
    GLuint                           mTexId = 0;
    bool							 mRunning;
};



#endif /* SYNTH_IDE_DEPTH_INPUT_DEPTHSENSORMGR_H_ */

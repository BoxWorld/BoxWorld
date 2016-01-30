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

	void dispose() {
		mRunning = false;
	}
    
    void averageData() {
        mFrames[mFrameCtr] = mDepthSensor->getDepthBufTexture();
        mAverageTexture = average(mFrames, 10);
        mFrameCtr = (mFrameCtr+1) % 10;
    }
    
    int getDepthmapTexId() {
        int tex_id = mAverageTexture.getTextureData().textureID;
        //int tex_id = mDepthSensor->getDepthBufTexture().getTextureData().textureID;
        return tex_id;
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
        
        mWidth = mDepthSensor->getAttrib().width;
        mHeight = mDepthSensor->getAttrib().height;
        
        string vertexAverageShader = "#version 330\nlayout(location = 0) \
            in vec3 VertexPosition; \
            uniform mat4 TileMatrix = mat4(1.0); \
            uniform vec2 resolution; \
            out vec4 glFragCoord; \
            void main() { \
                vec2 v = VertexPosition.xy; \
                v.x = v.x * TileMatrix[0][0] + TileMatrix[3][0]; \
                v.y = v.y * TileMatrix[1][1] + TileMatrix[3][1]; \
                v = v * 0.5 + 0.5; \
                glFragCoord.xy = v * resolution; \
                gl_Position = vec4(VertexPosition, 1.0); \
            }";
        
        string fragmentAverageShader = "#version 330\nlayout(location = 0) \
            out vec4 glFragColor; \
            in vec4 glFragCoord; \
            uniform vec3 iResolution; \
            uniform sampler2D tex0;\
            uniform sampler2D tex1;\
            uniform sampler2D tex2;\
            uniform sampler2D tex3;\
            uniform sampler2D tex4;\
            uniform sampler2D tex5;\
            uniform sampler2D tex6;\
            uniform sampler2D tex7;\
            uniform sampler2D tex8;\
            uniform sampler2D tex9;\
            uniform float steps;\
            void main(void) {\
                vec2 st = glFragCoord.xy / iResolution.xy;\
                float a = texture(tex0, st).r;\
                float b = texture(tex1, st).r;\
                float c = texture(tex2, st).r;\
                float d = texture(tex3, st).r;\
                float e = texture(tex4, st).r;\
                float f = texture(tex5, st).r;\
                float g = texture(tex6, st).r;\
                float h = texture(tex7, st).r;\
                float i = texture(tex8, st).r;\
                float j = texture(tex9, st).r;\
                float value = a + b + c + d + e + f + g + h + i + j;\
                value /= steps;\
                glFragColor.rgb = vec3( value, value, value);\
                glFragColor.a = 1.0;\
            }";
        mAverageShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexAverageShader);
        mAverageShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentAverageShader);
        mAverageShader.linkProgram();
        mAverageTexture.allocate(mWidth, mHeight, GL_RGB32F);
        
        mDrawPlane.set(mWidth, mHeight);
        mDrawPlane.setPosition(mWidth, mHeight, 0);
        mDrawPlane.setResolution(2, 2);
        
        mRSDepthSensorImp = new RSSensorImp();
        cout << "use simu depth implementation." << endl;
        
        mFbo.allocate(mWidth, mHeight, GL_RGB32F);
        mFbo.begin();
        ofClear(255,255);
        mFbo.end();
        
        delete [] mFrames;
        mFrames = new ofTexture[10];
        for (int i = 0; i < 10; i++){
            mFrames[i] = ofTexture();
            mFrames[i].allocate(mWidth, mHeight, GL_RGB32F);
            mFrames[i] = mFbo.getTexture();
        }
        mFrameCtr = 0;
	}
    
    ofTexture average(ofTexture *framesArray, int nTotal)
    {
        ofVec2f win_size = ofVec2f(mWidth, mHeight);

        mFbo.begin();
        {
            ofClear(0, 0, 0, 255);
            mAverageShader.begin();
            {
                for (int i = 0; i < nTotal; i++){
                    string texName = "tex"+ofToString(i);
                    mAverageShader.setUniformTexture(texName.c_str() , framesArray[i] , i );
                }
                
                mAverageShader.setUniform1f("steps", (float)nTotal);
                mAverageShader.setUniform2f("resolution", win_size);
                mAverageShader.setUniform3f("iResolution", win_size.x, win_size.y, 0.0);

                mDrawPlane.draw();
            }
            mAverageShader.end();
        }
        mFbo.end();
        
        return mFbo.getTexture();
    }
    
    ofPlanePrimitive mDrawPlane;
    ofShader         mAverageShader;
    ofFbo            mFbo;
    ofTexture       *mFrames, mAverageTexture;
    DepthSensorImp  *mRSDepthSensorImp;
    DepthSensorImp 	*mSimuDepthSensorImp;
    DepthSensor 	*mDepthSensor;
    bool			 mRunning;
    int              mFrameCtr, mWidth, mHeight;
};



#endif /* SYNTH_IDE_DEPTH_INPUT_DEPTHSENSORMGR_H_ */

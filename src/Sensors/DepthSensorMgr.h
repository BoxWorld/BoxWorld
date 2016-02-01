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
    
    void blurData() {
        ofVec2f win_size = ofVec2f(mWidth, mHeight);
        int actual, other;
        for(int i=0; i<6; i++) {
            actual = i%2; other = (i+1)%2;
            mFboBlur[actual].begin();
            {
                ofClear(0, 0, 0, 255);
                mBlurShader.begin();
                {
                    mBlurShader.setUniform2f("resolution", win_size);
                    mBlurShader.setUniform3f("iResolution", win_size.x, win_size.y, 0.0);
                    mBlurShader.setUniform1f("fade_const", 0.0005);
                    if(i == 0) {
                        mBlurShader.setUniformTexture("tex", mAverageTexture, 0);
                    }else {
                        mBlurShader.setUniformTexture("tex", mFboBlur[other], 0);
                    }

                    mDrawPlane.draw();
                }
                mBlurShader.end();
            }
            mFboBlur[actual].end();
        }
    }
    
    void preProcessingData() {
        averageData();
        //blurData();
    }
    
    int getDepthmapTexId() {
        int tex_id = mAverageTexture.getTextureData().textureID;
        //int tex_id = mFboBlur[0].getTexture().getTextureData().textureID;
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

    void initAverageShader() {
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
        mAverageShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        mAverageShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentAverageShader);
        mAverageShader.linkProgram();
        mAverageTexture.allocate(mWidth, mHeight, GL_RGB32F);
        
        mFboAverage.allocate(mWidth, mHeight, GL_RGB32F);
        mFboAverage.begin();
        ofClear(255,255);
        mFboAverage.end();
    }
    
    void initBlurShader() {
        string fragmentBlurShader = "#version 330\nlayout(location = 0) \
        out vec4 glFragColor; \
        in vec4 glFragCoord; \
        uniform vec3 iResolution; \
        float kernel[9];\
        uniform sampler2D tex;\
        uniform float fade_const;\
        vec2 offset[9];\
        void main(void){\
            vec2 st = glFragCoord.xy;\
            vec4 sum = vec4(0.0);\
            offset[0] = vec2(-1.0, -1.0);\
            offset[1] = vec2(0.0, -1.0);\
            offset[2] = vec2(1.0, -1.0);\
            offset[3] = vec2(-1.0, 0.0);\
            offset[4] = vec2(0.0, 0.0);\
            offset[5] = vec2(1.0, 0.0);\
            offset[6] = vec2(-1.0, 1.0);\
            offset[7] = vec2(0.0, 1.0);\
            offset[8] = vec2(1.0, 1.0);\
            kernel[0] = 1.0/16.0;   kernel[1] = 2.0/16.0;   kernel[2] = 1.0/16.0;\
            kernel[3] = 2.0/16.0;   kernel[4] = 4.0/16.0;   kernel[5] = 2.0/16.0;\
            kernel[6] = 1.0/16.0;   kernel[7] = 2.0/16.0;   kernel[8] = 1.0/16.0;\
            int i = 0;\
            for (i = 0; i < 4; i++){\
                vec4 tmp = texture(tex, (st + offset[i]) / iResolution.xy);\
                sum += tmp * kernel[i];\
            }\
            for (i = 5; i < 9; i++){\
                vec4 tmp = texture(tex, (st + offset[i]) / iResolution.xy);\
                sum += tmp * kernel[i];\
            }\
            vec4 color0 = texture(tex, (st + offset[4]) / iResolution.xy);\
            sum += color0 * kernel[4];\
            glFragColor = (1.0 - fade_const) * color0 +  fade_const * vec4(sum.rgb, color0.a);\
        }";
        mBlurShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
        mBlurShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentBlurShader);
        mBlurShader.linkProgram();
        
        mFboBlur[0].allocate(mWidth, mHeight, GL_RGB32F);
        mFboBlur[0].begin();
        ofClear(255,255);
        mFboBlur[0].end();
        mFboBlur[1].allocate(mWidth, mHeight, GL_RGB32F);
        mFboBlur[1].begin();
        ofClear(255,255);
        mFboBlur[1].end();
    }
    
    void init(){
        mSimuDepthSensorImp = new SimuDepthSensorImp();
        mDepthSensor = new DepthSensor(mSimuDepthSensorImp);
        
        mWidth = mDepthSensor->getAttrib().width;
        mHeight = mDepthSensor->getAttrib().height;
        
        initAverageShader();
        initBlurShader();
        
        mDrawPlane.set(mWidth, mHeight);
        mDrawPlane.setPosition(mWidth, mHeight, 0);
        mDrawPlane.setResolution(2, 2);
        
        mRSDepthSensorImp = new RSSensorImp();
        cout << "use simu depth implementation." << endl;
        
        delete [] mFrames;
        mFrames = new ofTexture[10];
        for (int i = 0; i < 10; i++){
            mFrames[i] = ofTexture();
            mFrames[i].allocate(mWidth, mHeight, GL_RGB32F);
            mFrames[i] = mFboAverage.getTexture();
        }
        mFrameCtr = 0;
	}
    
    ofTexture average(ofTexture *framesArray, int nTotal)
    {
        ofVec2f win_size = ofVec2f(mWidth, mHeight);

        mFboAverage.begin();
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
        mFboAverage.end();
        
        return mFboAverage.getTexture();
    }
    
    ofPlanePrimitive mDrawPlane;
    ofShader         mAverageShader, mBlurShader;
    ofFbo            mFboAverage, mFboBlur[2];
    ofTexture       *mFrames, mAverageTexture;
    DepthSensorImp  *mRSDepthSensorImp;
    DepthSensorImp 	*mSimuDepthSensorImp;
    DepthSensor 	*mDepthSensor;
    bool			 mRunning;
    int              mFrameCtr, mWidth, mHeight;
    
    string vertexShader = "#version 330\nlayout(location = 0) \
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
};



#endif /* SYNTH_IDE_DEPTH_INPUT_DEPTHSENSORMGR_H_ */

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
#include "BoxWorldWindowAttrib.h"

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
        mBufferFull = false;
        if(mDepthSensor) delete mDepthSensor;
	}
    
    void smoothData() {
        int count = 0;
        ofTexture tex = mDepthSensor->getDepthBufTexture();
        count = mFrameCtr;
        if(mBufferFull) {
            count = 10;
        }
        mSmoothTexture = smooth(mFrames, tex, mFrameCtr, count);
        
        mFrames[mFrameCtr].clear();
        mFrames[mFrameCtr] = tex;
        
        mFrameCtr = (mFrameCtr+1) % 10;
        if(!mBufferFull && (mFrameCtr == 0)) {
            mBufferFull = true;
        }
        //mSmoothTexture = mDepthSensor->getDepthBufTexture();
    }
    
    void gaussianBlur() {
        ofTexture srcTex = mSmoothTexture;
        for(int i=0; i<BoxWorldWindowAttrib::getInst().blurCtr; i++) {
            mFboHGaussian.begin();
            {
                mGaussianBlurHShader.begin();
                ofClear(255, 255);
                {
                    mGaussianBlurHShader.setUniformTexture("sTexture", srcTex, 0);
                    
                    mDrawPlane.draw();
                }
                mGaussianBlurHShader.end();
            }
            mFboHGaussian.end();
            
            mFboVGaussian.begin();
            {
                mGaussianBlurVShader.begin();
                ofClear(255, 255);
                {
                    mGaussianBlurVShader.setUniformTexture("sTexture", mFboHGaussian.getTexture(), 0);
                    
                    mDrawPlane.draw();
                }
                mGaussianBlurVShader.end();
            }
            mFboVGaussian.end();
            
            srcTex = mFboVGaussian.getTexture();
        }
    }
    
    void preProcessingData() {
        smoothData();
        //gaussianBlur();
    }
    
    int getDepthmapTexId() {
        //int tex_id = mDepthSensor->getDepthBufTexture();
        int tex_id = mSmoothTexture.getTextureData().textureID;
        //int tex_id = mFboVGaussian.getTexture().getTextureData().textureID;

        return tex_id;
    }
    
    void setImp(DepthSensorImp *depth_sensor_imp){
        cout << "use custom implementation." << endl;
        mDepthSensor->setIml(depth_sensor_imp);
    }

private:
	DepthSensorMgr() :
		mSimuDepthSensorImp(NULL),
		mDepthSensor(NULL){
		init();
	}

	virtual ~DepthSensorMgr() {
        dispose();
    }

    void initCopyShader() {
        string fragmentCopyShader = "#version 330\nlayout(location = 0) \
        out vec4 glFragColor; \
        in vec4 glFragCoord; \
        uniform vec3 iResolution; \
        uniform sampler2D tex;\
        void main(void) {\
            vec2 st = glFragCoord.xy / iResolution.xy;\
            float v   = texture(tex, st).r;\
            glFragColor.rgb = vec3( v, v, v);\
            glFragColor.a = 1.0;\
        }";
        mCopyShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexCopyShader);
        mCopyShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentCopyShader);
        mCopyShader.linkProgram();
    }
    
    void initSmoothShader() {
        string fragmentSmoothShader = "#version 330\nlayout(location = 0) \
        out vec4 glFragColor; \
        in vec4 glFragCoord; \
        uniform vec3 iResolution; \
        uniform sampler2D tex_head;\
        uniform sampler2D tex_tail;\
        uniform sampler2D tex_smooth;\
        uniform float count;\
        void main(void) {\
            vec2 st = glFragCoord.xy / iResolution.xy;\
            float head   = texture(tex_head, st).r;\
            float tail   = texture(tex_tail, st).r;\
            float avg = texture(tex_smooth, st).r;\
            float value = 0.0;\
            if(count < 10.0) {\
                value = avg*count + head;\
                value = value/(count + 1.0);\
            }else {\
                value = avg*count - tail + head;\
                value = (head + tail)/2.0; \
            }\
            glFragColor.rgb = vec3( value, value, value);\
            glFragColor.a = 1.0;\
        }";
        mSmoothShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexSmoothShader);
        mSmoothShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentSmoothShader);
        mSmoothShader.linkProgram();
        mSmoothTexture.allocate(mWidth, mHeight, GL_RGBA);
        mSmoothTexture.clear();

        mFboSmooth.allocate(mWidth, mHeight, GL_RGBA);
        mFboSmooth.begin();
        ofClear(255,255);
        mFboSmooth.end();
    }
    
    void initGaussianBlurShader() {
        string fragmentGaussianBlurShader = "#version 330\nlayout(location = 0) \
        out vec4 glFragColor; \
        uniform sampler2D sTexture; \
        in vec4 glFragCoord; \
        in vec2 v_blurTexCoords[14]; \
        void main() { \
            glFragColor = vec4(0.0); \
            glFragColor += texture(sTexture, v_blurTexCoords[ 0])*0.0044299121055113265; \
            glFragColor += texture(sTexture, v_blurTexCoords[ 1])*0.00895781211794; \
            glFragColor += texture(sTexture, v_blurTexCoords[ 2])*0.0215963866053; \
            glFragColor += texture(sTexture, v_blurTexCoords[ 3])*0.0443683338718; \
            glFragColor += texture(sTexture, v_blurTexCoords[ 4])*0.0776744219933; \
            glFragColor += texture(sTexture, v_blurTexCoords[ 5])*0.115876621105; \
            glFragColor += texture(sTexture, v_blurTexCoords[ 6])*0.147308056121; \
            glFragColor += texture(sTexture, glFragCoord.xy       )*0.159576912161; \
            glFragColor += texture(sTexture, v_blurTexCoords[ 7])*0.147308056121; \
            glFragColor += texture(sTexture, v_blurTexCoords[ 8])*0.115876621105; \
            glFragColor += texture(sTexture, v_blurTexCoords[ 9])*0.0776744219933; \
            glFragColor += texture(sTexture, v_blurTexCoords[10])*0.0443683338718; \
            glFragColor += texture(sTexture, v_blurTexCoords[11])*0.0215963866053; \
            glFragColor += texture(sTexture, v_blurTexCoords[12])*0.00895781211794; \
            glFragColor += texture(sTexture, v_blurTexCoords[13])*0.0044299121055113265; \
        }";
        mGaussianBlurHShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexGaussianBlurHShader);
        mGaussianBlurHShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentGaussianBlurShader);
        mGaussianBlurHShader.linkProgram();
        mGaussianBlurHTexture.allocate(mWidth, mHeight, GL_RGBA);

        mFboHGaussian.allocate(mWidth, mHeight, GL_RGBA);
        mFboHGaussian.allocate(mWidth, mHeight, GL_RGBA);
        mFboHGaussian.begin();
        ofClear(255,255);
        mFboHGaussian.end();
        
        mGaussianBlurVShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexGaussianBlurVShader);
        mGaussianBlurVShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentGaussianBlurShader);
        mGaussianBlurVShader.linkProgram();
        mGaussianBlurVTexture.allocate(mWidth, mHeight, GL_RGBA);
        
        mFboVGaussian.allocate(mWidth, mHeight, GL_RGBA);
        mFboVGaussian.allocate(mWidth, mHeight, GL_RGBA);
        mFboVGaussian.begin();
        ofClear(255,255);
        mFboVGaussian.end();
    }
    
    void init(){
        mSimuDepthSensorImp = new SimuDepthSensorImp();
        mDepthSensor = new DepthSensor(mSimuDepthSensorImp);
        
        mWidth = mDepthSensor->getAttrib().width;
        mHeight = mDepthSensor->getAttrib().height;
        
        initCopyShader();
        initSmoothShader();
        initGaussianBlurShader();
        
        mDrawPlane.set(mWidth, mHeight);
        mDrawPlane.setPosition(mWidth, mHeight, 0);
        mDrawPlane.setResolution(2, 2);
        
        mRSDepthSensorImp = new RSSensorImp();
        cout << "use simu depth implementation." << endl;
        
        /*delete [] mFrames;
        mFrames = new ofFbo[20];
        for (int i = 0; i < 20; i++){
            mFrames[i].allocate(mWidth, mHeight, GL_RGBA);
        }*/
        for (int i = 0; i < 10; i++){
            mFrames[i].allocate(mWidth, mHeight, GL_RGBA);
            mFrames[i].clear();
        }
        
        mFrameCtr = 0;
	}
    
    ofTexture smooth(ofTexture *framesArray, ofTexture tex, int tailIdx, int count) {
        ofVec2f win_size = ofVec2f(mWidth, mHeight);
        mFboSmooth.begin();
        {
            mSmoothShader.begin();
            {
                mSmoothShader.setUniform1f("count", (float)count);
                mSmoothShader.setUniformTexture("tex_head", tex, 0);
                mSmoothShader.setUniformTexture("tex_tail", framesArray[tailIdx], 1);
                mSmoothShader.setUniformTexture("tex_smooth", mSmoothTexture, 2);

                mSmoothShader.setUniform2f("resolution", win_size);
                mSmoothShader.setUniform3f("iResolution", win_size.x, win_size.y, 0.0);
                
                mDrawPlane.draw();
            }
            mSmoothShader.end();
        }
        mFboSmooth.end();
        
        return mFboSmooth.getTexture();
    }
    
    ofPlanePrimitive mDrawPlane;
    ofShader         mSmoothShader, mGaussianBlurHShader, mGaussianBlurVShader, mCopyShader;
    ofFbo            mFboSmooth, mFboHGaussian, mFboVGaussian;
    ofTexture        mFrames[10];
    ofTexture        mSmoothTexture, mGaussianBlurHTexture, mGaussianBlurVTexture;
    DepthSensorImp  *mRSDepthSensorImp;
    DepthSensorImp 	*mSimuDepthSensorImp;
    DepthSensor 	*mDepthSensor;
    bool			 mRunning, mBufferFull;
    int              mFrameCtr, mWidth, mHeight;
    
    string vertexCopyShader = "#version 330\nlayout(location = 0) \
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
    string vertexSmoothShader = "#version 330\nlayout(location = 0) \
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
    
    string vertexGaussianBlurHShader = "#version 330\nlayout(location = 0) \
    in vec3 VertexPosition; \
    out vec4 glFragCoord;   \
    out vec2 v_blurTexCoords[14]; \
    void main() { \
        gl_Position = vec4(VertexPosition, 1.0);; \
        glFragCoord.xy = VertexPosition.xy * 0.5 + 0.5;  \
        v_blurTexCoords[ 0] = glFragCoord.xy + vec2(-0.028, 0.0); \
        v_blurTexCoords[ 1] = glFragCoord.xy + vec2(-0.024, 0.0); \
        v_blurTexCoords[ 2] = glFragCoord.xy + vec2(-0.020, 0.0); \
        v_blurTexCoords[ 3] = glFragCoord.xy + vec2(-0.016, 0.0); \
        v_blurTexCoords[ 4] = glFragCoord.xy + vec2(-0.012, 0.0); \
        v_blurTexCoords[ 5] = glFragCoord.xy + vec2(-0.008, 0.0); \
        v_blurTexCoords[ 6] = glFragCoord.xy + vec2(-0.004, 0.0); \
        v_blurTexCoords[ 7] = glFragCoord.xy + vec2( 0.004, 0.0); \
        v_blurTexCoords[ 8] = glFragCoord.xy + vec2( 0.008, 0.0); \
        v_blurTexCoords[ 9] = glFragCoord.xy + vec2( 0.012, 0.0); \
        v_blurTexCoords[10] = glFragCoord.xy + vec2( 0.016, 0.0); \
        v_blurTexCoords[11] = glFragCoord.xy + vec2( 0.020, 0.0); \
        v_blurTexCoords[12] = glFragCoord.xy + vec2( 0.024, 0.0); \
        v_blurTexCoords[13] = glFragCoord.xy + vec2( 0.028, 0.0); \
    }";
    
    string vertexGaussianBlurVShader = "#version 330\nlayout(location = 0) \
    in vec3 VertexPosition; \
    out vec4 glFragCoord;   \
    out vec2 v_blurTexCoords[14]; \
    void main() { \
    gl_Position = vec4(VertexPosition, 1.0);; \
    glFragCoord.xy = VertexPosition.xy * 0.5 + 0.5;  \
    v_blurTexCoords[ 0] = glFragCoord.xy + vec2(0.0, -0.028); \
    v_blurTexCoords[ 1] = glFragCoord.xy + vec2(0.0, -0.024); \
    v_blurTexCoords[ 2] = glFragCoord.xy + vec2(0.0, -0.020); \
    v_blurTexCoords[ 3] = glFragCoord.xy + vec2(0.0, -0.016); \
    v_blurTexCoords[ 4] = glFragCoord.xy + vec2(0.0, -0.012); \
    v_blurTexCoords[ 5] = glFragCoord.xy + vec2(0.0, -0.008); \
    v_blurTexCoords[ 6] = glFragCoord.xy + vec2(0.0, -0.004); \
    v_blurTexCoords[ 7] = glFragCoord.xy + vec2(0.0,  0.004); \
    v_blurTexCoords[ 8] = glFragCoord.xy + vec2(0.0,  0.008); \
    v_blurTexCoords[ 9] = glFragCoord.xy + vec2(0.0,  0.012); \
    v_blurTexCoords[10] = glFragCoord.xy + vec2(0.0,  0.016); \
    v_blurTexCoords[11] = glFragCoord.xy + vec2(0.0,  0.020); \
    v_blurTexCoords[12] = glFragCoord.xy + vec2(0.0,  0.024); \
    v_blurTexCoords[13] = glFragCoord.xy + vec2(0.0,  0.028); \
    }";
};

#endif /* SYNTH_IDE_DEPTH_INPUT_DEPTHSENSORMGR_H_ */

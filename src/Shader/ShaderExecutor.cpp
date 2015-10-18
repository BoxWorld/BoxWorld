//
//  ShaderExecutor.cpp
//  BoxWorld
//
//  Created by Han Maokun on 10/16/15.
//
//
#include "ofMain.h"
#include "ShaderExecutor.h"

ShaderExecutor::ShaderExecutor(int width, int height){
    mGLProgramMain = NULL;
    mWidth = width;
    mHeight = height;
    mInternalFormat = GL_RGBA;
    allocate();
}

void ShaderExecutor::allocate(int _internalFormat){
    if (_internalFormat != -1)
        mInternalFormat = _internalFormat;
    
    mFboPingpong.allocate(mWidth, mHeight, mInternalFormat);
}

void ShaderExecutor::setProgramModel(const S_MainProgram mainProgramModel) {
    mGLProgramMain = new GLSLProgram(mainProgramModel, mWidth, mHeight);
}

void ShaderExecutor::update(){
    mFboPingpong.dst->begin();
    {
        ofClear(0);
        if(mGLProgramMain) {
            mGLProgramMain->render();
        }
    }
    mFboPingpong.dst->end();
    
    mFboPingpong.swap();
}

void ShaderExecutor::draw(){    
    ofPushStyle();
    ofEnableAlphaBlending();
    mFboPingpong.dst->draw(0, 0, mWidth, mHeight);
    ofPopStyle();
}

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
    mGLProgramAudio = NULL;
    mAudGen = NULL;
    mAudioFileReady = false;
    mWidth = width;
    mHeight = height;
    mInternalFormat = GL_RGBA;
    allocate();
}

ShaderExecutor::~ShaderExecutor() {
    if(mGLProgramMain) delete mGLProgramMain;
    if(mAudGen) delete mAudGen;
    mFboPingpong.dispose();
}

void ShaderExecutor::allocate(int _internalFormat){
    if (_internalFormat != -1)
        mInternalFormat = _internalFormat;
    
    mFboPingpong.allocate(mWidth, mHeight, mInternalFormat);
}

void ShaderExecutor::setProgramModel(const S_Program& mainProgramModel, const S_Program& audioProgramModel) {
    if(mGLProgramMain) delete mGLProgramMain;
    if(mainProgramModel.valid)
        mGLProgramMain = new GLSLProgram(mainProgramModel, mWidth, mHeight);
    if(audioProgramModel.valid) {
        mGLProgramAudio = new GLSLProgram(audioProgramModel, mWidth, mHeight);
        mAudGen = new AudioOutGenerator(mWidth, mHeight);
        mAudioFbo.allocate(mWidth,mHeight, mInternalFormat );
        mAudioFbo.begin();
        ofClear(0, 0);
        mAudioFbo.end();
        mAudioPixels.allocate(mWidth, mHeight, OF_PIXELS_RGBA);
    }
}

void ShaderExecutor::update(){
    /* main program rendering to fbo. */
    mFboPingpong.dst->begin();
    {
        ofClear(0);
        if(mGLProgramMain) {
            mGLProgramMain->render();
        }
    }
    mFboPingpong.dst->getTextureReference().getTextureData().bFlipTexture = true;
    mFboPingpong.dst->end();
    mFboPingpong.swap();
    
    /* audio program rendering to generate wav file, only for once. */
    if(mGLProgramAudio && !mAudioFileReady && mAudGen) {
        for(int i=0; i<mAudGen->getNumBlocks(); i++) {
            int offset = i * mAudGen->getTexSamples();
            float block_ofs = offset/mAudGen->getSampleRate();
            /* render to generate texture data. */
            mAudioFbo.begin();
            {
                ofClear(0);
                mGLProgramAudio->setBlockOfs(block_ofs);
                mGLProgramAudio->render();
            }
            mAudioFbo.end();
            mAudioFbo.readToPixels(mAudioPixels);
            mAudGen->writeToBuf(offset, mAudioPixels.getPixels());
        }
        mAudGen->writeWavFile();
        mAudioFileReady = true;
    }
}

void ShaderExecutor::draw(){    
    ofPushStyle();
    ofEnableAlphaBlending();
    mFboPingpong.dst->draw(0, 0, mWidth, mHeight);
    ofPopStyle();
}

//
//  ShaderExecutor.cpp
//  BoxWorld
//
//  Created by Han Maokun on 10/16/15.
//
//
#include "ofMain.h"
#include "ShaderExecutor.h"
#include "ResourceMgrInst.h"

ShaderExecutor::ShaderExecutor(int width, int height){
    mGLProgramMain = NULL;
    mGLProgramAudio = NULL;
    mAudGen = NULL;
    mAudioFileReady = false;
    mAudioFileExist = false;
    mWidth = width;
    mHeight = height;
    mInternalFormat = GL_RGBA;
    allocate();
}

ShaderExecutor::~ShaderExecutor() {
    if(mGLProgramMain)  delete mGLProgramMain;
    if(mGLProgramAudio) delete mGLProgramAudio;
    if(mAudGen)         delete mAudGen;
    mFboPingpong.dispose();
}

void ShaderExecutor::allocate(int _internalFormat){
    if (_internalFormat != -1)
        mInternalFormat = _internalFormat;
    
    mFboPingpong.allocate(mWidth, mHeight, mInternalFormat);
}

bool ShaderExecutor::setProgramModel(const S_Program& mainProgramModel, const S_Program& audioProgramModel) {
    bool suc = false;
    
    /* create new one with feeds. */
    if(mainProgramModel.valid){
        if(mGLProgramMain) delete mGLProgramMain;
        mGLProgramMain = new GLSLProgram(mainProgramModel, mWidth, mHeight);
        suc = true;
    }
    
    if(mGLProgramAudio) {
        delete mGLProgramAudio;
        mGLProgramAudio = NULL;
    }
    if(mAudGen){
        delete mAudGen;
        mAudGen = NULL;
    }
    if(audioProgramModel.valid) {
        mGLProgramAudio = new GLSLProgram(audioProgramModel, mWidth, mHeight);
        mAudGen = new AudioOutGenerator(audioProgramModel.name, mWidth, mHeight);
        mAudioFbo.allocate(mWidth,mHeight, mInternalFormat );
        mAudioFbo.begin();
        ofClear(0, 0);
        mAudioFbo.end();
        mAudioPixels.allocate(mWidth, mHeight, OF_PIXELS_RGBA);
    }
    
    /* find if wav file already exists in cache first. */
    if(mAudGen)
        mAudioFileExist = ResourceMgrInst::get()->isWavFileExists(mAudGen->getRawname());
    
    return suc;
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
    if(!mAudioFileExist && mGLProgramAudio && mAudGen) {
        static bool wasExecuted = false;
        
        if(!wasExecuted) {
            static int blocks_ctr = 0;
            {
                int offset = blocks_ctr * mAudGen->getTexSamples();
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
                blocks_ctr += 1;
            }
            if(blocks_ctr == mAudGen->getNumBlocks()){
                mAudGen->writeWavFile();
                mAudioFileReady = true;
                mAudioFileExist = true;
                wasExecuted = true;
            }
        }
    }
}

void ShaderExecutor::draw(){
    ofPushStyle();
    ofEnableAlphaBlending();
    mFboPingpong.dst->draw(0, 0, mWidth, mHeight);
    ofPopStyle();
}

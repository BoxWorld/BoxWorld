//
//  ShaderExecutor.h
//  BoxWorld
//
//  Created by Han Maokun on 10/16/15.
//
//

#ifndef __BoxWorld__ShaderExecutor__
#define __BoxWorld__ShaderExecutor__

#include <stdio.h>
#include "BoxWorldConfig.h"
#include "ShaderConfig.h"
#include "FboPingPong.h"
#include "GLSLProgram.h"
#include "AudioOutGenerator.h"
#include "ThreadIf.h"

/* https://github.com/danoli3/ofxOSXBoost */
#include <boost/thread/thread.hpp>

class AudioFileWriteThread : public ofThread {
public:
    AudioFileWriteThread(AudioOutGenerator *aud_gen, BoxWorldThreadCallbackIf *callback_inst) {
        mAudGen = aud_gen;
        mCallbackInst = callback_inst;
    }
    ~AudioFileWriteThread() {}
    
    void threadedFunction() {
        while(isThreadRunning()) {
            mAudGen->writeWavFile();
            mCallbackInst->threadCallBack(NULL);
            stopThread();
        }
    }
private:
    AudioOutGenerator           *mAudGen;
    BoxWorldThreadCallbackIf    *mCallbackInst;
};

class ShaderExecutor : public BoxWorldThreadCallbackIf{
public:
    ShaderExecutor(int width, int height);
    virtual ~ShaderExecutor();
    
    bool setProgramModel(const S_Program& mainProgramModel, const S_Program& audioProgramModel);
    void update();
    void draw();
    
    void threadCallBack(void *);
private:
    void allocate(int _internalFormat = -1);
    void renderFrame();

    bool                 mAudioFileExist;
    int                  mWidth, mHeight;
    int                  mInternalFormat;
    ofFbo                mAudioFbo;
    ofPixels             mAudioPixels;
    FboPingPong          mFboPingpong;
    ofSoundPlayer        mSoundPlayer;
    GLSLProgram         *mGLProgramMain, *mGLProgramAudio;
    AudioOutGenerator   *mAudGen;
    AudioFileWriteThread *mAudThread;
};

#endif /* defined(__BoxWorld__ShaderExecutor__) */

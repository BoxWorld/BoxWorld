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
#include "ShaderConfig.h"
#include "FboPingPong.h"
#include "GLSLProgram.h"
#include "AudioOutGenerator.h"

/* https://github.com/danoli3/ofxOSXBoost */
#include <boost/thread/thread.hpp>

class ShaderExecutor {
public:
    ShaderExecutor(int width, int height);
    virtual ~ShaderExecutor();
    
    bool setProgramModel(const S_Program& mainProgramModel, const S_Program& audioProgramModel);
    void update();
    void draw();
private:
    void allocate(int _internalFormat = -1);
    void renderFrame();

    bool            mAudioFileExist;
    bool            mAudioFileReady;
    int             mWidth, mHeight;
    int             mInternalFormat;
    ofFbo           mAudioFbo;
    ofPixels        mAudioPixels;
    FboPingPong     mFboPingpong;
    GLSLProgram     *mGLProgramMain, *mGLProgramAudio;
    AudioOutGenerator *mAudGen;
};

#endif /* defined(__BoxWorld__ShaderExecutor__) */

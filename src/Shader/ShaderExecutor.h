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

class ShaderExecutor {
public:
    ShaderExecutor(int width, int height);
    virtual ~ShaderExecutor() {}
    
    void setProgramModel(const S_MainProgram mainProgramModel);
    void update();
    void draw();
private:
    void allocate(int _internalFormat = -1);
    void renderFrame();

    int             mWidth, mHeight;
    int             mInternalFormat;
    FboPingPong     mFboPingpong;
    GLSLProgram     *mGLProgramMain;
};

#endif /* defined(__BoxWorld__ShaderExecutor__) */

//
//  GLSLProgram.h
//  BoxWorld
//
//  Created by Han Maokun on 10/16/15.
//
//

#ifndef __BoxWorld__GLSLProgram__
#define __BoxWorld__GLSLProgram__

#include <stdio.h>
#include "ofMain.h"
#include "ShaderConfig.h"
#include "UniformIf.h"
#include "ShaderCtrlIf.h"

typedef map<string, UniformIf*> ProgramUniformMap;

class GLSLProgram : public ShaderCtrlIf {
public:
    GLSLProgram(S_MainProgram programModel, int width, int height);
    virtual ~GLSLProgram() {}
    
    void  render();
private:
    float getElapsedTime();
    void  renderFrame();
    void  fillUniformMaps();
    void  applyUniforms();
    
    bool                mLinkFine;
    int                 mWidth, mHeight;
    GLuint              mProgram;
    ofShader            mShader;
    S_MainProgram       mProgramModel;
    ProgramUniformMap   mMainUniformMap;
    ProgramUniformMap   mCustomUniformMap;
};

#endif /* defined(__BoxWorld__GLSLProgram__) */

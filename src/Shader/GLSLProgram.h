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
#include "QuadV.h"

typedef map<string, UniformIf*> ProgramUniformMap;

class GLSLProgram : public ShaderCtrlIf {
public:
    GLSLProgram(S_MainProgram programModel, int width, int height);
    virtual ~GLSLProgram() {}
    
    void  render();
private:
    /* inherited from ShaderCtrlIf. */
    float getElapsedTime();
    int   getTexUnitIdx();
    void  applyTextureOnShader(string name, int textureTarget, int textureID, int textureLocation);
    void  applyTextureOnShader(string name, ofTexture& tex, int texLocation);
    void  applyChannelResolutionOnShader(int idx, int width , int height);
    void  applyIntegerOnShader(int loc, int value);
    void  applyFloatOnShader(int loc, float value);
    void  applyDoubleOnShader(int loc, double value);
    void  applyVec2OnShader(int loc, float f1, float f2);
    void  applyVec3OnShader(int loc, float f1, float f2, float f3);
    void  applyVec4OnShader(int loc, float f1, float f2, float f3, float f4);
    void  applyUintOnShader(int loc, unsigned int value);
    void  applyMat2OnShader(int loc, float* fp);
    void  applyMat3OnShader(int loc, float* fp);
    void  applyMat4OnShader(int loc, float* fp);

    /* methods. */
    void  renderFrame();
    void  fillUniformMaps();
    void  applyUniforms();
    
    bool                mLinkFine;
    bool                mUniformsLoaded;
    int                 mWidth, mHeight;
    GLuint              mProgram;
    ofShader            mShader;
    S_MainProgram       mProgramModel;
    ProgramUniformMap   mMainUniformMap;
    ProgramUniformMap   mCustomUniformMap;
    QuadV               *mQuadDrawable;
};

#endif /* defined(__BoxWorld__GLSLProgram__) */

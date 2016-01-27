//
//  ShaderCtrlIf.h
//  BoxWorld
//
//  Created by Han Maokun on 10/17/15.
//
//

#ifndef BoxWorld_ShaderCtrlIf_h
#define BoxWorld_ShaderCtrlIf_h

class ShaderCtrlIf {
public:
    virtual float getElapsedTime() = 0;
    virtual int   getTexUnitIdx() = 0;
    virtual float getBlockOfs() = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    
    virtual void  applyTextureOnShader(string name, int textureTarget, int textureID, int textureLocation) = 0;
    virtual void  applyTextureOnShader(string name, ofTexture& tex, int texLocation) = 0;
    virtual void  applyChannelResolutionOnShader(int idx, int width , int height) = 0;
    virtual void  applyIntegerOnShader(int loc, int value) = 0;
    virtual void  applyFloatOnShader(int loc, float value) = 0;
    virtual void  applyDoubleOnShader(int loc, double value) = 0;
    virtual void  applyVec2OnShader(int loc, float f1, float f2) = 0;
    virtual void  applyVec3OnShader(int loc, float f1, float f2, float f3) = 0;
    virtual void  applyVec4OnShader(int loc, float f1, float f2, float f3, float f4) = 0;
    virtual void  applyUintOnShader(int loc, unsigned int value) = 0;
    virtual void  applyMat2OnShader(int loc, float* fp) = 0;
    virtual void  applyMat3OnShader(int loc, float* fp) = 0;
    virtual void  applyMat4OnShader(int loc, float* fp) = 0;

protected:
    clock_t         mClockStart;
    int             mTexUnitIdx;
};

#endif

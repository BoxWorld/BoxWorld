//
//  GLSLProgram.cpp
//  BoxWorld
//
//  Created by Han Maokun on 10/16/15.
//
//

#include "GLSLProgram.h"
#include "UniformFactory.h"

GLSLProgram::GLSLProgram(S_MainProgram programModel, int width, int height) {
    mTexUnitIdx = 0;
    mWidth = width;
    mHeight = height;
    mProgramModel = programModel;
    mShader.unload();
    mShader.setupShaderFromSource(GL_VERTEX_SHADER, mProgramModel.code.vertexCode.vert);
    mShader.setupShaderFromSource(GL_FRAGMENT_SHADER, mProgramModel.code.fragmentCode.frag);
    mLinkFine = mShader.linkProgram();
    mProgram = mShader.getProgram();
    fillUniformMaps();
    mUniformsLoaded = false;
    
    mClockStart = clock();
}

float GLSLProgram::getElapsedTime(){
    /* get elapsed time in milliseconds. */
    return (clock() - mClockStart)/1000;
}

void GLSLProgram::render() {
    if(!mLinkFine) return;
    
    mShader.begin();
    {
        if(!mUniformsLoaded) {
            applyUniforms();
            mUniformsLoaded = true;
        }
        renderFrame();
    }
    mShader.end();
}

/*
 * uniforms in shader file.
 * - main uniforms;
 * - custom uniforms;
 */
void GLSLProgram::fillUniformMaps(){
    int maxLen = -1;
    int nUniforms = -1;
    
    glGetProgramiv(mProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
    glGetProgramiv(mProgram, GL_ACTIVE_UNIFORMS, &nUniforms);

    int written = -1;
    int size = -1;
    unsigned int type;
    char nameBuf[maxLen];
    
    for (int i = 0; i < nUniforms; i++) {
        glGetActiveUniform(mProgram, i, maxLen, &written, &size, &type, nameBuf);
        string name(nameBuf, 0, written);
        int location = glGetUniformLocation(mProgram, name.c_str());
        
        UniformIf *main_uniform = UniformFactory::getInst()->CreateUniform(name, UniformFactory::CREATE_BY_NAME);
        if(main_uniform != NULL) {
            /* main uniforms. */
            main_uniform->setName(name);
            main_uniform->setLocation(location);
            main_uniform->setCtroller(this);
            mMainUniformMap.insert(ProgramUniformMap::value_type(name,main_uniform));
        }else {
            /* custom uniforms. */
            list<S_UniformControl>::iterator iter = mProgramModel.code.fragmentCode.fragUniformControlList.begin();
            for(iter; iter!=mProgramModel.code.fragmentCode.fragUniformControlList.end(); iter++) {
                if(iter->name == name) {
                    UniformIf *custom_uniform = UniformFactory::getInst()->CreateUniform(iter->typeString, UniformFactory::CREATE_BY_TYPE);
                    custom_uniform->setName(name);
                    custom_uniform->setLocation(location);
                    custom_uniform->setValueStr(iter->value);
                    if(iter->typeString == "sampler2D") {
                        custom_uniform->setTexParamList(iter->texParamList);
                    }
                    custom_uniform->setCtroller(this);
                    mCustomUniformMap.insert(ProgramUniformMap::value_type(name,custom_uniform));
                }
            }
        }
    }
}

void GLSLProgram::applyUniforms(){
    /* main uniforms. */
    ProgramUniformMap::iterator iter = mMainUniformMap.begin();
    for(iter; iter != mMainUniformMap.end(); iter++) {
        iter->second->applyValue();
    }
    
    /* custom uniforms. */
    iter = mCustomUniformMap.begin();
    for(iter; iter != mCustomUniformMap.end(); iter++) {
        iter->second->applyValue();
    }
}

void GLSLProgram::renderFrame(){
    if (mLinkFine)
        ofSetColor(255,255);
    else
        ofSetColor(150,255);
    
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(mWidth, 0); glVertex3f(mWidth, 0, 0);
        glTexCoord2f(mWidth, mHeight); glVertex3f(mWidth, mHeight, 0);
        glTexCoord2f(0,mHeight);  glVertex3f(0,mHeight, 0);
    }
    glEnd();
}

int GLSLProgram::getTexUnitIdx() {
    return mTexUnitIdx++;
}

void  GLSLProgram::applyTextureOnShader(string name, int textureTarget, int textureID, int textureLocation){
    mShader.setUniformTexture(name, textureTarget, textureID, textureLocation);
}

void GLSLProgram::applyTextureOnShader(string name, ofTexture& tex, int texLocation){
    mShader.setUniformTexture(name, tex, texLocation);
}

void GLSLProgram::applyChannelResolutionOnShader(int idx, int width , int height){
    int location = glGetUniformLocation(mProgram, UNIFORM_ICHANNEL_RESO_PREFIX.c_str());
    if(location == -1) {
        location = glGetUniformLocation(mProgram, UNIFORM_ICHANNEL_RESO_DEFAULT.c_str());
        if(location == -1) return;
    }
    
    float *buff = new float[MAX_CHANNELS * 3];
    
    glGetUniformfv(mProgram, location, buff);
    
    buff[3 * idx] = width;
    buff[3 * idx + 1] = height;
    buff[3 * idx + 2] = 0.0f;
    
    glUniform3fv(location, MAX_CHANNELS, buff);
}

void  GLSLProgram::applyIntegerOnShader(int loc, int value){
    glUniform1i(loc, value);
}

void  GLSLProgram::applyFloatOnShader(int loc, float value){
    glUniform1f(loc, value);
}

void  GLSLProgram::applyVec2OnShader(int loc, float f1, float f2){
    glUniform2f(loc, f1, f2);
}

void  GLSLProgram::applyVec3OnShader(int loc, float f1, float f2, float f3){
    glUniform3f(loc, f1, f2, f3);
}

void  GLSLProgram::applyVec4OnShader(int loc, float f1, float f2, float f3, float f4){
    glUniform4f(loc, f1, f2, f3, f4);
}

void  GLSLProgram::applyUintOnShader(int loc, unsigned int value){
    glUniform1ui(loc, value);
}

void  GLSLProgram::applyDoubleOnShader(int loc, double value){
    glUniform1d(loc, value);
}

void  GLSLProgram::applyMat2OnShader(int loc, float* fp){
    glUniformMatrix2fv(loc, 1, false, fp);
}

void  GLSLProgram::applyMat3OnShader(int loc, float* fp)
{
    glUniformMatrix3fv(loc, 1, false, fp);
}

void  GLSLProgram::applyMat4OnShader(int loc, float* fp)
{
    glUniformMatrix4fv(loc, 1, false, fp);
}

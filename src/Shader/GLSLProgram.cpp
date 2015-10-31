//
//  GLSLProgram.cpp
//  BoxWorld
//
//  Created by Han Maokun on 10/16/15.
//
//

#include "GLSLProgram.h"
#include "UniformFactory.h"

GLSLProgram::GLSLProgram(S_Program programModel, int width, int height) {
    mTexUnitIdx = 0;
    mWidth = width;
    mHeight = height;
    mProgramModel = programModel;
    
    mShader.setupShaderFromSource(GL_VERTEX_SHADER, mProgramModel.code.vertexCode.vert);
    mShader.setupShaderFromSource(GL_FRAGMENT_SHADER, mProgramModel.code.fragmentCode.frag);
    mShader.bindDefaults();
    mLinkFine = mShader.linkProgram();
    mProgram = mShader.getProgram();
    initUniformPresetMap();
    fillUniformMaps();
    
    mClockStart = clock();
    
    mQuadDrawable = new QuadV();
    mQuadDrawable->init();
}

GLSLProgram::~GLSLProgram() {
    if(mQuadDrawable) delete mQuadDrawable;
    
    /* delete main uniforms. */
    ProgramUniformMap::iterator iter = mMainUniformMap.begin();
    for(; iter != mMainUniformMap.end(); iter++) {
        delete iter->second;
    }
    
    /* delete custom uniforms. */
    iter = mCustomUniformMap.begin();
    for(; iter != mCustomUniformMap.end(); iter++) {
        delete iter->second;
    }
}

void GLSLProgram::initUniformPresetMap()
{
    list<S_Preset> preset_list = mProgramModel.code.fragmentCode.presetList;
    if(preset_list.size() == 0) return;
    
    list<S_Preset>::iterator iter = preset_list.begin();
    for(; iter!= preset_list.end(); iter++) {
        mUniformPresetMap.insert(UniformPresetMap::value_type(iter->name, iter->rawTextValue));
    }
}

float GLSLProgram::getElapsedTime(){
    /* get elapsed time in milliseconds. */
    return ((float)(clock() - mClockStart)) * 0.00001;
}

void GLSLProgram::render() {
    if(!mLinkFine) return;
    
    mShader.begin();
    {
        applyUniforms();
        renderFrame();
    }
    mShader.end();
}

string GLSLProgram::getUniformValueByName(string name, string default_val)
{
    if((mUniformPresetMap.size() == 0) ||
       mUniformPresetMap.find(name) == mUniformPresetMap.end()) return default_val;
    
    return mUniformPresetMap.find(name)->second;
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
            for(; iter!=mProgramModel.code.fragmentCode.fragUniformControlList.end(); iter++) {
                if(iter->name == name) {
                    UniformIf *custom_uniform = UniformFactory::getInst()->CreateUniform(iter->typeString, UniformFactory::CREATE_BY_TYPE);
                    custom_uniform->setName(name);
                    custom_uniform->setLocation(location);
                    custom_uniform->setValueStr(getUniformValueByName(name, iter->value));
                    if((iter->typeString == UNIFORM_TYPE_SAMPLER2D) || iter->typeString == UNIFORM_TYPE_SAMPLERCUBE) {
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
    for(; iter != mMainUniformMap.end(); iter++) {
        iter->second->applyValue();
    }
    
    /* custom uniforms. */
    iter = mCustomUniformMap.begin();
    for(; iter != mCustomUniformMap.end(); iter++) {
        iter->second->applyValue();
    }
}

void GLSLProgram::renderFrame(){
    mQuadDrawable->render();
}

int GLSLProgram::getTexUnitIdx() {
    return mTexUnitIdx++;
}

float GLSLProgram::getBlockOfs() {
    return mBlockOfs;
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
    delete buff;
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

void  GLSLProgram::applyMat3OnShader(int loc, float* fp){
    glUniformMatrix3fv(loc, 1, false, fp);
}

void  GLSLProgram::applyMat4OnShader(int loc, float* fp){
    glUniformMatrix4fv(loc, 1, false, fp);
}

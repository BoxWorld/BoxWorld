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
    mWidth = width;
    mHeight = height;
    mProgramModel = programModel;
    mShader.unload();
    mShader.setupShaderFromSource(GL_VERTEX_SHADER, mProgramModel.code.vertexCode.vert);
    mShader.setupShaderFromSource(GL_FRAGMENT_SHADER, mProgramModel.code.fragmentCode.frag);
    mLinkFine = mShader.linkProgram();
    mProgram = mShader.getProgram();
    fillUniformMaps();
    
    mClockStart = clock();
}

float GLSLProgram::getElapsedTime()
{
    /* get elapsed time in milliseconds. */
    return (clock() - mClockStart)/1000;
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

/*
 * uniforms in shader file.
 * - main uniforms;
 * - custom uniforms;
 */
void GLSLProgram::fillUniformMaps()
{
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
            /* main uniform. */
            main_uniform->setLocation(location);
            mMainUniformMap.insert(ProgramUniformMap::value_type(name,main_uniform));
        }else {
            /* custom uniform. */
            list<S_UniformControl>::iterator iter = mProgramModel.code.fragmentCode.fragUniformControlList.begin();
            for(iter; iter!=mProgramModel.code.fragmentCode.fragUniformControlList.end(); iter++) {
                if(iter->name == name) {
                    UniformIf *custom_uniform = UniformFactory::getInst()->CreateUniform(iter->typeString, UniformFactory::CREATE_BY_TYPE);
                    custom_uniform->setLocation(location);
                    custom_uniform->setValueStr(iter->value);
                    mCustomUniformMap.insert(ProgramUniformMap::value_type(name,custom_uniform));
                }
            }
        }
    }
}

void GLSLProgram::applyUniforms()
{
    ProgramUniformMap::iterator iter = mMainUniformMap.begin();
    for(iter; iter != mMainUniformMap.end(); iter++) {
        iter->second->applyValue(this);
    }
}

void GLSLProgram::renderFrame(){
    if (mLinkFine)
        ofSetColor(255,255);
    else
        ofSetColor(150,255);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(mWidth, 0); glVertex3f(mWidth, 0, 0);
    glTexCoord2f(mWidth, mHeight); glVertex3f(mWidth, mHeight, 0);
    glTexCoord2f(0,mHeight);  glVertex3f(0,mHeight, 0);
    glEnd();
}
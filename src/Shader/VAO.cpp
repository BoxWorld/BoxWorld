//
//  VAO.cpp
//  BoxWorld
//
//  Created by Han Maokun on 11/3/15.
//
//

#include <stdio.h>
#include "VAO.h"

#ifdef TARGET_OPENGLES
#include <dlfcn.h>
typedef void (* glGenVertexArraysType) (GLsizei n,  GLuint *arrays);
glGenVertexArraysType glGenVertexArraysFunc = nullptr;
#define glGenVertexArrays glGenVertexArraysFunc

typedef void (* glDeleteVertexArraysType) (GLsizei n,  GLuint *arrays);
glDeleteVertexArraysType glDeleteVertexArraysFunc = nullptr;
#define glDeleteVertexArrays glDeleteVertexArraysFunc

typedef void (* glBindVertexArrayType) (GLuint array);
glBindVertexArrayType glBindVertexArrayFunc = nullptr;
#define glBindVertexArray glBindVertexArrayFunc
#endif

VAO::VAO(int buffer_count) {
    mInited = false;
    mNum = buffer_count;
    mVBOPairArr = new VBOPair[mNum];
    
#ifdef TARGET_OPENGLES
    if(glGenVertexArrays == 0) {
        glGenVertexArrays = (glGenVertexArraysType)dlsym(RTLD_DEFAULT, "glGenVertexArrays");
        glDeleteVertexArrays = (glDeleteVertexArraysType)dlsym(RTLD_DEFAULT, "glDeleteVertexArrays");
        glBindVertexArray = (glBindVertexArrayType)dlsym(RTLD_DEFAULT, "glBindVertexArray");
    }
#endif
}

bool VAO::init() {
    if(mInited) return mInited;
    
    glGenVertexArrays(1, &mId);
    glBindVertexArray(mId);
    
    for (int i = 0; i < mNum; i++) {
        VBOPair vbo_pair = *(mVBOPairArr+i);
        VBO *vbo = vbo_pair.mBuffer;
        
        if (vbo->getTarget() == GL_ELEMENT_ARRAY_BUFFER) {
            vbo->bind(true);
            glBindVertexArray(0);
            vbo->bind(false);
            glBindVertexArray(mId);
            
        } else {
            vbo->bind(true);
            glVertexAttribPointer(i, vbo->getVectorComponents(),
                                  vbo->getDataType()->getGLType(), false, 0, 0);
            glEnableVertexAttribArray(i);
            vbo->bind(false);
        }
    }
    
    glBindVertexArray(0);
    
    mInited = true;
    
    return mInited;
}

void VAO::destroy() {
    for (int i = 0; i < mNum; i++) {
        VBOPair vbo_pair = *(mVBOPairArr+i);
        
        vbo_pair.mBuffer->destroy();
        delete vbo_pair.mBuffer;
    }
    delete mVBOPairArr;
    
    if (mId != 0) {
        glDeleteVertexArrays(1, &mId);
        mId = 0;
    }
    
    mId = 0;
    mInited = false;
}

void VAO::bind(bool bindObj) {
    if (bindObj) {
        glBindVertexArray(mId);
    } else {
        glBindVertexArray(0);
    }
}
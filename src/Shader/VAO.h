//
//  VAO.h
//  BoxWorld
//
//  Created by Han Maokun on 10/20/15.
//
//

#ifndef BoxWorld_VAO_h
#define BoxWorld_VAO_h

#include <list>
#include "VBO.h"

class VBOPair {
public:
    VBOPair() {}
    
    VBOPair(VBO *buf, bool own) {
        mOwn = own;
        mBuffer = buf;
    }
    
    VBO *mBuffer;
    bool mOwn;
};

class VAO {
public:
    VAO(int buffer_count) {
        mInited = false;
        mNum = buffer_count;
        mVBOPairArr = new VBOPair[mNum];
    }
    
    void setBuffer(int index, VBO *vbo) {
        mVBOPairArr[index] = *new VBOPair(vbo, true);
    }
    
    bool init() {
        if(mInited) return;
        
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
    }
    
    bool isInited() { return mInited; }
    
    void destroy() {
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

    void bind(bool bindObj) {
        if (bindObj) {
            glBindVertexArray(mId);
        } else {
            glBindVertexArray(0);
        }
    }
    
private:
    VBOPair      *mVBOPairArr;
    unsigned int mId;
    int          mNum;
    bool         mInited;
};

#endif

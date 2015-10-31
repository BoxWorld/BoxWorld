//
//  VBO.h
//  BoxWorld
//
//  Created by Han Maokun on 10/20/15.
//
//

#ifndef BoxWorld_VBO_h
#define BoxWorld_VBO_h

#include "ofMain.h"
#include "GLDataType.h"

class VBO {
public:
    VBO(int target) {
        mTarget = target;
        mInited = false;
        
        init();
    }
    
    bool init() {
        if(mInited) return true;
        
        glGenBuffers(1, &mId);
        
        mInited = true;
        return mInited;
    }
    
    void bufferData(int elements, int vectorComponents, GLDataType *dataType, void* data, int usage) {
        mVectorComponents = vectorComponents;
        mDataType = dataType;
        
        if (mId == 0) {
            glGenBuffers(1, &mId);
        }
        
        long size = (long) elements * (long) vectorComponents * (long) mDataType->getBytes();
        glBindBuffer(mTarget, mId);
        glBufferData(mTarget, size, data, usage);
        glBindBuffer(mTarget, 0);
        
        mInited = true;
    }
    
    bool isInited() { return mInited; }
    
    void destroy() {
        if (mId != 0) {
            glDeleteBuffers(1, &mId);
        }
        mId = 0;
        mInited = false;
    }
    
    void bind(bool bindBuffer) {
        if(bindBuffer) {
            glBindBuffer(mTarget, mId);
        }else {
            glBindBuffer(mTarget, 0);
        }
    }
    
    int getTarget() { return mTarget; }
    
    int getVectorComponents() {
        return mVectorComponents;
    }
    
    GLDataType *getDataType() {
        return mDataType;
    }
    
private:
    int mTarget;
    bool mInited;
    unsigned int mId;
    int mVectorComponents;
    GLDataType *mDataType;
};

#endif

//
//  QuadV.h
//  BoxWorld
//
//  Created by Han Maokun on 10/20/15.
//
//

#ifndef BoxWorld_QuadV_h
#define BoxWorld_QuadV_h

#include "VAO.h"
#include "GLDataType.h"

static float FLT_EPS = 1e-7f;

class QuadV {
public:
    const static int ATTRIBUTE_POSITION = 0;

    QuadV() {
        mHalfLength = 1.0;
        mVertexArr = new VAO(1);
        mVertexBuf = new VBO(GL_ARRAY_BUFFER);
        mDataType = new GLDataType(4, GL_FLOAT);
    }
    
    QuadV(float halfLength) {
        mHalfLength = halfLength;
        mVertexArr = new VAO(1);
        mVertexBuf = new VBO(GL_ARRAY_BUFFER);
    }
    
    float getHalfLength() { return mHalfLength; }
    
    void setHalfLength(float halfLength) {
        if (mHalfLength - halfLength > FLT_EPS || mHalfLength - halfLength < -FLT_EPS) {
            mHalfLength = halfLength;
            mNeedUpdate = true;
        }
    }
    
    void render() {
        if (mNeedUpdate) {
            createOrUpdateGeometry();
            mNeedUpdate = false;
        }
        mVertexArr->bind(true);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        mVertexArr->bind(false);
    }
    
    bool init() {
        createOrUpdateGeometry();
        
        mVertexArr->setBuffer(ATTRIBUTE_POSITION, mVertexBuf);
        
        if (!mVertexArr->init()) {
            mInited = false;
            return false;
        }
        mInited = true;
        
        return mInited;
    }
    
    void destroy() {
        mVertexArr->destroy();
        mInited = false;
    }
    
    void createOrUpdateGeometry() {
        float minX = -mHalfLength;
        float maxX = mHalfLength;
        float minY = -mHalfLength;
        float maxY = mHalfLength;
        
        /* @formatter:off */
        float verts[18] = {
            minX, minY, 0.0f,
            maxX, minY, 0.0f,
            maxX, maxY, 0.0f,
            minX, minY, 0.0f,
            maxX, maxY, 0.0f,
            minX, maxY, 0.0f,
        };
        /* @formatter:on */
        
        mVertexBuf->bufferData(6, 3, mDataType, (void *)verts, GL_STATIC_DRAW);
    }
    
    bool isInitialized() {
        return mInited;
    }
    
private:
    VBO *mVertexBuf;
    VAO *mVertexArr;
    float mHalfLength;
    bool mInited;
    bool mNeedUpdate;
    GLDataType *mDataType;
};

#endif

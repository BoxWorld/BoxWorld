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
#include "BoxWorldConfig.h"

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
    VAO(int buffer_count);
    
    void setBuffer(int index, VBO *vbo) {
        mVBOPairArr[index] = *new VBOPair(vbo, true);
    }
    
    bool init();
    
    bool isInited() { return mInited; }
    
    void destroy();

    void bind(bool bindObj);
    
private:
    VBOPair      *mVBOPairArr;
    unsigned int mId;
    int          mNum;
    bool         mInited;
};

#endif

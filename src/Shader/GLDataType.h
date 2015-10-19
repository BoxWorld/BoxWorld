//
//  GLDataType.h
//  BoxWorld
//
//  Created by Han Maokun on 10/20/15.
//
//

#ifndef BoxWorld_GLDataType_h
#define BoxWorld_GLDataType_h

class GLDataType {
public:
    int mBytes;
    int mGLType;
    
    GLDataType() {}
    
    GLDataType(int bytes, int glType) {
        mBytes = bytes;
        mGLType = glType;
    }
    
    int getBytes() {
        return mBytes;
    }
    
    int getGLType() {
        return mGLType;
    }
};

#endif

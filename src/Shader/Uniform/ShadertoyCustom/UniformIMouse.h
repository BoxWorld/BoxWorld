//
//  UniformIMouse.h
//  BoxWorld
//
//  Created by Han Maokun on 10/17/15.
//
//

#ifndef BoxWorld_UniformIMouse_h
#define BoxWorld_UniformIMouse_h

#include "UniformIf.h"
#include "ofMain.h"

class UniformIMouse : public UniformIf {
public:
    UniformIMouse() {
        mNeedRefresh = true;
    }
    
    void applyValue() {
        glUniform4f(getLocation(), 0.0, 0.0, 0.0, 0.0);
    }
    
    static UniformIf * __stdcall Create() { return new UniformIMouse(); }
};

#endif

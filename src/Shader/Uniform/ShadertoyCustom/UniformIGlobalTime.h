//
//  UniformIGlobalTime.h
//  BoxWorld
//
//  Created by Han Maokun on 10/17/15.
//
//

#ifndef BoxWorld_UniformIGlobalTime_h
#define BoxWorld_UniformIGlobalTime_h

#include "UniformIf.h"
#include "ofMain.h"

class UniformIGlobalTime : public UniformIf {
public:
    UniformIGlobalTime() {
        mNeedRefresh = true;
    }
    
    void applyValue() {
        glUniform1f(getLocation(), mCtrl->getElapsedTime());
    }
    
    static UniformIf * __stdcall Create() { return new UniformIGlobalTime(); }
};

#endif

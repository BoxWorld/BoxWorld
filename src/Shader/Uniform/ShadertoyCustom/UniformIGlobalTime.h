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
    void applyValue(ShaderCtrlIf *shaderCtrl) {
        glUniform1f(getLocation(), shaderCtrl->getElapsedTime());
    }
    
    static UniformIf * __stdcall Create() { return new UniformIGlobalTime(); }
};

#endif

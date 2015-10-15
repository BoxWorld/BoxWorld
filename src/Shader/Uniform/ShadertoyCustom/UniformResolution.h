//
//  UniformResolution.h
//  BoxWorld
//
//  Created by Han Maokun on 10/17/15.
//
//

#ifndef BoxWorld_UniformResolution_h
#define BoxWorld_UniformResolution_h

#include "UniformIf.h"
#include "ofMain.h"
#include "Config.h"

class UniformResolution : public UniformIf {
public:
    void applyValue(ShaderCtrlIf *shaderCtrl) {
        glUniform3f(getLocation(), BOXWORLD_WIDTH, BOXWORLD_HEIGHT, 1.0);
    }
    
    static UniformIf * __stdcall Create() { return new UniformResolution(); }
};

#endif

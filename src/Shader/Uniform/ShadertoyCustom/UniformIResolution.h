//
//  UniformResolution.h
//  BoxWorld
//
//  Created by Han Maokun on 10/17/15.
//
//

#ifndef BoxWorld_UniformIResolution_h
#define BoxWorld_UniformIResolution_h

#include "UniformIf.h"
#include "ofMain.h"
#include "BoxWorldConfig.h"

class UniformIResolution : public UniformIf {
public:
    void applyValue() {
        glUniform3f(getLocation(), mCtrl->getWidth(), mCtrl->getHeight(), 1.0);
    }
    
    static UniformIf * Create() { return new UniformIResolution(); }
};

#endif

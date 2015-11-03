//
//  UniformResolution.h
//  BoxWorld
//
//  Created by Han Maokun on 10/19/15.
//
//

#ifndef BoxWorld_UniformResolution_h
#define BoxWorld_UniformResolution_h

#include "UniformIf.h"
#include "ofMain.h"
#include "BoxWorldConfig.h"

class UniformResolution : public UniformIf {
public:
    void applyValue() {
        glUniform2f(getLocation(), BOXWORLD_WIDTH, BOXWORLD_HEIGHT);
    }
    
    static UniformIf * Create() { return new UniformResolution(); }
};

#endif

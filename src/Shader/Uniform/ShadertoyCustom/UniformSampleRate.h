//
//  UniformSampleRate.h
//  BoxWorld
//
//  Created by Han Maokun on 10/17/15.
//
//

#ifndef BoxWorld_UniformSampleRate_h
#define BoxWorld_UniformSampleRate_h

#include "UniformIf.h"
#include "ofMain.h"

class UniformSampleRate : public UniformIf {
public:
    void applyValue() {
        glUniform1f(getLocation(), 0.0);
    }
    
    static UniformIf * __stdcall Create() { return new UniformSampleRate(); }
};

#endif

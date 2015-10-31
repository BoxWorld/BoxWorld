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
#include "AudioOutGenerator.h"

class UniformSampleRate : public UniformIf {
public:
    void applyValue() {
        glUniform1f(getLocation(), DEFAULT_SAMPLE_RATE);
    }
    
    static UniformIf * __stdcall Create() { return new UniformSampleRate(); }
};

#endif

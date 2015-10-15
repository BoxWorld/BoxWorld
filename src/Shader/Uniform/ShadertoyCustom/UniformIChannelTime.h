//
//  UniformIChannelTime.h
//  BoxWorld
//
//  Created by Han Maokun on 10/17/15.
//
//

#ifndef BoxWorld_UniformIChannelTime_h
#define BoxWorld_UniformIChannelTime_h

#include "UniformIf.h"
#include "ofMain.h"

class UniformIChannelTime : public UniformIf {
public:
    void applyValue(ShaderCtrlIf *shaderCtrl) {
        float time[4];
        for(int i=0; i<4; i++) {
            time[i] = shaderCtrl->getElapsedTime();
        }
        glUniform1fv(getLocation(), 4, time);
    }
    
    static UniformIf * __stdcall Create() { return new UniformIChannelTime(); }
};


#endif

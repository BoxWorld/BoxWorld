//
//  UniformIBlockOffset.h
//  BoxWorld
//
//  Created by Han Maokun on 10/27/15.
//
//

#ifndef BoxWorld_UniformIBlockOffset_h
#define BoxWorld_UniformIBlockOffset_h

#include "UniformIf.h"
#include "ofMain.h"

class UniformIBlockOffset : public UniformIf {
public:
    void applyValue() {
        glUniform1f(getLocation(), mCtrl->getBlockOfs());
    }
    
    static UniformIf * __stdcall Create() { return new UniformIBlockOffset(); }
};

#endif

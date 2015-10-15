//
//  UniformTypeSampler2D.h
//  BoxWorld
//
//  Created by Han Maokun on 10/17/15.
//
//

#ifndef BoxWorld_UniformTypeSampler2D_h
#define BoxWorld_UniformTypeSampler2D_h

#include "UniformIf.h"
#include "ofMain.h"

class UniformTypeSampler2D : public UniformIf {
public:
    void applyValue(ShaderCtrlIf *shaderCtrl) {
        std::string::size_type sz;     // alias of size_t
        float value = std::stof (mValue,&sz);
        
        glUniform1f(getLocation(), value);
    }
    
    static UniformIf * __stdcall Create() { return new UniformTypeSampler2D(); }
};


#endif

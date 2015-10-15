//
//  UniformTypeFloat.h
//  BoxWorld
//
//  Created by Han Maokun on 10/17/15.
//
//

#ifndef BoxWorld_UniformTypeFloat_h
#define BoxWorld_UniformTypeFloat_h

#include "UniformIf.h"
#include "ofMain.h"

class UniformTypeFloat : public UniformIf {
public:
    void applyValue(ShaderCtrlIf *shaderCtrl) {
        std::string::size_type sz;     // alias of size_t
        float value = std::stof (mValue,&sz);

        glUniform1f(getLocation(), value);
    }
    
    static UniformIf * __stdcall Create() { return new UniformTypeFloat(); }
};

#endif

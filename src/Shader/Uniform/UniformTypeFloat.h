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

class UniformTypeFloat : public UniformIf {
public:
    void applyValue() {
        float value = std::atof (mValue.c_str());

        mCtrl->applyFloatOnShader(getLocation(), value);
    }
    
    static UniformIf * __stdcall Create() { return new UniformTypeFloat(); }
};

#endif

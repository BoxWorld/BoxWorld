//
//  UniformTypeBool.h
//  BoxWorld
//
//  Created by Han Maokun on 10/18/15.
//
//

#ifndef BoxWorld_UniformTypeBool_h
#define BoxWorld_UniformTypeBool_h

#include "UniformIf.h"

class UniformTypeBool : public UniformIf {
public:
    void applyValue() {
        mCtrl->applyIntegerOnShader(getLocation(), (mValue == "true")? 1 : 0);
    }
    
    static UniformIf * __stdcall Create() { return new UniformTypeBool(); }
};

#endif

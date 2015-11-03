//
//  UniformTypeDouble.h
//  BoxWorld
//
//  Created by Han Maokun on 10/18/15.
//
//

#ifndef BoxWorld_UniformTypeDouble_h
#define BoxWorld_UniformTypeDouble_h

#include "UniformIf.h"

class UniformTypeDouble : public UniformIf {
public:
    void applyValue() {
        double value = std::atod(mValue.c_str());
        
        mCtrl->applyDoubleOnShader(getLocation(), value);
    }
    
    static UniformIf * Create() { return new UniformTypeDouble(); }
};

#endif

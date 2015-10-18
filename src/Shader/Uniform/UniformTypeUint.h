//
//  UniformTypeUint.h
//  BoxWorld
//
//  Created by Han Maokun on 10/18/15.
//
//

#ifndef BoxWorld_UniformTypeUint_h
#define BoxWorld_UniformTypeUint_h

class UniformTypeUint : public UniformIf {
public:
    void applyValue() {
        unsigned int value = std::atoi(mValue.c_str());

        mCtrl->applyUintOnShader(getLocation(), value);
    }
    
    static UniformIf * __stdcall Create() { return new UniformTypeUint(); }
};

#endif

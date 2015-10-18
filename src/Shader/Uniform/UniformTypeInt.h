//
//  UniformTypeInt.h
//  BoxWorld
//
//  Created by Han Maokun on 10/18/15.
//
//

#ifndef BoxWorld_UniformTypeInt_h
#define BoxWorld_UniformTypeInt_h

class UniformTypeInt : public UniformIf {
public:
    void applyValue() {
        int value = std::atoi(mValue.c_str());
        
        mCtrl->applyIntegerOnShader(getLocation(), value);
    }
    
    static UniformIf * __stdcall Create() { return new UniformTypeInt(); }
};


#endif

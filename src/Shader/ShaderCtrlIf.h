//
//  ShaderCtrlIf.h
//  BoxWorld
//
//  Created by Han Maokun on 10/17/15.
//
//

#ifndef BoxWorld_ShaderCtrlIf_h
#define BoxWorld_ShaderCtrlIf_h

class ShaderCtrlIf {
public:
    virtual float getElapsedTime() = 0;
    
protected:
    clock_t         mClockStart;
};

#endif

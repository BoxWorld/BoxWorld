//
//  UniformIf.h
//  BoxWorld
//
//  Created by Han Maokun on 10/17/15.
//
//

#ifndef BoxWorld_UniformIf_h
#define BoxWorld_UniformIf_h

#include <string.h>
#include "ShaderCtrlIf.h"

static string UNIFORM_IGLOBAL_TIME = "iGlobalTime";
static string UNIFORM_ICHANNEL_TIME = "iChannelTime[0]";
static string UNIFORM_IRESOLUTION = "iResolution";
static string UNIFORM_IMOUSE = "iMouse";
static string UNIFORM_DATE = "iDate";
static string UNIFORM_SAMPLE_RATE = "iSampleRate";

class UniformIf {
public:
    virtual string getName() { return mName; }
    virtual void setName(string name) { mName = name; }
    
    virtual int getLocation() { return mLocation; }
    virtual void setLocation(int location) { mLocation = location; }
    
    virtual void setValueStr(string value) { mValue = value; }
    virtual void applyValue(ShaderCtrlIf *shaderCtrl) = 0;
    
protected:
    string mName;
    string mValue;
    int mLocation;
};

typedef UniformIf* (__stdcall *CreateUniformFn)(void);

#endif

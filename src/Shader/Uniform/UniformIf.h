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
#include "ShaderConfig.h"

#define MAX_CHANNELS 4

static string UNIFORM_ICHANNEL_PREFIX = "iChannel";
static string UNIFORM_ICHANNEL_RESO_PREFIX = "iChannelResolution";
static string UNIFORM_ICHANNEL_RESO_DEFAULT = "iChannelResolution[0]";

static string UNIFORM_IGLOBAL_TIME = "iGlobalTime";
static string UNIFORM_ICHANNEL_TIME = "iChannelTime[0]";
static string UNIFORM_IRESOLUTION = "iResolution";
static string UNIFORM_IMOUSE = "iMouse";
static string UNIFORM_DATE = "iDate";
static string UNIFORM_SAMPLE_RATE = "iSampleRate";

static string UNIFORM_TYPE_FLOAT = "float";
static string UNIFORM_TYPE_DOUBLE = "double";
static string UNIFORM_TYPE_SAMPLER2D = "sampler2D";
static string UNIFORM_TYPE_SAMPLERCUBE = "samplerCube";
static string UNIFORM_TYPE_BOOL = "bool";
static string UNIFORM_TYPE_INT  = "int";
static string UNIFORM_TYPE_UINT = "uint";
static string UNIFORM_TYPE_VEC2 = "vec2";
static string UNIFORM_TYPE_VEC3 = "vec3";
static string UNIFORM_TYPE_VEC4 = "vec4";
static string UNIFORM_TYPE_MAT2 = "mat2";
static string UNIFORM_TYPE_MAT3 = "mat3";
static string UNIFORM_TYPE_MAT4 = "mat4";

class UniformIf {
public:
    virtual string getName() { return mName; }
    virtual void setName(string name) { mName = name; }
    
    virtual int getLocation() { return mLocation; }
    virtual void setLocation(int location) { mLocation = location; }
    
    virtual void setValueStr(string value) { mValue = value; }
    virtual void applyValue() = 0;
    
    virtual void setCtroller(ShaderCtrlIf *ctrl) { mCtrl = ctrl; }
    virtual void setTexParamList(list<S_TextureParam> texParamList) { mTexParamList = texParamList; };
    
protected:
    string                  mName;
    string                  mValue;
    int                     mLocation;
    ShaderCtrlIf            *mCtrl;
    list<S_TextureParam>    mTexParamList;
};

typedef UniformIf* (__stdcall *CreateUniformFn)(void);

#endif

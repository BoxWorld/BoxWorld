//
//  UniformFactory.h
//  BoxWorld
//
//  Created by Han Maokun on 10/17/15.
//
//

#ifndef BoxWorld_UniformFactory_h
#define BoxWorld_UniformFactory_h

#include "UniformIf.h"
#include "UniformIGlobalTime.h"
#include "UniformIChannelTime.h"
#include "UniformResolution.h"
#include "UniformIResolution.h"
#include "UniformIMouse.h"
#include "UniformDate.h"
#include "UniformSampleRate.h"
#include "UniformTypeFloat.h"
#include "UniformTypeSampler2D.h"
#include "UniformTypeSamplerCube.h"
#include "UniformTypeBool.h"
#include "UniformTypeVec2.h"
#include "UniformTypeVec3.h"
#include "UniformTypeVec4.h"
#include "UniformTypeUint.h"
#include "UniformTypeInt.h"
#include "UniformTypeMat2.h"
#include "UniformTypeMat3.h"
#include "UniformTypeMat4.h"

// Factory for creating instances of UniformIf
class UniformFactory
{
public:
    typedef enum {
        CREATE_BY_NAME,
        CREATE_BY_TYPE
    }CREATE_STRATEGY;
    
    ~UniformFactory() {
        mFactoryMapName.clear();
        mFactoryMapType.clear();
    }
    
    static UniformFactory *getInst()
    {
        static UniformFactory instance;
        return &instance;
    }
    
    UniformIf *CreateUniform(const string &uniformNameOrType, CREATE_STRATEGY createStrategy) {
        if(createStrategy == CREATE_BY_NAME) {
            UniformFactoryMap::iterator it = mFactoryMapName.find(uniformNameOrType);
            if( it != mFactoryMapName.end() )
                return it->second();
            return NULL;
        }else if(createStrategy == CREATE_BY_TYPE) {
            UniformFactoryMap::iterator it = mFactoryMapType.find(uniformNameOrType);
            if( it != mFactoryMapType.end() )
                return it->second();
            return NULL;
        }
    }
    
private:
    UniformFactory(){
        /* Shadertoy default uniforms. */
        Register(UNIFORM_IGLOBAL_TIME, &UniformIGlobalTime::Create, CREATE_BY_NAME);
        Register(UNIFORM_ICHANNEL_TIME, &UniformIChannelTime::Create, CREATE_BY_NAME);
        Register(UNIFORM_IRESOLUTION, &UniformIResolution::Create, CREATE_BY_NAME);
        Register(UNIFORM_IMOUSE, &UniformIMouse::Create, CREATE_BY_NAME);
        Register(UNIFORM_DATE, &UniformDate::Create, CREATE_BY_NAME);
        Register(UNIFORM_SAMPLE_RATE, &UniformSampleRate::Create, CREATE_BY_NAME);
        
        Register(UNIFORM_SANDBOX_RESOLUTION, &UniformResolution::Create, CREATE_BY_NAME);
        
        /* Custom uniforms. */
        Register(UNIFORM_TYPE_FLOAT, &UniformTypeFloat::Create, CREATE_BY_TYPE);
        Register(UNIFORM_TYPE_SAMPLER2D, &UniformTypeSampler2D::Create, CREATE_BY_TYPE);
        Register(UNIFORM_TYPE_SAMPLERCUBE, &UniformTypeSamplerCube::Create, CREATE_BY_TYPE);
        Register(UNIFORM_TYPE_BOOL, &UniformTypeBool::Create, CREATE_BY_TYPE);
        Register(UNIFORM_TYPE_VEC2, &UniformTypeVec2::Create, CREATE_BY_TYPE);
        Register(UNIFORM_TYPE_VEC3, &UniformTypeVec3::Create, CREATE_BY_TYPE);
        Register(UNIFORM_TYPE_VEC4, &UniformTypeVec4::Create, CREATE_BY_TYPE);
        Register(UNIFORM_TYPE_INT, &UniformTypeInt::Create, CREATE_BY_TYPE);
        Register(UNIFORM_TYPE_UINT, &UniformTypeUint::Create, CREATE_BY_TYPE);
        Register(UNIFORM_TYPE_DOUBLE, &UniformTypeUint::Create, CREATE_BY_TYPE);
        Register(UNIFORM_TYPE_MAT2, &UniformTypeMat2::Create, CREATE_BY_TYPE);
        Register(UNIFORM_TYPE_MAT3, &UniformTypeMat3::Create, CREATE_BY_TYPE);
        Register(UNIFORM_TYPE_MAT4, &UniformTypeMat4::Create, CREATE_BY_TYPE);
    }
    
    UniformFactory(const UniformFactory &) { }
    UniformFactory &operator=(const UniformFactory &) { return *this; }
    
    void Register(const string &uniformNameOrType, CreateUniformFn pfnCreate, CREATE_STRATEGY createStrategy){
        if(createStrategy == CREATE_BY_NAME) {
            mFactoryMapName[uniformNameOrType] = pfnCreate;
        }else if(createStrategy == CREATE_BY_TYPE) {
            mFactoryMapType[uniformNameOrType] = pfnCreate;
        }
    }
    
    typedef map<string, CreateUniformFn> UniformFactoryMap;
    UniformFactoryMap mFactoryMapName;
    UniformFactoryMap mFactoryMapType;
};

#endif

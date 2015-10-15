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
#include "UniformIMouse.h"
#include "UniformDate.h"
#include "UniformSampleRate.h"

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
        Register(UNIFORM_IGLOBAL_TIME, &UniformIGlobalTime::Create, CREATE_BY_NAME);
        Register(UNIFORM_ICHANNEL_TIME, &UniformIChannelTime::Create, CREATE_BY_NAME);
        Register(UNIFORM_IRESOLUTION, &UniformResolution::Create, CREATE_BY_NAME);
        Register(UNIFORM_IMOUSE, &UniformIMouse::Create, CREATE_BY_NAME);
        Register(UNIFORM_DATE, &UniformDate::Create, CREATE_BY_NAME);
        Register(UNIFORM_SAMPLE_RATE, &UniformSampleRate::Create, CREATE_BY_NAME);
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

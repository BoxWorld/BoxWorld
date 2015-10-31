//
//  UniformTypeSamplerCube.h
//  BoxWorld
//
//  Created by Han Maokun on 10/19/15.
//
//

#ifndef BoxWorld_UniformTypeSamplerCube_h
#define BoxWorld_UniformTypeSamplerCube_h

#include "ofxCubeMap.h"

#define SHADERTOY_CUBEMAP_DELIM '_'

class UniformTypeSamplerCube : public UniformIf {
public:
    void setCtroller(ShaderCtrlIf *ctrl) {
        mCtrl = ctrl;
        mTexUnitIdx = mCtrl->getTexUnitIdx();
        
        /* generate texture. */
        std::string img_name_arr[6];
        std::stringstream value_s(mValue);
        std::string base_name_str;
        std::getline(value_s, base_name_str, SHADERTOY_CUBEMAP_DELIM);
        std::string suffix_str;
        std::getline(value_s, suffix_str, SHADERTOY_CUBEMAP_DELIM);
        for(int i=0; i<6; i++) {
            stringstream convert;
            convert << i;
            string idx_str = convert.str();
            std::string per_suffix_str = idx_str.append(suffix_str.substr(1,suffix_str.length()-1));
            img_name_arr[i] = base_name_str.append("_").append(per_suffix_str);
        }
        mCubemap.loadImages(img_name_arr[0],
                            img_name_arr[1],
                            img_name_arr[2],
                            img_name_arr[3],
                            img_name_arr[4],
                            img_name_arr[5]
        );
    }
    
    void applyValue() {
        /* set up texture parameters. */
        if(mTexParamList.size() > 0) {
            GLint wrap_s = GL_CLAMP_TO_EDGE;
            GLint wrap_t = GL_CLAMP_TO_EDGE;
            GLint mag_filter = GL_LINEAR;
            GLint min_filter = GL_LINEAR;
            list<S_TextureParam>::iterator iter = mTexParamList.begin();
            for(; iter != mTexParamList.end(); iter++) {
                switch(iter->key){
                    case GL_TEXTURE_WRAP_S:
                        wrap_s = iter->value;
                        break;
                    case GL_TEXTURE_WRAP_T:
                        wrap_t = iter->value;
                        break;
                    case GL_TEXTURE_MIN_FILTER:
                        min_filter = iter->value;
                        break;
                    case GL_TEXTURE_MAG_FILTER:
                        mag_filter = iter->value;
                        break;
                    default:
                        break;
                }
            }
            /* bind */
            glBindTexture(GL_TEXTURE_CUBE_MAP, mCubemap.getTextureID());
            
            /* set */
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap_s);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap_t);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, mag_filter);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, min_filter);
            
            /* unbind */
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        }
        
        /* apply texture uniform. */
        mCtrl->applyTextureOnShader(mName, GL_TEXTURE_CUBE_MAP, mCubemap.getTextureID(), mTexUnitIdx);
        
        /* if 'iChannelX' is used, set the default 'iChannelResolution[X]' uniform. */
        if (mName.find(UNIFORM_ICHANNEL_PREFIX) == 0){
            int idx_in_str = string(UNIFORM_ICHANNEL_PREFIX).length();
            string channelIdxStr = mName.substr(idx_in_str);
            int channelIdx = 0;
            channelIdx = std::atoi(channelIdxStr.c_str());
            if(channelIdx > MAX_CHANNELS) return;
            
            /* set channelResolution uniform. */
            mCtrl->applyChannelResolutionOnShader(channelIdx, mCubemap.getWidth(), mCubemap.getHeight());
        }
    }
    
    static UniformIf * __stdcall Create() { return new UniformTypeSampler2D(); }
    
private:
    int                     mTexUnitIdx;
    ofxCubeMap              mCubemap;
};

#endif

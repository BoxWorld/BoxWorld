//
//  UniformTypeSampler2D.h
//  BoxWorld
//
//  Created by Han Maokun on 10/17/15.
//
//

#ifndef BoxWorld_UniformTypeSampler2D_h
#define BoxWorld_UniformTypeSampler2D_h

#include "UniformIf.h"
#include "ofMain.h"

class UniformTypeSampler2D : public UniformIf {
public:
    void setCtroller(ShaderCtrlIf *ctrl) {
        mCtrl = ctrl;
        mTexUnitIdx = mCtrl->getTexUnitIdx();
        
        /* generate texture. */
        ofImage img;
        bool suc = img.loadImage(ofToDataPath(mValue));
        if(!suc) {
            ofLog(OF_LOG_ERROR, "Unable to load texture resource image.");
        }
        mTex = img.getTextureReference();
        ofTextureData tex_data = mTex.getTextureData();
        tex_data.textureTarget = GL_TEXTURE_2D;
        setupTexParameters();
    }

    void applyValue() {
        /* set up texture parameters. */
        if(mTexParamList.size() > 0) {
            GLint wrap_s = GL_CLAMP_TO_EDGE;
            GLint wrap_t = GL_CLAMP_TO_EDGE;
            GLint mag_filter = GL_LINEAR;
            GLint min_filter = GL_LINEAR;
            list<S_TextureParam>::iterator iter = mTexParamList.begin();
            for(iter; iter != mTexParamList.end(); iter++) {
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
            mTex.setTextureWrap(wrap_s, wrap_t);
            mTex.setTextureMinMagFilter(min_filter, mag_filter);
        }
        
        /* set texture uniform. */
        mCtrl->applyTextureOnShader(mName, mTex, mTexUnitIdx);
        
        /* if 'iChannelX' is used, set the default 'iChannelResolution[X]' uniform. */
        if (mName.find(UNIFORM_ICHANNEL_PREFIX) == 0){
            int idx_in_str = string(UNIFORM_ICHANNEL_PREFIX).length();
            string channelIdxStr = mName.substr(idx_in_str);
            int channelIdx = 0;
            channelIdx = std::atoi(channelIdxStr.c_str());
            if(channelIdx > MAX_CHANNELS) return;
            
            /* set channelResolution uniform. */
            mCtrl->applyChannelResolutionOnShader(channelIdx, mTex.getWidth(), mTex.getHeight());
        }
    }
    
    static UniformIf * __stdcall Create() { return new UniformTypeSampler2D(); }
    
private:
    void setupTexParameters() {
        glBindTexture(GL_TEXTURE_2D, mTex.getTextureData().textureID);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    int                     mTexUnitIdx;
    ofTexture               mTex;
};


#endif

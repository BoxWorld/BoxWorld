//
//  DepthTransMgr.h
//  BoxWorld
//
//  Created by Han Maokun on 4/13/16.
//
//

#ifndef DepthTransMgr_h
#define DepthTransMgr_h

#include "ofMain.h"
#include "BoxWorldWindowAttrib.h"

#define RESOLUTION_WIDTH  640
#define RESOLUTION_HEIGHT 480

class  DepthTransMgr {
public:
    static DepthTransMgr* get() {
        static DepthTransMgr *inst = new DepthTransMgr();
        return inst;
    }
    
    DepthTransMgr(){
        string transVert = "#version 330\nlayout(location = 0) \
        in vec3 VertexPosition; \
        uniform mat4 TileMatrix = mat4(1.0); \
        uniform vec2 resolution; \
        out vec4 glFragCoord; \
        void main() { \
            vec2 v = VertexPosition.xy; \
            glFragCoord.xy = v * resolution; \
            gl_Position = vec4(VertexPosition, 1.0); \
        }";
        
        string transFrag = "#version 330\nlayout(location = 0) \
        out vec4 glFragColor; \
        in vec4 glFragCoord; \
        uniform vec3 iResolution; \
        uniform float minD; \
        uniform float maxD; \
        uniform int minH; \
        uniform int maxH; \
        uniform float k; \
        uniform float b; \
        uniform float ka; \
        uniform float kb; \
        uniform float kc; \
        uniform sampler2D tex; \
        void mainImage(out vec4 fragColor,in vec2 fragCoord) { \
            vec2 uv = fragCoord.xy / iResolution.xy; \
            \
            float a = uv.x * 337.5 * ka; \
            float b = (uv.y+1) * 210.0 * kb; \
            float c = 535.0 * kc; \
            \
            uv = uv * 0.5 + 0.5; \
            float d0 = 1.0 - texture(tex, uv).r; \
            float d = d0 * (maxD-minD) + minD; \
            float height = abs(sqrt(d*d - a*a - b*b)); \
            height = d*d - a*a - b*b; \
            height = smoothstep(minH, maxH, height); \
            vec4 col = vec4(height, height, height, 1.0); \
            fragColor = col; \
        } \
        void main() { \
            vec4 color; \
            mainImage(color, glFragCoord.xy); \
            glFragColor = color; \
        }";
        
        mTransShader.unload();
        mTransShader.setupShaderFromSource(GL_VERTEX_SHADER, transVert);
        mTransShader.setupShaderFromSource(GL_FRAGMENT_SHADER, transFrag);
        mTransShader.linkProgram();
        
        mTransDrawPlane.set(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
        mTransDrawPlane.setPosition(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0);
        mTransDrawPlane.setResolution(2, 2);
        
        mTransDepthFbo.allocate(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, GL_RGBA);
    }
    virtual ~DepthTransMgr(){}
    
    ofTexture & getTransTexture(ofTexture &tex, float min_d, float max_d){
        mTransDepthFbo.begin();
        {
            mTransShader.begin();
            mTransShader.setUniformTexture("tex", tex, 0);
            mTransShader.setUniform2f("resolution", ofVec2f(RESOLUTION_WIDTH, RESOLUTION_HEIGHT));
            mTransShader.setUniform3f("iResolution", RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0.0);
            mTransShader.setUniform1f("minD", min_d);
            mTransShader.setUniform1f("maxD", max_d);
            mTransShader.setUniform1i("minH", BoxWorldWindowAttrib::getInst().minHeight);
            mTransShader.setUniform1i("maxH", BoxWorldWindowAttrib::getInst().maxHeight);
            mTransShader.setUniform1f("k", 0.6);
            mTransShader.setUniform1f("b", 0.2);
            mTransShader.setUniform1f("ka", BoxWorldWindowAttrib::getInst().ka);
            mTransShader.setUniform1f("kb", BoxWorldWindowAttrib::getInst().kb);
            mTransShader.setUniform1f("kc", BoxWorldWindowAttrib::getInst().kc);

            mTransDrawPlane.draw();
            mTransShader.end();
        }
        mTransDepthFbo.end();
        
        return mTransDepthFbo.getTexture();
    }
    
private:
    ofFbo                    mTransDepthFbo;
    ofPlanePrimitive         mTransDrawPlane;
    ofShader                 mTransShader;
};

#endif /* DepthTransMgr_h */

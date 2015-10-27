//
//  ShaderConfig.h
//  BoxWorld
//
//  Created by Han Maokun on 10/16/15.
//
//

#ifndef BoxWorld_ShaderConfig_h
#define BoxWorld_ShaderConfig_h

#include <string>
#include <list>

using namespace std;

const static char* BOXWORLD_SHADER_KEY = "boxworldShader";
const static char* PROGRAM_MAIN_KEY = "mainProgram";
const static char* PROGRAM_AUDIO_KEY = "audioProgram";
const static char* PROGRAM_POSTPROCESS_KEY = "postProcessProgram";
const static char* PROGRAM_SUB_NAME_KEY = "name";
const static char* PROGRAM_SUB_CODE_KEY = "code";
const static char* CODE_SUB_FRAG_KEY = "fragmentCode";
const static char* FRAG_SUB_CODE_KEY = "frag";
const static char* FRAG_SUB_UCLIST_KEY = "fragUniformControlList";
const static char* FRAG_SUB_PRESETLIST_KEY = "presetList";
const static char* PRESET_SUB_NAME_KEY = "name";
const static char* PRESET_SUB_VALUE_KEY = "rawTextValue";
const static char* CODE_SUB_VERT_KEY = "vertexCode";
const static char* VERT_SUB_CODE_KEY = "vert";
const static char* VERT_SUB_UCLIST_KEY = "vertUniformControlList";
const static char* UC_SUB_NAME_KEY = "name";
const static char* UC_SUB_TYPE_KEY = "typeString";
const static char* UC_SUB_VALUE_KEY = "value";
const static char* UC_SUB_TEXPARAMLIST_KEY = "texParamList";
const static char* TEXPARAM_SUB_KEY_KEY = "key";
const static char* TEXPARAM_SUB_VALUE_KEY = "value";

typedef struct {
    int key;
    int value;
}S_TextureParam;

typedef struct {
    string name;
    string typeString;
    string value;
    list<S_TextureParam> texParamList;
}S_UniformControl;

typedef struct {
    string name;
    string rawTextValue;
}S_Preset;

typedef struct {
    string frag;
    list<S_Preset> presetList;
    list<S_UniformControl> fragUniformControlList;
}S_FragmentCode;

typedef struct {
    string vert;
}S_VertexCode;

typedef struct{
    S_VertexCode vertexCode;
    S_FragmentCode fragmentCode;
}S_Code;

typedef struct {
    string name;
    S_Code code;
    bool valid;
}S_Program;

#endif

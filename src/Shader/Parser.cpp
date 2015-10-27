//
//  Parse.cpp
//  BoxWorld
//
//  Created by Han Maokun on 10/16/15.
//
//
#include "Parser.h"

S_Program Parser::decodeToProgram(const Json::Value& val) {
    S_Program program;
    program.valid = false;
    
    if (val.empty()) { mError = NO_MAINPROGRAM; return program; }
    
    string program_name = val[PROGRAM_SUB_NAME_KEY].asString();
    const Json::Value program_code_value = val[PROGRAM_SUB_CODE_KEY];
    if (program_code_value.empty()) { mError = NO_CODE; return program; }
    
    const Json::Value vertex_sec_value = program_code_value[CODE_SUB_VERT_KEY];
    if (vertex_sec_value.empty()) { mError = NO_VERT_CODE; return program; }
    {
        /* get vertex code. */
        program.code.vertexCode.vert = vertex_sec_value[VERT_SUB_CODE_KEY].asString();
    }
    
    const Json::Value fragment_sec_value = program_code_value[CODE_SUB_FRAG_KEY];
    if (fragment_sec_value.empty()) { mError = NO_FRAG_CODE; return program; }
    {
        /* get fragment code. */
        program.code.fragmentCode.frag = fragment_sec_value[FRAG_SUB_CODE_KEY].asString();
        
        /* get fragment preset list. */
        list<S_Preset> fragment_preset_list;
        const Json::Value arr_preset_value = fragment_sec_value[FRAG_SUB_PRESETLIST_KEY];
        for(int i=0; i<arr_preset_value.size(); i++) {
            S_Preset preset;
            preset.name = arr_preset_value[i][PRESET_SUB_NAME_KEY].asString();
            preset.rawTextValue = arr_preset_value[i][PRESET_SUB_VALUE_KEY].asString();
            fragment_preset_list.insert(fragment_preset_list.end(), preset);
        }
        if(fragment_preset_list.size()>0) {
            program.code.fragmentCode.presetList = fragment_preset_list;
        }
        
        /* get fragment uniform control list. */
        list<S_UniformControl> fragment_uc_list;
        const Json::Value arr_uc_value = fragment_sec_value[FRAG_SUB_UCLIST_KEY];
        for(int i=0; i<arr_uc_value.size(); i++) {
            S_UniformControl uc;
            uc.name = arr_uc_value[i][UC_SUB_NAME_KEY].asString();
            uc.typeString = arr_uc_value[i][UC_SUB_TYPE_KEY].asString();
            uc.value = arr_uc_value[i][UC_SUB_VALUE_KEY].asString();
            if((uc.typeString == "sampler2D") && (arr_uc_value[i][UC_SUB_TEXPARAMLIST_KEY] != NULL)) {
                const Json::Value arr_texparam_value = arr_uc_value[i][UC_SUB_TEXPARAMLIST_KEY];
                for(int k=0; k<arr_texparam_value.size(); k++) {
                    S_TextureParam tex_param;
                    tex_param.key = arr_texparam_value[k][TEXPARAM_SUB_KEY_KEY].asInt();
                    tex_param.value = arr_texparam_value[k][TEXPARAM_SUB_VALUE_KEY].asInt();
                    uc.texParamList.insert(uc.texParamList.end(), tex_param);
                }
            }
            fragment_uc_list.insert(fragment_uc_list.end(), uc);
        }
        if(fragment_uc_list.size() > 0) {
            program.code.fragmentCode.fragUniformControlList = fragment_uc_list;
        }
    }
    
    program.valid = true;
    return program;
}

Parser::Parser(string json_data)
{
    Json::Reader reader;
    Json::Value  value;
    
    mError = NONE;
    
    if (reader.parse(json_data, value))
    {
        const Json::Value boxworld_shader_value = value[BOXWORLD_SHADER_KEY];
        if (boxworld_shader_value.empty()) { mError = NO_BOXWORLD; return; }
        
        const Json::Value main_program_value = boxworld_shader_value[PROGRAM_MAIN_KEY];
        mMainProgram = decodeToProgram(main_program_value);
        const Json::Value audio_program_value = boxworld_shader_value[PROGRAM_AUDIO_KEY];
        mAudioProgram = decodeToProgram(audio_program_value);
    }else {
        mError = NO_JASON;
    }
}



//
//  Parse.cpp
//  BoxWorld
//
//  Created by Han Maokun on 10/16/15.
//
//
#include "Parser.h"
#include "json.h"

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
        if (main_program_value.empty()) { mError = NO_MAINPROGRAM; return; }
        
        string main_program_name = main_program_value[PROGRAM_SUB_NAME_KEY].asString();
        const Json::Value main_program_code_value = main_program_value[PROGRAM_SUB_CODE_KEY];
        if (main_program_code_value.empty()) { mError = NO_CODE; return; }

        const Json::Value vertex_sec_value = main_program_code_value[CODE_SUB_VERT_KEY];
        if (vertex_sec_value.empty()) { mError = NO_VERT_CODE; return; }
        {
            /* get vertex code. */
            mMainProgram.code.vertexCode.vert = vertex_sec_value[VERT_SUB_CODE_KEY].asString();
        }
        
        const Json::Value fragment_sec_value = main_program_code_value[CODE_SUB_FRAG_KEY];
        if (fragment_sec_value.empty()) { mError = NO_FRAG_CODE; return; }
        {
            /* get fragment code. */
            mMainProgram.code.fragmentCode.frag = fragment_sec_value[FRAG_SUB_CODE_KEY].asString();
            
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
                mMainProgram.code.fragmentCode.presetList = fragment_preset_list;
            }

            /* get fragment uniform control list. */
            list<S_UniformControl> fragment_uc_list;
            const Json::Value arr_uc_value = fragment_sec_value[FRAG_SUB_UCLIST_KEY];
            for(int i=0; i<arr_uc_value.size(); i++) {
                S_UniformControl uc;
                uc.name = arr_uc_value[i][UC_SUB_NAME_KEY].asString();
                uc.typeString = arr_uc_value[i][UC_SUB_TYPE_KEY].asString();
                uc.value = arr_uc_value[i][UC_SUB_VALUE_KEY].asString();
                fragment_uc_list.insert(fragment_uc_list.end(), uc);
            }
            if(fragment_uc_list.size() > 0) {
                mMainProgram.code.fragmentCode.fragUniformControlList = fragment_uc_list;
            }
        }
        
    }else {
        mError = NO_JASON;
    }
}



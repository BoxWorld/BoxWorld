//
//  ResourceMgrInst.cpp
//  BoxWorld
//
//  Created by Han Maokun on 10/28/15.
//
//

#include "ofMain.h"
#include "ResourceMgrInst.h"

ResourceMgrInst::ResourceMgrInst() {
    mDefAppModel.valid = false;
    mNeedsUpdate = false;
    mCurAppIdx = 0;
    
    /* Init according to manifest file, or create one. */
    ofFile file;
    bool suc = file.open(ofToDataPath(MAIN_MANIFEST_FILE_NAME), ofFile::ReadWrite, false);
    if(!suc) {
        /* create MainManifest.json file. */
        ofFile newMainManifestFile(ofToDataPath(MAIN_MANIFEST_FILE_NAME), ofFile::ReadWrite);
        newMainManifestFile.create();
        
        /* Scan resources under data/ folder, then register results in MainManifest.json file */
        resetManifestFile();
        file.open(ofToDataPath(MAIN_MANIFEST_FILE_NAME), ofFile::ReadOnly, false);
    }
    
    /* parse default shader info. */
    //ofBuffer file_buf = file.readToBuffer();
    //Json::Reader reader;
    
    //if (reader.parse(file_buf.getBinaryBuffer(), mManifestJsonValue)) {
        bool default_shader_valid = mManifestJsonValue[ROOT_KEY_VALID].asBool();
        if(default_shader_valid) {
            string def_app_name = mManifestJsonValue[ROOT_KEY_DEFAULT_APP_NAME].asString();
            mAppsVal = mManifestJsonValue[ROOT_KEY_APPS];
            for(int i=0; i<mAppsVal.size(); i++) {
                if(mAppsVal[i][APP_KEY_NAME] == def_app_name) {
                    mCurAppIdx = i;
                    mDefAppModel.name = def_app_name;
                    mDefAppModel.execFile = mAppsVal[i][APP_KEY_EXEC].asString();
                    mDefAppModel.soundFile = mAppsVal[i][APP_KEY_SOUND].asString();
                    mDefAppModel.valid = mAppsVal[i][APP_KEY_VALID].asBool();
                    break;
                }
            }
        }
    //}
}

ResourceMgrInst::~ResourceMgrInst() {
    if(mNeedsUpdate) {
        /* write updated data into file. */
        updateManifestFile();
    }
}

bool ResourceMgrInst::isDefaultAppValid(){
    return mDefAppModel.valid;
}

bool ResourceMgrInst::isWavFileExists(string raw_name){
    string sound_file_path = "";
    Json::Value apps_val = mManifestJsonValue[ROOT_KEY_APPS];
    for(int i=0; i<apps_val.size(); i++) {
        if(apps_val[i][APP_KEY_NAME] == raw_name) {
            sound_file_path = apps_val[i][APP_KEY_SOUND].asString();
            break;
        }
    }
    
    return (sound_file_path == "")? false:true;
}

void ResourceMgrInst::updateDefApp(string name){
    mManifestJsonValue[ROOT_KEY_DEFAULT_APP_NAME] = name;
    mNeedsUpdate = true;
}

string ResourceMgrInst::getDefAppContent() {
    if(!mDefAppModel.valid) return string("");
    
    string file_path = mDefAppModel.execFile;
    return ofBufferFromFile(file_path).getText();
}

string ResourceMgrInst::getNextAppContent() {
    if(mAppsVal.size() < 2) return string("");
    mCurAppIdx += 1;
    if(mCurAppIdx == mAppsVal.size())
        mCurAppIdx = 0;
    
    updateDefApp(mAppsVal[mCurAppIdx][APP_KEY_NAME].asString());
    std::string file_path = mAppsVal[mCurAppIdx][APP_KEY_EXEC].asString();
    return ofBufferFromFile(file_path).getText();
}

void ResourceMgrInst::resetManifestFile() {
    string default_app_name = "";
    
    /* parse Cache/Wav/ directory. */
    map<string, string> wav_res_map;
    ofDirectory wav_dir(ofToDataPath(string(CACHE_DIR_PREFIX).append(WAV_FILE_DIR_PREFIX)));
    wav_dir.allowExt(WAV_FILE_SUFFIX);
    wav_dir.listDir();
    for(int i=0; i<wav_dir.numFiles(); i++) {
        string raw_name = wav_dir.getName(i).substr(0, wav_dir.getName(i).find_last_of("."));
        wav_res_map[raw_name] = wav_dir.getAbsolutePath().append("/").append(wav_dir.getName(i));
    }

    /* parse App/ directory. */
    Json::Value root;
    ofDirectory app_dir(ofToDataPath(APPS_DIR_PREFIX));
    app_dir.allowExt(APP_FILE_SUFFIX);
    app_dir.listDir();
    Json::Value app_arr;
    for(int i=0; i<app_dir.numFiles(); i++) {
        Json::Value app_item;
        string app_path = app_dir.getAbsolutePath().append("/").append(app_dir.getName(i));
        string raw_name = app_dir.getName(i).substr(0, app_dir.getName(i).find_last_of("."));
        app_item[APP_KEY_NAME] = raw_name;
        app_item[APP_KEY_EXEC] = app_path;
        app_item[APP_KEY_SOUND] = "";
        if(wav_res_map.find(raw_name) != wav_res_map.end()) {
            string wav_path = wav_res_map[raw_name];
            app_item[APP_KEY_SOUND] = wav_path;
        }
        app_item[APP_KEY_VALID] = true;
        app_arr.append(app_item);
        if(raw_name == DEFAULT_APP_NAME) {
            default_app_name = DEFAULT_APP_NAME;
        }
    }
    root[ROOT_KEY_APPS] = app_arr;
    bool boxworld_has_app = false;
    if(default_app_name == DEFAULT_APP_NAME) {
        boxworld_has_app = true;
    }else {
        if(app_arr.size() == 0) {
            boxworld_has_app = false;
        }else {
            boxworld_has_app = true;
            default_app_name = app_dir.getName(0).substr(0, app_dir.getName(0).find_last_of("."));
        }
    }
    root[ROOT_KEY_DEFAULT_APP_NAME] = default_app_name;
    root[ROOT_KEY_VALID] = boxworld_has_app;
    mHasDefaultShader = boxworld_has_app;
    
    ofFile file;
    file.open(ofToDataPath(MAIN_MANIFEST_FILE_NAME), ofFile::ReadWrite, false);
    file.setWriteable(true);
    ofBuffer buf(root.toStyledString());
    bool file_written = file.writeFromBuffer(buf);
    if(!file_written) {
        ofLogError("main manifest file written failed\n");
    }
    file.close();
    mManifestJsonValue = root;
    mAppsVal = mManifestJsonValue[ROOT_KEY_APPS];
}

void ResourceMgrInst::updateWavFileInfo(string wav_file_raw_name) {
    mAppsVal = mManifestJsonValue[ROOT_KEY_APPS];
    for(int i=0; i<mAppsVal.size(); i++) {
        if(mAppsVal[i][APP_KEY_NAME] == wav_file_raw_name) {
            mAppsVal[i][APP_KEY_SOUND] = ofToDataPath(string("Cache/Wav/").append(wav_file_raw_name).append(".wav"));
            break;
        }
    }
    mManifestJsonValue[ROOT_KEY_APPS] = mAppsVal;
    updateManifestFile();
}

void ResourceMgrInst::updateManifestFile(){
    ofFile file;
    file.open(ofToDataPath(MAIN_MANIFEST_FILE_NAME), ofFile::ReadWrite, false);
    file.setWriteable(true);
    ofBuffer buf(mManifestJsonValue.toStyledString());
    bool file_written = file.writeFromBuffer(buf);
    if(!file_written) {}
    file.close();
}

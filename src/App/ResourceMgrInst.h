//
//  ResourceMgrInst.h
//  BoxWorld
//
//  Created by Han Maokun on 10/28/15.
//
//

#ifndef __BoxWorld__ResourceMgrInst__
#define __BoxWorld__ResourceMgrInst__

#include <stdio.h>
#include <string>
#include <list>
#include "json.h"

using namespace std;

#define MAIN_MANIFEST_FILE_NAME "MainManifest.json"
#define DEFAULT_APP_NAME "Shiny-Toy"

static const string APPS_DIR_PREFIX = "Apps/";
static const string CACHE_DIR_PREFIX = "Cache/";
static const string WAV_FILE_DIR_PREFIX = "Wav/";

static const string APP_FILE_SUFFIX = "json";
static const string WAV_FILE_SUFFIX = "wav";

static string ROOT_KEY_APPS = "Apps";
static string ROOT_KEY_DEFAULT_APP_NAME = "DefaultAppName";
static string ROOT_KEY_VALID = "Valid";

static string APP_KEY_NAME = "name";
static string APP_KEY_EXEC = "exec";
static string APP_KEY_SOUND = "sound";
static string APP_KEY_VALID = "valid";

typedef struct{
    string name;
    string execFile;
    string soundFile;
    bool valid;
}S_App;

typedef struct{
    list<S_App> appList;
}S_BoxWorldResource;

class ResourceMgrInst {
public:
    static ResourceMgrInst* get() {
        static ResourceMgrInst *inst = new ResourceMgrInst();
        return inst;
    }
    
    ResourceMgrInst();
    virtual ~ResourceMgrInst();
    
    void setRootDir(string root_dir) { mRootDir = root_dir; }
    string getRootDir() { return mRootDir; }
    string getWavFileDir() { return mRootDir.append(CACHE_DIR_PREFIX).append(WAV_FILE_DIR_PREFIX);}
    void resetManifestFile();
    bool isDefaultAppValid();
    bool isWavFileExists(string raw_name);
    string getDefAppContent();
    
private:
    string mRootDir;
    bool mHasDefaultShader;
    S_App mDefAppModel;
    Json::Value  mManifestJsonValue;
};

#endif /* defined(__BoxWorld__ResourceMgrInst__) */

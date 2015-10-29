//
//  ConnSimuImp.cpp
//  BoxWorld
//
//  Created by Han Maokun on 10/12/15.
//
//

#include "ConnSimuImp.h"
#include "ofUtils.h"
#include "Message.h"
#include "ResourceMgrInst.h"

void ConnSimuImp::init() {
    isConnected = true;
    
    mFileWatcher = new FW::FileWatcher();
    FW::WatchID watchid = mFileWatcher->addWatch(ofToDataPath("Apps", true), this);
    
    return;
}

void ConnSimuImp::update(){
    mFileWatcher->update();
}

void ConnSimuImp::onAppAdd(string name) {
    std::string content(ofBufferFromFile(name).getText());
    if(content == "") {
        cout << "empty file?" << endl;
        return;
    }
    Message *msg = new Message(Message::UPDATE_SHADER_CMD, content);
    mConnListener->onIncomingMsg(msg);
    ResourceMgrInst::get()->resetManifestFile();
    delete msg;
}

void ConnSimuImp::handleFileAction(FW::WatchID watchid, const string& dir, const string& filename, FW::Action action)
{
    static map<string, FILE_EVENTS> fileStatusMap;
    switch(action)
    {
        case FW::Add:{
            std::cout << "File (" << filename << ") Added! " <<  std::endl;
            fileStatusMap[filename] = ADDED;
            break;
        }
        case FW::Modified:{
            if(fileStatusMap.find(filename) != fileStatusMap.end()){
                if(fileStatusMap[filename] == ADDED){
                    onAppAdd(filename);
                    fileStatusMap[filename] = NORMAL;
                }
            }
        }
        default:
            break;
    }
}

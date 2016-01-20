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
    mFileWatcher->addWatch(ofToDataPath("Apps", true), this);
    
    return;
}

void ConnSimuImp::update(){
    mFileWatcher->update();
}

void ConnSimuImp::sendMsg(Message *msg)
{
    
}

void ConnSimuImp::onAppAdd(string name) {
    std::string content(ofBufferFromFile(name).getText());
    if(content == "") {
        cout << "empty file?" << endl;
        return;
    }
    Message *msg = new Message(Message::REQ_UPDATE_SHADER_CMD, content, this, &ConnSimuImp::msgCbConstWrapper);
    mConnListener->onIncomingMsg(msg);
    delete msg;
}

void ConnSimuImp::msgCbConstWrapper(void *user_data, int ret_val, void *args) {
    ConnSimuImp *inst = static_cast<ConnSimuImp *>(user_data);
    inst->msgCallback(ret_val, args);
}

void ConnSimuImp::msgCallback(int ret_val, void *args) {
    ResourceMgrInst::get()->resetManifestFile();
}

void ConnSimuImp::handleFileAction(FW::WatchID watchid, const string& dir, const string& filename, FW::Action action)
{
    static map<string, FILE_EVENTS> fileStatusMap;
    switch(action)
    {
        case FW::Add:{
            std::cout << "File (" << filename << ") Added! " <<  std::endl;
            fileStatusMap[filename] = ADDED;
            onAppAdd(filename);
            fileStatusMap[filename] = NORMAL;
            break;
        }/*
        case FW::Modified:{
            if(fileStatusMap.find(filename) != fileStatusMap.end()){
                if(fileStatusMap[filename] == ADDED){
                    onAppAdd(filename);
                    fileStatusMap[filename] = NORMAL;
                }
            }
        }*/
        default:
            break;
    }
}

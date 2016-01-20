#include <unistd.h>

#include "BoxWorldApp.h"
#include "ConnMgrInst.h"
#include "Parser.h"
#include "ResourceMgrInst.h"

//--------------------------------------------------------------
void BoxWorldApp::setup(){
    ofEnableAlphaBlending();
    ofDisableArbTex();

    string data_resource_path;
#ifdef TARGET_OSX
    // Get the absolute location of the executable file in the bundle.
    CFBundleRef appBundle     = CFBundleGetMainBundle();
    CFURLRef	executableURL = CFBundleCopyExecutableURL(appBundle);
    CFURLRef	dataPrefixURL = CFBundleCopyBundleURL(appBundle);
    char execFile[4096];
    if (CFURLGetFileSystemRepresentation(executableURL, TRUE, (UInt8 *)execFile, 4096))
    {
        // Strip out the filename to just get the path
        string strExecFile = execFile;
        int found = strExecFile.find_last_of("/");
        string strPath = strExecFile.substr(0, found);
        
        // Change the working directory to that of the executable
        if(-1 == chdir(strPath.c_str())) {
            ofLog(OF_LOG_ERROR, "Unable to change working directory to executable's directory.");
        }
    }
    else {
        ofLog(OF_LOG_ERROR, "Unable to identify executable's directory.");
    }  
    CFRelease(executableURL);
    
    char dataPrefix[4096];
    if (CFURLGetFileSystemRepresentation(dataPrefixURL, TRUE, (UInt8 *)dataPrefix, 4096))
    {
        string strExecFile = dataPrefix;
        int found = strExecFile.find_last_of("/");
        string strPath = strExecFile.substr(0, found);
        data_resource_path = strPath.append("/data/");
    }
#elif defined TARGET_LINUX
    char buf[512];
    getcwd(buf, sizeof(buf));
    printf("current working directory : %s\n", buf);
    string data_resource_prefix = buf;
    data_resource_path = "/home/han/devel/BoxWorld/bin/data/";//data_resource_prefix.append("/bin/data/");
#endif
    
    /* Init according to manifest file. */
    ResourceMgrInst::get()->setRootDir(data_resource_path);

    /* Init connection manager for incoming message handling. */
    ConnMgrInst::get()->setCmdReceiver(this);
    ofSetDataPathRoot(data_resource_path);
    
    mShaderExecutor = new ShaderExecutor(BOXWORLD_WIDTH, BOXWORLD_HEIGHT);
    
    if(ResourceMgrInst::get()->isDefaultAppValid()) {
        runAppWithContent(ResourceMgrInst::get()->getDefAppContent());
    }
}

BoxWorldApp::~BoxWorldApp() {
    if(mShaderExecutor) delete mShaderExecutor;
    delete ResourceMgrInst::get();
}

void BoxWorldApp::runAppWithContent(string content) {
    Parser *shader_parser = new Parser(content);
    mShaderExecutor->setProgramModel(shader_parser->getMainProgram(), shader_parser->getAudioProgram());
    delete shader_parser;
}

//--------------------------------------------------------------
void BoxWorldApp::update(){
    mShaderExecutor->update();
    ConnMgrInst::get()->update();
}

//--------------------------------------------------------------
void BoxWorldApp::draw(){
    mShaderExecutor->draw();
}

void BoxWorldApp::updateScene(Message *msg){
    runAppWithContent(msg->getContent());
    msg->runCb(0, NULL);
}

//--------------------------------------------------------------
void BoxWorldApp::keyPressed(int key){

}

//--------------------------------------------------------------
void BoxWorldApp::keyReleased(int key){
    switch(key) {
        case OF_KEY_UP:{
            string next_app_content = ResourceMgrInst::get()->getNextAppContent();
            if(next_app_content != string("")) {
                runAppWithContent(next_app_content);
            }
        }break;
        case OF_KEY_DOWN: {
            
        }break;
    }
}

//--------------------------------------------------------------
void BoxWorldApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void BoxWorldApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void BoxWorldApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void BoxWorldApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void BoxWorldApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void BoxWorldApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void BoxWorldApp::dragEvent(ofDragInfo dragInfo){

}

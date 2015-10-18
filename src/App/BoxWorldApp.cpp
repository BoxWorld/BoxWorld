#include "BoxWorldApp.h"
#include "ConnMgrInst.h"
#include "Parser.h"

//--------------------------------------------------------------
void BoxWorldApp::setup(){
    
#ifdef TARGET_OSX
    // Get the absolute location of the executable file in the bundle.
    CFBundleRef appBundle     = CFBundleGetMainBundle();
    CFURLRef	executableURL = CFBundleCopyExecutableURL(appBundle);
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
#endif
    
    ConnMgrInst::get()->setCmdReceiver(this);
    ofSetDataPathRoot("./data/");
    
    mShaderExecutor = new ShaderExecutor(BOXWORLD_WIDTH, BOXWORLD_HEIGHT);
}

//--------------------------------------------------------------
void BoxWorldApp::update(){
    mShaderExecutor->update();
}

//--------------------------------------------------------------
void BoxWorldApp::draw(){
    mShaderExecutor->draw();
}

//--------------------------------------------------------------
void BoxWorldApp::keyPressed(int key){

}

//--------------------------------------------------------------
void BoxWorldApp::keyReleased(int key){

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

void BoxWorldApp::updateScene(Message *msg){
    Parser *shader_parser = new Parser(msg->getContent());
    mShaderExecutor->setProgramModel(shader_parser->getMainProgram());
}

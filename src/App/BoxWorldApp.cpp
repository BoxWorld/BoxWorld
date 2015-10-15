#include "BoxWorldApp.h"
#include "ConnMgrInst.h"
#include "Parser.h"

//--------------------------------------------------------------
void BoxWorldApp::setup(){
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

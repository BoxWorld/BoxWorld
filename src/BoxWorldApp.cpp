#include "BoxWorldApp.h"
#include "ConnMgrInst.h"

//--------------------------------------------------------------
void BoxWorldApp::setup(){
    ConnMgrInst::get()->setCmdReceiver(this);
    ofSetDataPathRoot("./data/");
}

//--------------------------------------------------------------
void BoxWorldApp::update(){

}

//--------------------------------------------------------------
void BoxWorldApp::draw(){

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
    
}

#include <unistd.h>

#include "BoxWorldApp.h"
#include "ConnMgrInst.h"
#include "Parser.h"
#include "ResourceMgrInst.h"
#include "intelRealsenseMgr.h"
#include "BoxWorldWindowAttrib.h"

//--------------------------------------------------------------
void BoxWorldApp::setup(){
    string finalDisplayVert = "#version 330\nlayout(location = 0) \
    in vec3 VertexPosition; \
    uniform mat4 TileMatrix = mat4(1.0); \
    uniform vec2 resolution; \
    out vec4 glFragCoord; \
    void main() { \
    vec2 v = VertexPosition.xy; \
    v.x = v.x * TileMatrix[0][0] + TileMatrix[3][0]; \
    v.y = v.y * TileMatrix[1][1] + TileMatrix[3][1]; \
    v = v * 0.5 + 0.5; \
    glFragCoord.xy = v * resolution; \
    gl_Position = vec4(VertexPosition, 1.0); \
    }";
    
    string finalDisplayFrag = "#version 330\nlayout(location = 0) \
    out vec4 glFragColor; \
    in vec4 glFragCoord; \
    uniform vec3 iResolution; \
    uniform sampler2D tex; \
    void mainImage(out vec4 fragColor,in vec2 fragCoord) { \
        vec2 uv = fragCoord.xy / iResolution.xy; \
        vec4 col = texture(tex, uv); \
        fragColor = col; \
    } \
    void main() { \
        vec4 color; \
        mainImage(color, glFragCoord.xy); \
        glFragColor = color; \
    }";
    
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
    char *dir = getcwd(buf, sizeof(buf));
    printf("current working directory : %s\n", dir);
    string data_resource_prefix = dir;
    data_resource_path = "/home/han/devel/BoxWorld/bin/data/";//data_resource_prefix.append("/bin/data/");
#endif
    
    /* Init according to manifest file. */
    ResourceMgrInst::get()->setRootDir(data_resource_path);
    BoxWorldWindowAttrib::getInst().load();
    /* Init connection manager for incoming message handling. */
    ConnMgrInst::get()->setCmdReceiver(this);
    ofSetDataPathRoot(data_resource_path);
    
    mShaderExecutor = new ShaderExecutor(BOXWORLD_WIDTH, BOXWORLD_HEIGHT);
    
    if(ResourceMgrInst::get()->isDefaultAppValid()) {
        runAppWithContent(ResourceMgrInst::get()->getDefAppContent());
    }
    
    /* Init final display map shader, to map a 'BOXWORLD_WIDTH X BOXWORLD_HEIGHT' sized buffer to full screen */
    mFinalDisplayShader.unload();
    mFinalDisplayShader.setupShaderFromSource(GL_VERTEX_SHADER, finalDisplayVert);
    mFinalDisplayShader.setupShaderFromSource(GL_FRAGMENT_SHADER, finalDisplayFrag);
    mFinalDisplayShader.linkProgram();
    
    mWinSize = ofVec2f(ofGetWindowWidth(), ofGetWindowHeight());
    printf("%f-%f\n", mWinSize.x, mWinSize.y);
    mFinalDisplayPlane.set(mWinSize.x, mWinSize.y);
    mFinalDisplayPlane.setPosition(mWinSize.x, mWinSize.y, 0);
    mFinalDisplayPlane.setResolution(2, 2);
    mTileMat = ofMatrix4x4::newTranslationMatrix(0.0f, 0.0f, 0.0f) * ofMatrix4x4::newScaleMatrix(1.0f, 1.0f, 0.0f);
    
    mEdit = false;
    mMouse = true;
}

BoxWorldApp::~BoxWorldApp() {
    if(mShaderExecutor) delete mShaderExecutor;
    delete ResourceMgrInst::get();
    delete intelrsMgr::get();
}

void BoxWorldApp::runAppWithContent(string content) {
    Parser *shader_parser = new Parser(content);
    mShaderExecutor->setProgramModel(shader_parser->getMainProgram(), shader_parser->getAudioProgram());
    delete shader_parser;
}

//--------------------------------------------------------------
void BoxWorldApp::update(){
    BoxWorldWindowAttrib::getInst().update();
    mTileMat = ofMatrix4x4::newTranslationMatrix(BoxWorldWindowAttrib::getInst().transX/640.0,
                                                 BoxWorldWindowAttrib::getInst().transY/-480.0, 0.0f)
             * ofMatrix4x4::newScaleMatrix(BoxWorldWindowAttrib::getInst().scaleX,
                                           BoxWorldWindowAttrib::getInst().scaleX * 0.75, 0.0f);

    mShaderExecutor->update();
    ConnMgrInst::get()->update();
}

//--------------------------------------------------------------
void BoxWorldApp::draw(){
    //mShaderExecutor->draw();
    if(mEdit)
        ofBackgroundGradient(ofColor::gray, ofColor::black);
    
    mFinalDisplayShader.begin();
    mFinalDisplayShader.setUniformTexture("tex", mShaderExecutor->getResultTexture(), 0);
    mFinalDisplayShader.setUniformMatrix4f("TileMatrix", mTileMat);
    mFinalDisplayShader.setUniform2f("resolution", mWinSize);
    mFinalDisplayShader.setUniform3f("iResolution", mWinSize.x, mWinSize.y, 0.0);
    mFinalDisplayPlane.draw();
    mFinalDisplayShader.end();
    
    if(mEdit)
        BoxWorldWindowAttrib::getInst().draw();
}

void BoxWorldApp::updateScene(Message *msg){
    runAppWithContent(msg->getContent());
    msg->runCb(0, NULL);
}

//--------------------------------------------------------------
void BoxWorldApp::keyPressed(int key){
    if( key == 'e' ){
        mEdit = !mEdit;
    }else if (key == 'm'){
        mMouse = !mMouse;
        if (mMouse){
            ofShowCursor();
        } else {
            ofHideCursor();
        }
    }else if (key == 'f'){
        ofToggleFullscreen();
    }
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

void BoxWorldApp::exit(){
    BoxWorldWindowAttrib::getInst().save();
}

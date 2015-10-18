#include "ofMain.h"
#include "BoxWorldApp.h"
#include "Config.h"
#include "ofAppGLFWWindow.h"

//========================================================================

#define USE_PROGRAMMABLE_RENDERER

#ifdef USE_PROGRAMMABLE_RENDERER
#include "ofGLProgrammableRenderer.h"
#endif

int main( ){
#ifdef USE_PROGRAMMABLE_RENDERER
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
#endif
    
	ofSetupOpenGL(BOXWORLD_WIDTH, BOXWORLD_HEIGHT, OF_WINDOW);			// <-------- setup the GL context
    //static_cast<ofAppGLFWWindow*>(ofGetWindowPtr())->setOpenGLVersion(4, 0);
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new BoxWorldApp());

}

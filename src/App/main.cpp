#include "ofMain.h"
#include "BoxWorldApp.h"
#include "Config.h"

//========================================================================
int main( ){
	ofSetupOpenGL(BOXWORLD_WIDTH, BOXWORLD_HEIGHT, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new BoxWorldApp());

}

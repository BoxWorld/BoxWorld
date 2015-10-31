#include "ofMain.h"
#include "BoxWorldApp.h"
#include "Config.h"
#include "ofAppGLFWWindow.h"

//========================================================================

int main( ){
	ofSetupOpenGL(BOXWORLD_WIDTH, BOXWORLD_HEIGHT, OF_WINDOW);
	ofRunApp(new BoxWorldApp());
}

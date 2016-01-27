#include "ofMain.h"
#include "BoxWorldApp.h"
#include "BoxWorldConfig.h"
#include "ofAppGLFWWindow.h"
//========================================================================

int main( ){
    /* Customized gl version(4, 0) settings. */
#ifdef TARGET_OPENGLES
    ofGLESWindowSettings settings;
    settings.glesVersion = 2;
#else
    ofGLWindowSettings settings;
    settings.glVersionMajor = 3;
    settings.glVersionMinor = 3;
#endif
    //settings.width = 1024;
    //settings.height = 576;
    settings.windowMode = OF_FULLSCREEN;
    ofCreateWindow(settings);
    
	ofRunApp(new BoxWorldApp());
}
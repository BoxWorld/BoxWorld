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
    settings.width = 640;
    settings.height = 480;
    settings.windowMode = OF_WINDOW;
    ofCreateWindow(settings);
    
	ofRunApp(new BoxWorldApp());
}
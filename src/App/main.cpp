#include "ofMain.h"
#include "BoxWorldApp.h"
#include "BoxWorldConfig.h"
#include "ofAppGLFWWindow.h"

//========================================================================

int main( ){
    
    /* Customized gl version(4, 0) settings. */
#ifdef TARGET_OPENGLES
    ofGLESWindowSettings settings;
    settings.glesVersion = 1;
#else
    ofGLWindowSettings settings;
    settings.glVersionMajor = 4;
    settings.glVersionMinor = 0;
#endif
    settings.width = BOXWORLD_WIDTH;
    settings.height = BOXWORLD_HEIGHT;
    settings.windowMode = OF_WINDOW;
    ofCreateWindow(settings);
    
	ofRunApp(new BoxWorldApp());
}

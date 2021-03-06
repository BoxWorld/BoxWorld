#pragma once

#include "ofMain.h"
#include "CmdReceiverIf.h"
#include "ShaderExecutor.h"

class BoxWorldApp : public ofBaseApp, CmdReceiverIf{

public:
    virtual ~BoxWorldApp();
    
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    /* CmdReceiverIf Implementations. */
    void updateScene(Message *msg);
		
private:
    void runAppWithContent(string content);
    
    ShaderExecutor *mShaderExecutor;
};

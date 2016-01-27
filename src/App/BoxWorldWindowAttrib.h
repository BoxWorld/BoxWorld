//
//  BoxWorldWindowAttrib.h
//  BoxWorld
//
//  Created by Han Maokun on 1/27/16.
//
//

#ifndef BoxWorldWindowAttrib_h
#define BoxWorldWindowAttrib_h

#include "ofMain.h"
#include "ofxGui.h"

class BoxWorldWindowAttrib {
public:
    static BoxWorldWindowAttrib &getInst() {
        static BoxWorldWindowAttrib instance;
        return instance;
    }
    ~BoxWorldWindowAttrib(){}
    
    void    load();
    void    save();
    void    update();
    void    draw();
    
    ofxPanel        gui;
    
    ofxIntSlider    transX, transY;
    ofxFloatSlider  scaleX;
    //ofxFloatSlider  scaleY, ratio;
    ofxIntSlider    minDist, maxDist;
    
private:
    BoxWorldWindowAttrib();
};

#endif /* BoxWorldWindowAttrib_hp*/
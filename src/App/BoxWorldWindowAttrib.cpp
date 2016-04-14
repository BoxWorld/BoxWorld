//
//  BoxWorldWindowAttrib.cpp
//  BoxWorld
//
//  Created by Han Maokun on 1/27/16.
//
//

#include "BoxWorldWindowAttrib.h"

BoxWorldWindowAttrib::BoxWorldWindowAttrib() {}

void BoxWorldWindowAttrib::load(){
    gui.setPosition(0, 0);
    gui.setup("Panel");
    
    gui.add(blurCtr.setup("Blur.Ctr", 1, 1, 10));
    gui.add(transX.setup("Trans.X", 0, -192, 640));
    gui.add(transY.setup("Trans.Y", 0, -144, 480));
    gui.add(scaleX.setup("Scale.X", 1.0, 0.0, 1.6));
    gui.add(scaleY.setup("Scale.Y", 1.0, 0.0, 1.6));
    //gui.add(ratio.setup("Ratio", 1.77777777777778, 1.77777777777778, 1.77916666666667));
    gui.add(minDist.setup("Dist.Min", 300, 200, 700));
    gui.add(maxDist.setup("Dist.Max", 700, 399, 1000));
    
    gui.add(minHeight.setup("Height.Min", 0, -500, 178056));
    gui.add(maxHeight.setup("Height.Max", 100, -200, 571536));
    
    gui.add(k.setup("k", 0.6, 0, 1));
    gui.add(b.setup("b", 0.2, 0, 1));
    
    gui.add(ka.setup("ka", 1.0, 0, 3));
    gui.add(kb.setup("kb", 1.0, 0, 3));
    gui.add(kc.setup("kc", 1.0, 0, 3));


    gui.loadFromFile("settings.xml");
}

void BoxWorldWindowAttrib::save(){
    gui.saveToFile("settings.xml");
}

void BoxWorldWindowAttrib::update(){
    
}

void BoxWorldWindowAttrib::draw(){
    gui.draw();
}
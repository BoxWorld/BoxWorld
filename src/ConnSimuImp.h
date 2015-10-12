//
//  ConnSimuImp.h
//  BoxWorld
//
//  Created by Han Maokun on 10/12/15.
//
//

#ifndef BoxWorld_ConnSimuImp_h
#define BoxWorld_ConnSimuImp_h

#include "ConnIf.h"
#include "ofFileUtils.h"
#include "ofxTimer.h"

class ConnSimuImp : public ConnIf {
public:
    ConnSimuImp (ConnListenerIf *listener) {
        mConnListener = listener;
        init();
    }
    virtual ~ConnSimuImp() {}
    
    void init();
private:
    ofFile   mFile;
    ofxTimer mTimer;
    
    void scanDataDir(ofEventArgs& args);
};

#endif

//
//  ThreadIf.h
//  BoxWorld
//
//  Created by Han Maokun on 10/30/15.
//
//

#ifndef BoxWorld_ThreadIf_h
#define BoxWorld_ThreadIf_h

class BoxWorldThreadCallbackIf {
public:
    virtual ~BoxWorldThreadCallbackIf() {};
    
    virtual void threadCallBack(void *) = 0;
};

#endif

//
//  CmdIf.h
//  BoxWorld
//
//  Created by Han Maokun on 10/12/15.
//
//

#ifndef BoxWorld_CmdIf_h
#define BoxWorld_CmdIf_h

class CmdIf {
public:
    virtual ~CmdIf() {}
    
    virtual void exec() = 0;
};

#endif

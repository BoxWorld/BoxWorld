//
//  UniformDate.h
//  BoxWorld
//
//  Created by Han Maokun on 10/17/15.
//
//

#ifndef BoxWorld_UniformDate_h
#define BoxWorld_UniformDate_h

#include "UniformIf.h"
#include "ofMain.h"
#include <time.h>

class UniformDate : public UniformIf {
public:
    UniformDate () {
        mNeedRefresh = true;
    }
    
    void applyValue() {
        time_t theTime = time(NULL);
        struct tm *aTime = localtime(&theTime);
        
        int day = aTime->tm_mday;
        int month = aTime->tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
        int year = aTime->tm_year + 1900; // Year is # years since 1900
        
        int seconds = aTime->tm_hour * 60 * 60
                            + aTime->tm_min * 60
                            + aTime->tm_sec;
        
        glUniform4f(getLocation(), year, month, day, seconds);
    }
    
    static UniformIf * __stdcall Create() { return new UniformDate(); }
};

#endif

//
//  UniformTypeMat2.h
//  BoxWorld
//
//  Created by Han Maokun on 10/18/15.
//
//

#ifndef BoxWorld_UniformTypeMat2_h
#define BoxWorld_UniformTypeMat2_h

#include "UniformIf.h"

class UniformTypeMat2 : public UniformIf {
public:
    void applyValue() {
        /* remove the first '(' or '[' and the last ')' or ']'. */
        string num_content = mValue.substr(1, mValue.length()-2);
        std::vector<float> vect;
        std::stringstream ss(num_content);
        
        float f;
        while (ss >> f)
        {
            vect.push_back(f);
            /* ignore space and comma. */
            if (ss.peek() == ',' || ss.peek() == ' ')
                ss.ignore();
        }
        
        float valueArr[2];
        valueArr[0] = vect.at(0);
        valueArr[1] = vect.at(1);
        mCtrl->applyMat2OnShader(getLocation(), valueArr);
    }
    
    static UniformIf * __stdcall Create() { return new UniformTypeMat2(); }
};

#endif

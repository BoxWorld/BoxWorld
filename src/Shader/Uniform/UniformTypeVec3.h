//
//  UniformTypeVec3.h
//  BoxWorld
//
//  Created by Han Maokun on 10/18/15.
//
//

#ifndef BoxWorld_UniformTypeVec3_h
#define BoxWorld_UniformTypeVec3_h

#include "UniformIf.h"

class UniformTypeVec3 : public UniformIf {
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
        mCtrl->applyVec3OnShader(getLocation(), vect.at(0), vect.at(1), vect.at(2));
    }
    
    static UniformIf * __stdcall Create() { return new UniformTypeVec3(); }
};

#endif

//
//  UniformTypeVec4.h
//  BoxWorld
//
//  Created by Han Maokun on 10/18/15.
//
//

#ifndef BoxWorld_UniformTypeVec4_h
#define BoxWorld_UniformTypeVec4_h

#include "UniformIf.h"

class UniformTypeVec4 : public UniformIf {
public:
    void applyValue() {
        string num_content;
        string first_c = mValue.substr(0,1);
        if((first_c == "(") || (first_c == "[")) {
            /* remove the first '(' or '[' and the last ')' or ']'. */
            num_content = mValue.substr(1, mValue.length()-2);
        }else {
            num_content = mValue;
        }
        
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
        mCtrl->applyVec4OnShader(getLocation(), vect.at(0), vect.at(1), vect.at(2), vect.at(3));
    }
    
    static UniformIf * Create() { return new UniformTypeVec4(); }
};

#endif

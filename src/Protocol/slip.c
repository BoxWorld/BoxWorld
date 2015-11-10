//
//  slip.c
//  BoxWorld
//
//  Created by Han Maokun on 11/10/15.
//
//

#include "slip.h"

int slip_esc(unsigned char flags, const unsigned char *src, unsigned char *dst, int len) {
    unsigned char *dptr = dst;
    
    if (!(flags & SLIP_NO_EXTRA_END))
        *dptr++ = SLIP_END;
    
    while (len--) {
        switch (*src) {
            case SLIP_ESC:
                *dptr++ = SLIP_ESC;
                *dptr++ = SLIP_ESC_ESC;
                break;
                
            case SLIP_END:
                *dptr++ = SLIP_ESC;
                *dptr++ = SLIP_ESC_END;
                break;
                
            default:
                *dptr++ = *src;
                break;
        }
        
        src++;
    }
    
    if (!(flags & SLIP_NO_END))
        *dptr++ = SLIP_END;
    
    return (dptr - dst);
}
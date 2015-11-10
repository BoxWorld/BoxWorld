//
//  slip.h
//  BoxWorld
//
//  Created by Han Maokun on 11/10/15.
//
//

#ifndef __BoxWorld__slip__
#define __BoxWorld__slip__

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdio.h>
    
#define MAX_FRAME_LEN   16384
#define MIN_FRAME_LEN	(2 + 1)
#define MAX_RCV_BUF_LEN (MAX_FRAME_LEN*2 + 2 + 1)

#define SLIP_FRAMING_OVERHEAD	2		/* bytes, max */

/*
	Special characters
 */
#define SLIP_END				192		/* 0300, end of packet */
#define SLIP_ESC				219		/* 0333, byte stuffing */
#define SLIP_ESC_END			220		/* 0334, END data byte */
#define SLIP_ESC_ESC			221		/* 0335, ESC data byte */

/*
	Flags for slip_esc()
	
	These are useful for constructing an encoded packet in parts.
 */
#define SLIP_NO_EXTRA_END		0x01	/* no extra end-of-packet char */
#define SLIP_NO_END				0x02	/* no end-of-packet char */

/*
	Inserts necessary escape characters to data.
 
	flags
 SLIP_NO_EXTRA_END, SLIP_NO_END
	src
 raw packet data
	dst
 encoded packet data
	len
 number of src bytes
 */
int slip_esc(unsigned char flags, const unsigned char *src, unsigned char *dst, int len);

#ifdef __cplusplus
}
#endif

#endif /* defined(__BoxWorld__slip__) */

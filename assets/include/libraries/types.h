
/*---------------------------------------------------------------------------------

	Header file for datatypes.

	Copyright 19XX-2020 by Knightsurfer.

	A cleanup project to make a library similar to libgba but not so cluttered.

---------------------------------------------------------------------------------*/
/*! \file  types.h
    \brief datatype definitions.
*/
//---------------------------------------------------------------------------------
#ifndef _types_h_
#define _types_h_
//---------------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
//---------------------------------------------------------------------------------
// Data	types
//---------------------------------------------------------------------------------

#define SaveData	((u8*)	CARTRAM)  // Save the initial data for the fibonai sequence



//HeaderData[0] = 'F';

typedef uint8_t         u8,  byte;    //Unsigned 8 bit value.
typedef uint16_t        u16, hword;   //Unsigned 16 bit value.
typedef uint32_t        u32, word;    //Unsigned 32 bit value.
typedef uint64_t 		u64;		  //Unsigned 64 bit value.

typedef int8_t          s8;    		  //Signed 8 bit value.
typedef int16_t         s16;   		  //Signed 16 bit value.
typedef int32_t         s32;   		  //Signed 32 bit value.
typedef int64_t 		s64;		  //Signed 64 bit value.

typedef volatile u8     vu8;  		  //Volatile unsigned 8 bit value.
typedef volatile u16    vu16; 		  //Volatile unsigned 16 bit value.
typedef volatile u32    vu32; 		  //Volatile unsigned 32 bit value.
typedef volatile u64 	vu64;		  //Volatile unsigned 64 bit value.

typedef volatile s8     vs8;  		  //Volatile signed 8 bit value.
typedef volatile s16    vs16; 		  //Volatile signed 16 bit value.
typedef volatile s32    vs32; 		  //Volatile signed 32 bit value.
typedef volatile s64 	vs64;		  //Volatile signed 64 bit value.

typedef s32 FIXED;					  //!< Fixed point type
typedef u16 COLOR;					  //!< A Color.
#define ALIGN4	__attribute__((aligned(4)))
#define INLINE static inline

typedef struct OBJ_ATTR{ //An ordinary sprite.
u16 attr0;
u16 attr1;
u16 attr2;
s16 fill;
} ALIGN4 OBJ_ATTR;

typedef struct OBJ_AFFINE //An affine sprite.
{
	u16 fill0[3];	s16 pa;
	u16 fill1[3];	s16 pb;
	u16 fill2[3];	s16 pc;
	u16 fill3[3];	s16 pd;
} ALIGN4 OBJ_AFFINE;
//---------------------------------------------------------------------------------


//#ifndef __INTELLISENSE_H__
//#define __INTELLISENSE_H__

//#if __INTELLISENSE__
#define __attribute__(q)
#define __builtin_strcmp(a,b) 0
#define __builtin_strlen(a) 0
#define __builtin_memcpy(a,b) 0
#define __builtin_va_list void*
#define __builtin_va_start(a,b)
#define __extension__

#ifndef	NULL
#define	NULL	0
#endif

//#endif
//#endif //__INTELLISENSE_H__

//In theory this stuff will compile data where I tell it to go.
#define IWRAM_CODE	__attribute__((section(".iwram"), long_call))
#define EWRAM_CODE	__attribute__((section(".ewram"), long_call))

#define IWRAM_DATA	__attribute__((section(".iwram")))
#define EWRAM_DATA	__attribute__((section(".ewram")))
#define EWRAM_BSS	__attribute__((section(".sbss")))

#define ALIGN(m)	__attribute__((aligned (m)))

#endif // _types_h_
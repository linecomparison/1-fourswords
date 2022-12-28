//Bios call macro
#if	defined	( __thumb__ )
            #define	SystemCall(Number)	 __asm ("SWI	  "#Number"\n" :::  "r0", "r1", "r2", "r3") //! \def SystemCall(Number) \brief helper macro to insert a bios call. \param Number swi number to call Inserts a swi of the correct format for arm or thumb code.
#else
            #define	SystemCall(Number)	 __asm ("SWI	  "#Number"	<< 16\n" :::"r0", "r1", "r2", "r3") //! \def SystemCall(Number) \brief helper macro to insert a bios call. \param Number swi number to call Inserts a swi of the correct format for arm or thumb code.
#endif

//====================================================================
// BIT Related Stuff // I understand very little about 
// BIT shifting, and thus I have barely touched it.
//================================================================
#define BIT(number)             (1<<(number)) //! \def BIT(number)  \brief Macro for bit settings. \param number bit number to set
#define BIT_SHIFT(a, n)			( (a)<<(n) )
#define BIT_SET(word, flag)		( word |=  (flag) )
#define BIT_CLEAR(word, flag)	( word &= ~(flag) )
#define BIT_FLIP(word, flag)	( word ^=  (flag) )
#define BIT_EQ(word, flag)		( ((word)&(flag)) == (flag) )

#define BFN_PREP(x, name)		( ((x)<<name##_SHIFT) & name##_MASK )
#define BFN_GET(y, name)		( ((y) & name##_MASK)>>name##_SHIFT )
#define BFN_SET(y, x, name)		(y = ((y)&~name##_MASK) | BFN_PREP(x,name) )

#define BFN_PREP2(x, name)		( (x) & name##_MASK )
#define BFN_GET2(y, name)		( (y) & name##_MASK )
#define BFN_SET2(y, x, name)	(y = ((y)&~name##_MASK) | BFN_PREP2(x,name) )

#define BASE            IO 		   //!\def REG_BASE \brief Base address of gba hardware registers.

#define RGB5(r,g,b) (((r / 31) * 3 & 0x1F)|(((g / 31) * 3 & 0x1F)<<5)|(((b / 31) * 3 & 0x1F)<<10)) //Draw in 16 bit colour.
#define RGB3(r,g,b)  ((((b)>>3)<<10)        | (((g)>>3)<<5)                  |       ((r)>>3))

#define ATTR1_BUILDR(x, size, hflip, vflip)	\
( ((x)&511) | (((hflip)&1)<<12) | (((vflip)&1)<<13) | (((size)&3)<<14) )

#define ATTR0_BUILD(y, shape, bpp, mode, mos, bld, win)				\
(																	\
	((y)&255) | (((mode)&3)<<8) | (((bld)&1)<<10) | (((win)&1)<<11) \
	| (((mos)&1)<<12) | (((bpp)&8)<<10)| (((shape)&3)<<14)			\
)

#define ATTR1_BUILDA(x, size, affid)			\
( ((x)&511) | (((affid)&31)<<9) | (((size)&3)<<14) )

#define ATTR2_BUILD(id, 			pbank, prio)	\
( ((id)&0x3FF) | 					(((pbank)&15)<<12) | (((prio)&3)<<10) )

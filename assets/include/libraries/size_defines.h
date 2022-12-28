//Memory_Sizes
#define EWRAM_SIZE  	0x3FFFF //!\def EWRAM \brief Base address of end of external work ram
#define VRAM_SIZE	    0x18000 //How big the vram is in general.
#define PAGE_SIZE	    0x0A000 //How big a page size is for page flipping.
#define PAL_SIZE	    0x00400 //How big a palette is.
#define OAM_SIZE	    0x00400 //How big OAM is.

#define M3_SIZE		    0x12C00 //How big Mode 3 is.
#define M4_SIZE		    0x09600 //How big Mode 4 is.
#define M5_SIZE		    0x0A000 //How big Mode 5 is.

#define PAL_BG_SIZE	    0x00200 //The size of a tilemap palette.
#define PAL_OBJ_SIZE	0x00200 //The size of a sprite palette.
#define VRAM_BG_SIZE	0x10000 //The size of a tilemap.
#define VRAM_OBJ_SIZE	0x08000 //The size of a sprite.
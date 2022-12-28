#include "resources/tileSets.h"
#include "resources/spriteMaps.h"
#include "resources/customResources.h"

#include "libraries/size_defines.h"
#include "libraries/structs.h"
#include "libraries/macros.h"
#include "libraries/enums.h"

#include "libraries/base.h"
#include "libraries/input.h"

#define REG_DISPCNT		    		*(vu32*)(BASE+0x0000)	// Register Display Controller, basicly it tell the gba what videomode and background to use. \param usage REGDISCNT = MODE | BACKGROUND

#define BG0		        			0x0100	//!< Enable bg 0
#define BG1   						0x0200	//!< Enable bg 1
#define BG2			    			0x0400	//!< Enable bg 2
#define BG3  						0x0800	//!< Enable bg 3
#define OBJ      					0x1000	//!< Enable objects

#define OBJ_2D	      				0x0000	//!< OBJ-VRAM as matrix
#define OBJ_1D   					0x0040	//!< OBJ-VRAM as array
#define VRAM_OBJ					(VRAM + VRAM_BG_SIZE) 	//The area of memory that stores sprites.
//-----------------------------------------------------------------------
#define ATTR0_REG					0x0000	//!< Regular object
#define ATTR0_AFF					0x0100	//!< Affine object
#define ATTR0_HIDE					0x0200	//!< Inactive object
#define ATTR0_AFF_DBL				0x0300	//!< Double-size affine object
#define ATTR0_AFF_DBL_BIT			0x0200
#define ATTR0_BLEND					0x0400	//!< Enable blend
#define ATTR0_WINDOW				0x0800	//!< Use for object window
#define ATTR0_MOSAIC				0x1000	//!< Enable mosaic
#define ATTR0_4BPP					0x0000	//!< Use 4bpp (16 color) tiles
#define ATTR0_8BPP					0x2000	//!< Use 8bpp (256 color) tiles
#define ATTR0_SQUARE				0x0000	//!< Square shape
#define ATTR0_WIDE					0x4000	//!< Tall shape (height &gt; width)
#define ATTR0_TALL					0x8000	//!< Wide shape (height &lt; width)

#define ATTR0_Y_MASK				0x00FF
#define ATTR0_Y_SHIFT			 	0
#define ATTR0_Y(n)					((n)<<ATTR0_Y_SHIFT)

#define ATTR0_MODE_MASK				0x0300
#define ATTR0_MODE_SHIFT			8
#define ATTR0_MODE(n)				((n)<<ATTR0_MODE_SHIFT)

#define ATTR0_SHAPE_MASK			0xC000
#define ATTR0_SHAPE_SHIFT			14
#define ATTR0_SHAPE(n)				((n)<<ATTR0_SHAPE_SHIFT)

//-----------------------------------------------------------------------
#define ATTR1_HFLIP					0x1000	//!< Horizontal flip (reg obj only)
#define ATTR1_VFLIP					0x2000	//!< Vertical flip (reg obj only)
//-----------------------------------------------------------------------
// Base sizes
#define ATTR1_SIZE_8			 	0
#define ATTR1_SIZE_16				0x4000
#define ATTR1_SIZE_32				0x8000
#define ATTR1_SIZE_64				0xC000
// Square sizes
#define ATTR1_SIZE_8x8			 	0	//!< Size flag for  8x8 px object
#define ATTR1_SIZE_16x16			0x4000	//!< Size flag for 16x16 px object
#define ATTR1_SIZE_32x32			0x8000	//!< Size flag for 32x32 px object
#define ATTR1_SIZE_64x64			0xC000	//!< Size flag for 64x64 px object
// Tall sizes
#define ATTR1_SIZE_8x16			 	0	//!< Size flag for  8x16 px object
#define ATTR1_SIZE_8x32				0x4000	//!< Size flag for  8x32 px object
#define ATTR1_SIZE_16x32			0x8000	//!< Size flag for 16x32 px object
#define ATTR1_SIZE_32x64			0xC000	//!< Size flag for 32x64 px object
// Wide sizes
#define ATTR1_SIZE_16x8			 	0	//!< Size flag for 16x8 px object
#define ATTR1_SIZE_32x8				0x4000	//!< Size flag for 32x8 px object
#define ATTR1_SIZE_32x16			0x8000	//!< Size flag for 32x16 px object
#define ATTR1_SIZE_64x32			0xC000	//!< Size flag for 64x64 px object
//----------------------------------------------------------------------
#define ATTR1_X_MASK				0x01FF
#define ATTR1_X_SHIFT			 	0
#define ATTR1_X(n)					((n)<<ATTR1_X_SHIFT)

#define ATTR1_AFF_ID_MASK			0x3E00
#define ATTR1_AFF_ID_SHIFT		 	9
#define ATTR1_AFF_ID(n)				((n)<<ATTR1_AFF_ID_SHIFT)

#define ATTR1_FLIP_MASK				0x3000
#define ATTR1_FLIP_SHIFT			12
#define ATTR1_FLIP(n)				((n)<<ATTR1_FLIP_SHIFT)

#define ATTR1_SIZE_MASK				0xC000
#define ATTR1_SIZE_SHIFT			14
#define ATTR1_SIZE(n)				((n)<<ATTR1_SIZE_SHIFT)
//-----------------------------------------------------------------------
#define ATTR2_ID_MASK				0x03FF
#define ATTR2_ID_SHIFT			 	0
#define ATTR2_ID(n)					((n)<<ATTR2_ID_SHIFT)
#define ATTR2_PRIO_MASK				0x0C00
#define ATTR2_PRIO_SHIFT			10
#define ATTR2_PRIO(n)				((n)<<ATTR2_PRIO_SHIFT)
#define ATTR2_PALBANK_MASK			0xF000
#define ATTR2_PALBANK_SHIFT			12
#define ATTR2_PALBANK(n)			((n)<<ATTR2_PALBANK_SHIFT)

typedef struct 						{ u32 data[8];  } TILE, TILE4;
typedef struct 						{ u32 data[16]; } TILE8;

typedef TILE						CHARBLOCK[512];
typedef TILE8						CHARBLOCK8[256];

#define tile8_mem					((CHARBLOCK8*)VRAM)
#define tile_mem_obj				((CHARBLOCK*)VRAM_OBJ)
#define tile8_mem_obj				((CHARBLOCK8*)VRAM_OBJ)


extern TileSet tileset[10];
extern GameData game;
extern LevelData level;
extern MapLayer lyr[5];
extern Entity ent[20];
extern FileData file[2]; 

extern int videoMode;
extern bool BG[4]; 						//Background toggles that should probably be universal.
extern bool OBJTOGGLE; 					//OBJ toggles that should probably be universal.

extern int palIndexesS[16];
extern int palData[16];

void UpdateSpriteFrame(int entNo,int frame);

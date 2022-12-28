#include "libraries/base.h"
#include "resources/tileSets.h"
#include "libraries/structs.h"
#include "libraries/input.h"
#include "libraries/macros.h"


#define BG_PRIO(n)					((n)<<BG_PRIO_SHIFT)
#define BG_SBB(n)					((n)<<BG_SBB_SHIFT)
#define BG_CBB(n)					((n)<<BG_CBB_SHIFT)
#define BG_PRIO_SHIFT				0
#define BG_CBB_SHIFT				2
#define BG_SBB_SHIFT				8

#define BG_SIZE(m)					((m<<14))
//-----------------------------------------------------------------------
//!\enum BG_CTRL_BITS \brief bit values for background control
enum BG_CTRL_BITS {
	BG_MOSAIC			=	BIT(6),		// enable background mosaic.
	BG_16_COLOR			=	(0<<7),		// background uses 16 color tiles.
	BG_256_COLOR		=	BIT(7),		// background uses 256 color tiles.
	BG_WRAP				=	BIT(13),	// background wraps when scrolling.
	BG_SIZE_256			=	BG_SIZE(0),	// Map Size 256x256.
	BG_SIZE_512x256		=	BG_SIZE(1),	// Map Size 512x256.
	BG_SIZE_256x512		=	BG_SIZE(2),	// Map Size 256x512.
	BG_SIZE_512			=	BG_SIZE(3),	// Map Size 512x512.
};





#define REG_BG0CNT					*((vu16 *)(BASE + 0x08)) //! \def REG_BG0CNT \brief Background 0 control register.
#define REG_BG1CNT					*((vu16 *)(BASE + 0x0a)) //! \def REG_BG1CNT \brief Background 1 control register.
#define REG_BG2CNT					*((vu16 *)(BASE + 0x0c)) //! \def REG_BG2CNT \brief Background 2 control register.
#define REG_BG3CNT					*((vu16 *)(BASE + 0x0e)) //! \def REG_BG3CNT \brief Background 3 control register.


#define	REG_BG0HOFS					*((u16 *)(BASE + 0x10))	// BG 0 H Offset
#define	REG_BG0VOFS					*((u16 *)(BASE + 0x12))	// BG 0 V Offset

#define	REG_BG1HOFS					*((u16 *)(BASE + 0x14))	// BG 1 H Offset
#define	REG_BG1VOFS					*((u16 *)(BASE + 0x16))	// BG 1 V Offset

#define	REG_BG2HOFS					*((u16 *)(BASE + 0x18))	// BG 2 H Offset
#define	REG_BG2VOFS					*((u16 *)(BASE + 0x1a))	// BG 2 V Offset

#define	REG_BG3HOFS					*((u16 *)(BASE + 0x1c))	// BG 3 H Offset
#define	REG_BG3VOFS					*((u16 *)(BASE + 0x1e))	// BG 3 V Offset


typedef u16 SCR_ENTRY, 				SE;
typedef SCR_ENTRY					SCREENBLOCK[1024];


typedef struct 						{ u32 data[8];  } TILE, TILE4;
typedef struct 						{ u32 data[16]; } TILE8;

typedef TILE						CHARBLOCK[512];
typedef TILE8						CHARBLOCK8[256];
//---------------------------------------------------------------------------

void SetCamera();
extern int palData[16];
extern const unsigned short fileSelectDim[16];
extern int palIndexesT[16];

extern TileSet tileset[10];
extern GameData game;
extern LevelData level;
extern MapLayer lyr[5];
extern Entity ent[20];

inline int GetCoord(int x, int y){ return x + (32 * y);}

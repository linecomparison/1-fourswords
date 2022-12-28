/*---------------------------------------------------------------------------------

	Header file for video registers

	Copyright 19XX-2020 by Knightsurfer.

	A cleanup project to make a library similar to libgba but not so cluttered.

---------------------------------------------------------------------------------*/
/*! \file video.h
    \brief video variables go here.

*/
//---------------------------------------------------------------------------------
#ifndef _video_h_
#define _video_h_	

#include "libraries/macros.h"


extern int palContain[132];


typedef struct {
	vu16 x;
	vu16 y;
} bg_scroll;
		
//--------------------------------------------------------------------------------
//Used mostly to control the VRAM, 
//ADD the values together to enable/disable features.
//
// BACKGROUNDS TO EQUIP SETTINGS TO.
//
//---------------------------------------------------------------------------------------
#define REG_DISPCNT		    		*(vu32*)(BASE+0x0000)	// Register Display Controller, basicly it tell the gba what videomode and background to use. \param usage REGDISCNT = MODE | BACKGROUND
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//
// MODES TO CHOOSE FROM.
//
//---------------------------------------------------------------------------------------
#define MODE_0				     	0x0000	//!< Mode 0; bg 0-4: reg
#define MODE_1			    		0x0001	//!< Mode 1; bg 0-1: reg; bg 2: affine
#define MODE_2			    		0x0002	//!< Mode 2; bg 2-2: affine
#define MODE_3			    		0x0003	//!< Mode 3; bg2: 240x160\@16 bitmap
#define MODE_4			   	 		0x0004	//!< Mode 4; bg2: 240x160\@8 bitmap
#define MODE_5			    		0x0005	//!< Mode 5; bg2: 160x128\@16 bitmap
//---------------------------------------------------------------------------------------
//
// DIFFERENT LAYER OPTIONS TO TOGGLE.
//
//---------------------------------------------------------------------------------------
#define BG0		        			0x0100	//!< Enable bg 0
#define BG1   						0x0200	//!< Enable bg 1
#define BG2			    			0x0400	//!< Enable bg 2
#define BG3  						0x0800	//!< Enable bg 3
#define OBJ      					0x1000	//!< Enable objects

#define OBJ_2D	      				0x0000	//!< OBJ-VRAM as matrix
#define OBJ_1D   					0x0040	//!< OBJ-VRAM as array
#define BLANK   					0x0080	//!< Force screen blank

#define WIN0     					0x2000	//!< Enable window 0
#define WIN1     					0x4000	//!< Enable window 1
#define WINOBJ	     				0x8000	//!< Enable object window
//------------------------------------------------------------------------
#define BGCTRL						((vu16 *)(BASE + 0x08)) //! \def BGCTRL \brief Array definition for background control registers. \param BGCTRL[0] references background 0 control register. \param BGCTRL[1] references background 1 control register. 	\param BGCTRL[2] references background 2 control register. \param BGCTRL[3] references background 3 control register.
#define BG_OFFSET 					((bg_scroll *) (BASE + 0x10))

//-----------------------------------------------------------------------
#define	REG_WIN0H					*((vu16 *)(BASE + 0x40))
#define	REG_WIN1H					*((vu16 *)(BASE + 0x42))
#define	REG_WIN0V					*((vu16 *)(BASE + 0x44))
#define	REG_WIN1V					*((vu16 *)(BASE + 0x46))
#define	REG_WININ					*((vu16 *)(BASE + 0x48))
#define	REG_WINOUT					*((vu16 *)(BASE + 0x4A))
//-----------------------------------------------------------------------
#define MODE3_FB 					((MODE3_LINE *)0x06000000)
#define M3_WIDTH					SCREEN_WIDTH
#define M3_HEIGHT					SCREEN_HEIGHT

#define M4_WIDTH					SCREEN_WIDTH
#define M4_HEIGHT					SCREEN_HEIGHT


#define MODE5_FB 					((MODE5_LINE *)0x06000000)
#define MODE5_BB 					((MODE5_LINE *)0x0600A000)

#define M5_WIDTH					160
#define M5_HEIGHT					128

#define VRAM_BACK	    			(VRAM + PAGE_SIZE) //!\def VRAM BACK \brief Base address of back video ram for page flipping.
#define vid_mem						((COLOR*)VRAM)

#define vid_mem_front				((COLOR*)VRAM)
#define vid_mem_back				((COLOR*)VRAM_BACK)
//=======================================================================
#define	SPRITE_PALETTE	    		((u16 *)0x05000200)		// Sprite color table
#define OAM_HBL      				0x0020					//!< Allow OAM updates in HBlank
#define VRAM_OBJ					(VRAM + VRAM_BG_SIZE) 	//The area of memory that stores sprites.
//-----------------------------------------------------------------------

//=======================================================================
#define SE_HFLIP					0x0400	//!< Horizontal flip
#define SE_VFLIP					0x0800	//!< Vertical flip
//-----------------------------------------------------------------------
#define SCREENBUFFER 				((u16*)(VRAM)) //I Forget
//-----------------------------------------------------------------------
#define ATTR1_HFLIP					0x1000	//!< Horizontal flip (reg obj only)
#define ATTR1_VFLIP					0x2000	//!< Vertical flip (reg obj only)
//-----------------------------------------------------------------------
typedef struct 						{ u32 data[8];  } TILE, TILE4;
typedef struct 						{ u32 data[16]; } TILE8;
typedef TILE						CHARBLOCK[512];
typedef TILE8						CHARBLOCK8[256];
//-----------------------------------------------------------------------			
#define tile8_mem					((CHARBLOCK8*)VRAM)
#define tile_mem_obj				((CHARBLOCK*)VRAM_OBJ)
#define tile8_mem_obj				((CHARBLOCK8*)VRAM_OBJ)
//-----------------------------------------------------------------------

//=======================================================================
#define	REG_MOSAIC					*(vu16 *)(BASE + 0x4c)
#define	REG_BLDCNT					*((vu16 *)(BASE + 0x50))
#define	REG_BLDALPHA				*((vu16 *)(BASE + 0x52))
#define	REG_BLDY					*((vu16 *)(BASE + 0x54))
//=======================================================================
#define REG_VCOUNT	   				*(vu16*)(BASE+0x0006)	// vertical count

//=======================================================================

//***************************************************************************************
//	======	|		|	|\    |	  ----	--------- ----------	 -------	|\    | s
//	|		|		|	| \   |	 |			|		  |			|		|	| \   |
//	|====	|		|	|  \  |	|			|		  |			|		|	|  \  |
//	|		|		|	|   \ |	|			|		  |			|		|	|   \ |
//	|		 |_____|	|    \|	 |_____		|	  ____|______	 -------	|    \|
//			  					  
//****************************************************************************************



//=========================================================================================================
// LAYER STUFF
//=========================================================================================================
void LoadMap(short unsigned int* ground,short unsigned int* overhead);

void SetMode(int mode);
void ToggleObj(bool enable); //Toggle OBJ.
void ToggleBackGround(int BGNo, bool enable); //Toggle Layers.
void SetupLayers(); //Setup a layer.

void SetCamera();
void MoveCamera(u32 scrollForce);

void Fill(int tileID,int tileSetNo,int pal,int layer);
void DrawTile(int x, int y, int tileID,int tileSetNo,int flipped,int pal,int layer);
void DrawFrame(int posX, int posY, int width, int height, int pal,int layer,bool overlap);
void DrawRect(int posX,int posY,int width, int height, int tileID,int tileSetNo, int pal,int layer);

void AddRoom(int type);
void AddDoor(char Orientation,int x,int y);

void RoomPlan(int type,short unsigned int* cont);
void DoorPlan(int x, int y, char orientation, unsigned short* ground, unsigned short* overhead);

void DrawChar(int posX, int posY, char letter,int pal,int layer);
void DrawText(int posX, int posY, char* message, int pal, int layer);

void Fade();
void DrawKeyboard(bool caps);
//=========================================================================================================
// MISC STUFF
//=========================================================================================================

COLOR RGB15(u32 red, u32 green, u32 blue);
COLOR GetPalAdd(int paletteNo,int add);
COLOR GetPalDiv(int paletteNo,int div);
int GetCoord(int x, int y);

u16 *vid_flip();
u32 VCOUNT( int m);

char* LoadLocation(int index);

void vid_vsync();



//=========================================================================================================
// TILE MEMORY STUFF
//=========================================================================================================


void AddPaletteT(int pal);
void AddTileset(int tileSet,int charBase);


//=========================================================================================================
// SPRITE STUFF
//=========================================================================================================


OBJ_ATTR *SetSpriteAttributes(OBJ_ATTR *obj, u16 a0, u16 a1, u16 a2, u16 a2p);

void AddPaletteS(int pal);

void InitSprite(OBJ_ATTR *obj, u32 count);
void CopySprite(OBJ_ATTR *dst, OBJ_ATTR *src, u32 count);

void HideSprite(OBJ_ATTR *obj);
void UnhideSprite(OBJ_ATTR *obj, u16 mode);

void MoveSprite(OBJ_ATTR *obj, int x, int y);
void UpdateSprites();

void UpdateSprites();
void AnimateEntity();

void ChangePalette(int entNo, int pal);
void ScrollPalette(bool positive, int entNo);

void ChangeFrame(int entNo, int frameNo);
void ScrollFrame(bool positive,int entNo);

void UpdateSpriteFrame(int entNo,int frame);
void ResetPlayerAnim();

void GetSpriteAttr(int sprite,int entNo);
void LoadSpritesToMemory();

void RegisterEnt(int sprite, int pal, int pos_x, int pos_y, int layer,int frame, int state);
void AssignSprites();

void SpriteManager();

//=========================================================================================================
// BITMAP STUFF
//=========================================================================================================

typedef u16 MODE5_LINE[160];
typedef u16 MODE3_LINE[240];





#endif // _video_h
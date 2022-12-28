/*---------------------------------------------------------------------------------

	Header file for base registers.

	File by Knightsurfer.

	A cleanup project that combines libtonc and libgba
	to make a library similar to libgba but not so cluttered.

---------------------------------------------------------------------------------*/
/*! \file  base.h
    \brief base registers.
*/
//---------------------------------------------------------------------------------
#ifndef _base_h_
#define _base_h_
//---------------------------------------------------------------------------------
#include "libraries/types.h"
//---------------------------------------------------------------------------------
//Macros used to shorthand addresses.
#define EWRAM   0x02000000 //!\def EWRAM \brief Base address of gba external work ram.
#define IWRAM   0x03000000 //!\def IWRAM \brief Base address of gba internal work ram.
#define IO		0x04000000 //!\def REG_BASE \brief Base address of gba hardware registers.
#define PAL	    0x05000000
#define VRAM    0x06000000 //!\def VRAM \brief Base address of gba video ram.
#define OAM		0x07000000 // The address for controling sprites. 
#define PAKROM 	0x08000000 // The address for the game pack.

#define CARTFLASHBANK1 0x08000000
#define CARTFLASHBANK2 0x0A000000
#define CARTFLASHBANK3 0x0C000000


#define CARTRAM	0x0E000000 // SRAM memory Adress

//=======================================
//      PAL Memory
//=======================================
#define pal_bg_bank		((PALBANK*)PAL)         
#define pal_bg_mem      ((COLOR*)PAL)           			// pal_bg_mem[palNo] = colour

#define pal_obj_bank	((PALBANK*)(PAL + PAL_BG_SIZE))
#define pal_obj_mem		((COLOR*)(PAL + PAL_BG_SIZE))   	// pal_obj_mem[0] = colour //The area of memory that stores the palettes of sprites.

//=======================================
//      Tile Memory
//=======================================
#define tile_mem        ((CHARBLOCK*)VRAM)      			// til_mem[charblock][tileID] = Tile
#define tile_mem_obj    ((CHARBLOCK*)VRAM_OBJ)  			// til_mem_obj[0][tileID] = Tile
//=======================================
//      Layer Memory
//=======================================
#define se_mem			((SCREENBLOCK*)VRAM)  // )(se_mem[layer][(y * width + x) ] = palette + (flipped) tileID) -- Basicly it allows you to plot tiles to background layers.
//=======================================
//      OBJ Memory
//=======================================
#define obj_mem			((OBJ_ATTR*)OAM)
#define obj_aff_mem		((OBJ_AFFINE*)OAM)

#endif
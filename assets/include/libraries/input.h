/*---------------------------------------------------------------------------------

	Header file for input.

	File by Knightsurfer.

	A cleanup project that combines libtonc and libgba
	to make a library similar to libgba but not so cluttered

---------------------------------------------------------------------------------*/
/*! \file  input.h
    \brief input definitions.
    */
//---------------------------------------------------------------------------------
#include "base.h"
//---------------------------------------------------------------------------------
#ifndef _input_h_
#define _input_h_
//---------------------------------------------------------------------------------

//An enum of key indexes.
enum eKeyIndex{
    KI_A=0, KI_B, KI_SELECT, KI_START, 
    KI_RIGHT, KI_LEFT, KI_UP, KI_DOWN,
    KI_R, KI_L, KI_MAX
};


extern u16 __key_curr, __key_prev;

#define REG_KEYINPUT	*(vu16*)(BASE + 0x130)  // Key Input
#define KEY_DOWN_NOW(key)	(~(REG_KEYINPUT) & key)

#define KEY_A           0x0001 //The A button.
#define KEY_B           0x0002 //The B button.
#define KEY_SELECT      0x0004 //The Select button.
#define KEY_START       0x0008 //The Start button.
#define KEY_RIGHT       0x0010 //The Directional Right button.
#define KEY_LEFT        0x0020 //The Directional Left button.
#define KEY_UP          0x0040 //The Directional Up button.
#define KEY_DOWN        0x0080 //The Directional Down button.
#define KEY_R           0x0100 //The R button.
#define KEY_L           0x0200 //The L button.

#define KEY_ANY			0x03FF	//Where's the ANY key?
#define KEY_DIR			0x00F0	//any-dpad
#define KEY_ACCEPT		0x0009	//A or start
#define KEY_SHOULDER	0x0300	//L or R
#define KEY_RESET		0x000F	//St+Se+A+B

u32 key_hit(u32 key);  
u32 key_is_down(u32 key);
u32 key_is_up(u32 key);
u32 key_was_down(u32 key);
u32 key_was_up(u32 key);

int key_tri_horz();
int key_tri_vert();
int key_tri_shoulder();
int key_tri_fire();

u32 key_transit(u32 key);
u32 key_held(u32 key);
u32 key_released(u32 key);

bool key_tribool(u32 minus, u32 plus);
bool DIR_Detection();
void NavigateMenu();
void Reset();
void AnimatePlayer();
void ChangeCharacter(u32 key);
void MovePlayer();
void Pause();
void MyMenu(u32 input);

void key_poll();

//=======================================================
#endif //_input_h_
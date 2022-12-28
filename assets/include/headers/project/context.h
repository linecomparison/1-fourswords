#include "libraries/structs.h"
#include "libraries/enums.h"
#include "libraries/input.h"
#include "libraries/video.h"


extern int trackNo;
extern int menuItem;
extern int menuItems;
extern int resetTick;
extern int dialogDirection;
//extern bool animationPlaying;
extern LevelData level;
extern GameData game;
extern Entity ent[20];
extern FileData file[2];
extern MapLayer lyr[5];

extern bool key_capitals;
void PlaySound(int sound);
void ResetVariables();
void CreateFile();



//A macro of sorts that quickly adds the fourswords palettes for Link sprites.
//zelda - context.c
inline void AddLinkPalettes(){
	AddPaletteS(0);
	AddPaletteS(1);
	AddPaletteS(2);
	AddPaletteS(3);
	AddPaletteS(4);
}

//map names that go with the index.
const char* mapNames[3] = { 
   "HOUSE",
   "CAVE",
   "ERROR" 
};
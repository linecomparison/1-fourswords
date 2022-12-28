#include "libraries/structs.h"
#include "libraries/input.h"
#include "libraries/types.h"
#include "libraries/enums.h"
#include "libraries/video.h"

void AnimatePlayer();
void ResetPlayer();
void MovePlayer();

void HardReset();
void PlaySound(int soundID);
void ChangeCharacter(u32 resetKey);


extern GameData game;
extern LevelData level;
extern Entity ent[20];
extern FileData file[2];
void ConstructMap(int type,int map);
void ScrollPalette(bool positive, int entNo);

bool key_capitals;  //Checks if the keyboard mode is set to capitals or not.
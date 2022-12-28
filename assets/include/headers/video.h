#include "libraries/size_defines.h"
#include "libraries/structs.h"
#include "libraries/enums.h"
#include "libraries/input.h"
#include "libraries/base.h"
#include "libraries/video.h"
#include "libgba/maxmod.h"
#include "resources/tileSets.h"

extern MapLayer lyr[5];
extern TileSet tileset[10];
extern Entity ent[20];
extern GameData game;
extern LevelData level;
extern FileData file[2];

extern int trackNo;
extern const unsigned int *s_select;

void SetLevel(int index,int entrance);
void ConstructMap(int type,int map);
void LoadLevel();



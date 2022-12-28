#include "libraries/enums.h"
#include "libraries/input.h"
#include "resources/customResources.h"
#include "libraries/structs.h"
#include "libraries/data.h"
#include "libraries/video.h"

extern GameData game;
extern LevelData level;
extern Entity ent[20];
void MoveCamera(u32 scrollForce);
extern int palContain[132];

void SetLevel(int map, int entrance);
void ConstructMap(int type,int map);
void LoadTilesets();
void SoundManager();
void InputManager(void customMenu());
void UpdateSprites();

void SetCamera();
#include "libgba/interrupts.h"
#include "libraries/input.h"
#include "libraries/systemcalls.h"
#include "libraries/enums.h"
#include "libraries/structs.h"

void key_poll();
void mmVBlank( void );
void GameManager();
void Fade();

void InitFiles();
extern GameData game;
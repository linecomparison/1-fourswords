#include "headers/data.h"

FileData    file[2];       //A stuct containing file variables.
Entity      ent[20];       //Entity data, stores parameters for each sprite.
MapLayer    lyr[5];        //Layer variables.
TileSet     tileset[10];   //Tileset variables.
GameData    game;          //Global variables for all the game's main uses.
LevelData   level;         //Level variables for use only under the loaded map.

//=========================================================================================================
// WRITING
//=========================================================================================================

//erase the contents of all potential files and then
//draw a pretty table for easy viewing in a hex editor. 
void EraseSave(int saveFile){ 
   if(saveFile == -1) for(int i = 0; i < 600; i++) SaveData[i] = 0xFF;
   else for(int row = 0; row < 31; row++)for(int i = 0; i < 9; i++){
      SaveData[i + (10 * row) + (310 * saveFile)] = 0xFF;
      SaveData[9 + (10 * row) + (310 * saveFile)] = '|';
      SaveData[0 + (10 * row) + (310 * saveFile)] = '|';
      if(i > 0){
         if(row == 1)SaveData[i +  (10 * row) + (310 * saveFile)] = '_';
         if(row == 24)SaveData[i + (10 * row) + (310 * saveFile)] = '_';
         if(row == 30)SaveData[i + (10 * row) + (310 * saveFile)] = '_';
      }
   }
}

void InitSave(){
   EraseSave(-1);
   EraseSave(0);
   EraseSave(1);
}

//if the files aren't initialised, 
//do so by filling in this template.
void InitFiles(){ 
   InitSave(0); 
   const char* message;
   if(cartType == cartFLASH){
      message = "LINK";   
      for(int i = 0; i < 9; i++)
      if(i < sizeof(message) && message[i] != ' ') {SaveData[i] = message[i];}
      else SaveData[i] = ' ';
   
      SaveData[dataPlayerPal] = 0;
      SaveData[dataMap] = 1;
      SaveData[dataInit] = 1;
   }
}

//Saves info about all currently loaded entities.
void SaveEntities(){
   for(int i = 0; i != level.entNo; i++){ 
		//SaveData[20 * (i) + dataPlayerPal +  (310 *  game.fileNo)] = ent[i].palette;
      SaveData[20 * (i) + dataEntX  +      (310 *  game.fileNo)] = ent[i].global_x;
		SaveData[20 * (i) + dataEntY  +      (310 *  game.fileNo)] = ent[i].global_y;
		SaveData[20 * (i) + dataEntD  +      (310 *  game.fileNo)] = ent[i].currentDirection;
	}
}

void CreateFile(){
   SaveData[dataMap +         (310 * game.fileNo)] = file[game.fileNo].currentMap;
   for(int i = 0; i < 8; i++) SaveData[dataName + (310 * game.fileNo) + i] = file[game.fileNo].fileName[i];
   SaveData[dataInit +        (310 * game.fileNo)] = file[game.fileNo].init;
   SaveData[dataPlayerPal +   (310 * game.fileNo)] = file[game.fileNo].playerPal;
}

//Pause menu save function.
void Save(){
	SaveEntities();
	SaveData[dataMap + 310 * game.fileNo] = game.currentMap;
	SaveData[dataPlayerPal + (310 * game.fileNo)] = ent[0].palette;
}
//=========================================================================================================
// READING
//=========================================================================================================

//Check the files if they have been initialised and if so, 
//proceeds to read the filename.
void LoadFiles(){ 
   const char* newGame = "NEW GAME";

   for(int fileNo = 0; fileNo < 2; fileNo++){
      if(SaveData[dataInit + (310 * fileNo)] == 1) file[fileNo].init  = true;
      for(int nameIndex = 0; nameIndex != 8; nameIndex++) file[fileNo].fileName[nameIndex] = SaveData[dataName + 5 + (310 * fileNo) + nameIndex];
      if(!file[fileNo].init) file[fileNo].playerPal = 4;
      else {
         file[fileNo].playerPal = SaveData[dataPlayerPal + (310 * fileNo)];
         file[fileNo].currentMap = SaveData[dataMap + (310 * fileNo)];
      }
   }
   for(int df = 0; df != 2; df++){
      for(int i = 0; i != 8; i++){ 
         if(file[df].init) file[df].fileName[i] = SaveData[dataName + i + (df * 310)];
         else file[df].fileName[i] = (u8)newGame[i];
      }
   }
}

//Load basic sprite data.
void LoadEntities(){
	for(int i = 0; i != level.entNo; i++)
	{
		ent[i].global_x = SaveData[dataEntX + (10 * game.fileNo) + (20 * i)];
		ent[i].global_y = SaveData[dataEntY + (10 * game.fileNo) + (20 * i)];

		ent[i].currentDirection = (char)SaveData[dataEntD + (10 * game.fileNo) + (20 * i)];

		if(ent[i].currentDirection  == 'r'){ 
			ent[i].sprite->attr1 ^= ATTR1_HFLIP; 
			ent[i].flippedX = true;
		}
	}
}

//Returns the name of the location in the form of a string.
char* LoadLocation(int index){
   if(index < sizeof(mapNames)) return (char*)mapNames[index -1];
   else return (char*)mapNames[2];
}

//=========================================================================================================
// Variables
//=========================================================================================================

//Sets the default values on startup such as layers and input values.
//Located in: functions - data.c
void DefaultValues(){
   game.inputType = inputMenu;
   SetupLayers();
	ToggleObj(true);
   for(int i = 0; i != 3; i++) ToggleBackGround(i,true);
   menuItems = 1;
}

//Resets most essential variables.
void ResetVariables(){
   for(int i = 0; i != 4; i++) playerTaken[i] = false;

   level.entNo = 0;
   game.tilesCount = 0;
   level.tilePalCount = 0;
   level.spritePalCount = 0;
	level.tileSetCount = 0;
   for(int i = 0; i < 4; i++){ 
      lyr[i].scrollY = 0;
      lyr[i].scrollX = 0;
      SetCamera();
   }
   game.init[Sprites] = false;
}

//Reset areas of RAM.
void ResetMemory(int type){
    switch(type)
    {
      case 0:
      RegisterRamReset(RESET_EWRAM);
      break;

      case 1:
      RegisterRamReset(RESET_IWRAM);
      break;

      case 2:
      RegisterRamReset(RESET_PALETTE);
      break;

      case 3:
      RegisterRamReset(RESET_VRAM);
      break;

      case 4:
      RegisterRamReset(RESET_OAM);
      break;

      case 5:
      RegisterRamReset(RESET_SIO);
      break;  

      case 6:
      RegisterRamReset(RESET_SOUND);
      break;

      case 7:
      RegisterRamReset(RESET_OTHER);
      break;

      case 8:
      SoftReset(0);
      break;

    }
}

//Resets everything in RAM
void HardReset(){
   ResetMemory(2);
   ResetMemory(3);
   ResetMemory(4);
   ResetMemory(8);
}

//Sets level variables such as the level's music,
//amount of supported players and number of bots.
//Located in: zelda - context.c
//TODO: Combine this with AddTilesets and AddPalettes.
void SetLevel(int index,int entrance){
    
    int posX = 0,posY = 0;
    
    switch(index){
        default:
        trackNo = -1;
		level.plrNo = 1;
		level.botNo = 1;
        break;
		
		case 1:
		trackNo = -1;
		level.plrNo = 1;
		level.botNo = 1;
		level.mapWidth = 50;
        level.mapHeight = 96;

		level.screensX = 1;
		level.screensY = 1;
		break;

		case 2:
		trackNo = 1;
		level.plrNo = 1;
		level.botNo = 1;
		level.mapWidth = 50;
        level.mapHeight = 96;

		level.screensX = 1;
		level.screensY = 1;
		break;
    }
    ent[0].global_x = posX;
    ent[0].global_y = posY;
}


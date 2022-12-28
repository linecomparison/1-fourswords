#include "headers/video.h"

int palData[16];

int palIndexesT[16];
int palIndexesS[16];

int videoMode;
bool BG[4]; 						//Background toggles that should probably be universal.
bool OBJTOGGLE; 					//OBJ toggles that should probably be universal.

//=========================================================================================================
// TILE MEMORY STUFF
//=========================================================================================================

void LoadTilesets();

void GetPaletteT(int pal);


//=========================================================================================================
// SPRITE STUFF
//=========================================================================================================

void GetPaletteS(int pal);


//=========================================================================================================
// MISC STUFF
//=========================================================================================================

//The new and improved palette grabber
//allows one to convert a short to an RGB 
//value on the fly and then reduce it to a fraction of your
//choosing to dim or brighten a palette.
//\param paletteNo which palette index to choose from.
//\param div divide this colour by whichever number.
COLOR GetPalData(int paletteNo,int div){
	int math[2];
	int rgb[3];

	math[0] = palData[paletteNo];
    math[1] = math[0] >> 10;
    rgb[2]  = math[1];
    math[0] = math[0] - (1024 * rgb[2]);
    math[1] = math[0] >> 5;
    rgb[1]  = math[1];
    math[0] = math[0] - (32 * rgb[1]);
	rgb[0]  = math[0];

	return RGB15(rgb[0]/div,rgb[1]/div,rgb[2]/div);
}

//! \def VCOUNT \brief  Test
u32 VCOUNT(int m) { return m<<8; } 

//A colour bariable,allows you to convert a rgb value to a 16 bit hex colour.
COLOR RGB15(u32 red, u32 green, u32 blue) {	return red | (green<<5) | (blue<<10);}	

//A while loop that syncs the game up to 30 frames per second instead of computer cycles.
void vid_vsync(){
	while(REG_VCOUNT >= 160);	// wait till VDraw
	while(REG_VCOUNT < 160);	// wait till VBlank
}

//Allows you to set the mode on the fly.
void SetMode(int mode)	{
	REG_DISPCNT = BLANK;
	videoMode = mode;
} 

//Toggles background layers.
void ToggleBackGround(int BGNo,bool enable){
	BG[BGNo] = enable;

	REG_DISPCNT = videoMode;

	if(BG[0]) REG_DISPCNT += BG0;
	if(BG[1]) REG_DISPCNT += BG1;
	if(BG[2]) REG_DISPCNT += BG2;
	if(BG[3]) REG_DISPCNT += BG3;
	if(OBJTOGGLE) REG_DISPCNT += OBJ + OBJ_1D;
	else return;
}

//My attempt at fade effects.
void Fade(){
	if(game.fadeOut){
		game.timer[Fading]++;		
		if(game.timer[Fading] == 6){
			game.tick[Fading]++;
			game.timer[Fading] = 0;
			
			if(game.tick[Fading] <  5){ 
				for(int p = 0; p < level.tilePalCount; p++){ 
					GetPaletteT(palIndexesT[p]);
					for(int i = 0; i < 16; i++){
						pal_bg_mem[i + (16 * p)] = palData[i] - (GetPalData(i,4) * game.tick[Fading]);
					}
				}
				for(int p = 0; p < level.spritePalCount; p++){ 
					GetPaletteS(palIndexesS[p]);
					for(int i = 0; i < 16; i++){
						pal_obj_mem[i + (16 * p)] = palData[i] - (GetPalData(i,4) * game.tick[Fading]);
					}
				}

			}
			if(game.tick[Fading] == 5){
				game.faded[0] = true; 
				for(int i = 0; i < 16 * level.tilePalCount; i++) pal_bg_mem[i] = 0;
				for(int i = 0; i < 16 * level.spritePalCount; i++) pal_obj_mem[i] = 0;
				UpdateSprites();
				mmStop();
			}
			if(game.tick[Fading] == 6){
				game.fadeOut = false;
				game.fadeIn = true;
				game.tick[Fading] = 0;
				for(int i = 0; i < 4; i++)Fill(0,0,0,i);
				LoadTilesets();
				LoadLevel();
			}
		}
	}		
	if(game.fadeIn){
		game.timer[Fading]++;	
		if(game.timer[Fading] == 6){	
			game.tick[Fading]++;
			game.timer[Fading] = 0;
			
			if(game.tick[Fading] < 5) { 
			
				for(int p = 0; p < level.tilePalCount; p++){ 
					GetPaletteT(palIndexesT[p]);
					for(int i = 0; i < 16; i++){
						pal_bg_mem[i + (16 * p)] = 0 + (GetPalData(i,4) * game.tick[Fading]);
					}
				}				
				for(int p = 0; p < level.spritePalCount; p++){ 
					GetPaletteS(palIndexesS[p]);
					for(int i = 0; i < 16; i++){
						pal_obj_mem[i + (16 * p)] = 0 + (GetPalData(i,4) * game.tick[Fading]);
					}
				}				
			}
			if(game.tick[Fading] == 5) {
				
				for(int p = 0; p < level.tilePalCount; p++){ 
					GetPaletteT(palIndexesT[p]);
					for(int i = 0; i < 16; i++){
						pal_bg_mem[i + (16 * p)] = palData[i];
					}
				}
				for(int p = 0; p < level.tilePalCount; p++){ 
					GetPaletteS(palIndexesS[p]);
					for(int i = 0; i < 16; i++){
						pal_obj_mem[i + (16 * p)] = palData[i];
					}
				}
				if(trackNo != -1) mmStart( trackNo, MM_PLAY_LOOP );}
			if(game.tick[Fading] == 6){
				game.fadeIn = false; 
				game.tick[Fading] = 0; 
				game.faded[0] = false;
				game.faded[1] = true; 
				if(game.currentMap != 0) game.inputType = inputPlayer;
				else game.inputType = inputMenu;	
			}
		}
	}
	if(game.faded[0] && !level.loaded){
		game.currentMap = file[game.fileNo].currentMap;
		SetLevel(game.currentMap,0);
		level.loaded = true;
	}		
}




#include "headers/v_spr-mapping.h"

OBJ_ATTR spr_buffer[20]; OBJ_AFFINE *spr_aff_buffer= (OBJ_AFFINE*)spr_buffer;  //A buffer that stores all the sprites in memory.

bool playerTaken[4]; 				//When a palette option is taken for player character, these will fire.

int pal = -1,spriteLayer,sprite,frame,state;
unsigned int s_size; 				//Stores the array size of the sprite.
unsigned int sp_size; 				//Stores the array size of the sprite palette.

const unsigned int *s_select; 		//Stores the sprite array being used.
const unsigned short  *sp_select; 	//Stores the sprite palette being used.

//--------------------------------------------------------------------------------------------------------
//	Misc
//--------------------------------------------------------------------------------------------------------

// Set the attributes of a sprite.
	// \param obj Chosen Sprite.
	// \param a0 Attribute 0.
	// \param a1 Attribute 1.
	// \param a2 Attribute 2.
	// \param prio Attribute 2, set layer priority.
	//
OBJ_ATTR *SetSpriteAttributes(OBJ_ATTR *obj, u16 a0, u16 a1, u16 a2, u16 a2p){
	obj->attr0= a0; obj->attr1= a1; obj->attr2= a2; obj->attr2 = a2p;
	return obj;
}

//Appararently can't just be enabled and disabled.
void ToggleObj(bool enable){
	if (enable == OBJTOGGLE) return;
	OBJTOGGLE = enable;
	
	REG_DISPCNT = videoMode;
	
	if(BG[0]) REG_DISPCNT += BG0;
	if(BG[1]) REG_DISPCNT += BG1;
	if(BG[2]) REG_DISPCNT += BG2;
	if(BG[3]) REG_DISPCNT += BG3;

	if(enable) REG_DISPCNT += OBJ + OBJ_1D;
	else return;
}

//Sets up a table of a set palette bank for use.
void GetPaletteS(int pal){
	switch(pal){
		
		case 0:
		for(int i = 0; i < 16; i++) palData[i] = p_linkg[i];
		break;

		case 1:
		for(int i = 0; i < 16; i++) palData[i] = p_linkr[i];
		break;

		case 2:
		for(int i = 0; i < 16; i++) palData[i] = p_linkb[i];
		break;

		case 3:
		for(int i = 0; i < 16; i++) palData[i] = p_linkp[i];
		break;

		case 4:
		for(int i = 0; i < 16; i++) palData[i] = p_linkd[i];
		break;

		case 5:
		for(int i = 0; i < 16; i++) palData[i] = cursorPal[i];
		break;
	}
}

//Adds a pallete to a table which can be referenced for later use.
void AddPaletteS(int pal){
	palIndexesS[level.spritePalCount] = pal;
	level.spritePalCount++;
}

//Allows you to scroll through your palettes banks.
void ScrollPalette(bool positive, int entNo){
	if(cartType != cartFLASH) return;

	if(positive)
	{
		if(ent[entNo].palette ==  4) ent[entNo].palette = 0;
		else ent[entNo].palette++;
	}
	else
	{
		if(ent[entNo].palette == 0) ent[entNo].palette = 4;
		else ent[entNo].palette--;
	}
}

//Allows you to scroll through your animation frames.
void ScrollFrame(bool positive,int entNo){
	if(!positive){
		if(ent[entNo].frame == ent[entNo].startFrame) ent[entNo].frame = 16 * (ent[entNo].plrSprites) + 1;
		else ent[entNo].frame--;
	}
	else{
		if(ent[entNo].frame > 16 * (ent[entNo].plrSprites)) ent[entNo].frame = ent[entNo].startFrame;
		else ent[entNo].frame++;
	}
}

//--------------------------------------------------------------------------------------------------------
//	Initiialisation of the sprite.
//--------------------------------------------------------------------------------------------------------

// Copies a target sprite for the purpose of buffering.
	// also handy for copying from a template.
	// \param dst Variable for a new sprite.
	// \param src The sprite array you want to copy to.
	// \param count Currently unknown.
void CopySprite(OBJ_ATTR *dst, OBJ_ATTR *src, u32 count){

	// NOTE: while struct-copying is the Right Thing to do here, 
	//   there's a strange bug in DKP that sometimes makes it not work
	//   If you see problems, just use the word-copy version.
	#if 1
		while(count--)
			*dst++ = *src++;
	#else
		u32 *dstw= (u32*)dst, *srcw= (u32*)src;
		while(count--)
		{
			*dstw++ = *srcw++;
			*dstw++ = *srcw++;
		}
	#endif
}

// Inialise a sprite.
	// \param obj chosen sprite in question.
	// \param count Currently unknown.
	//
void InitSprite(OBJ_ATTR *obj, u32 count){
	u32 nn= count;
	u32 *dst= (u32*)obj;

	// Hide each object
	while(nn--){
		*dst++= ATTR0_HIDE;
		*dst++= 0;
	}
	// init oam
	CopySprite(obj_mem, obj, count);
}

//Assigns Sprite Data to variables.
void GetSpriteAttr(int sprite,int entNo){
	switch(sprite){
		
		default:
        break;

    	case mainChar:
			s_select = linkTiles;
            s_size =  sizeof(linkTiles);
    
			ent[entNo].shape = ATTR0_SQUARE;
			ent[entNo].size = ATTR1_SIZE_16x32;
			ent[entNo].plrSprites = 35;
			ent[entNo].tile_size = 16;
			ent[entNo].frames = 15;

			ent[level.entNo].ani_walk_u 	= 27;
			ent[level.entNo].ani_walk_d 	= 8;
			ent[level.entNo].ani_walk_l 	= 17;
			ent[level.entNo].ani_walk_r 	= 17;
        break;

		case 1:
			s_select = cursorTiles;
            s_size =  sizeof(cursorTiles);
    
			ent[entNo].shape = ATTR0_SQUARE;
			ent[entNo].size = ATTR1_SIZE_32x32;
			ent[entNo].plrSprites = 1;
			ent[entNo].tile_size = 16;
			ent[entNo].frames = 1;

			ent[level.entNo].ani_walk_u 	= 0;
			ent[level.entNo].ani_walk_d 	= 0;
			ent[level.entNo].ani_walk_l 	= 0;
			ent[level.entNo].ani_walk_r 	= 0;
        break;
	}
}

//Initialises the set variables for an entity into memory.
void RegisterEnt(int sprite, int pal, int pos_x, int pos_y, int layer,int frame, int state){

	GetSpriteAttr(sprite,level.entNo); 
	UpdateSpriteFrame(level.entNo,frame);

	ent[level.entNo].frame = frame;
	ent[level.entNo].palette = pal;
	ent[level.entNo].state = state;	 
		 
		 if(ent[level.entNo].frame == 6) ent[level.entNo].currentDirection = 'u';
	else if(ent[level.entNo].frame == 0) ent[level.entNo].currentDirection = 'd';
	else if(ent[level.entNo].frame == 3) ent[level.entNo].currentDirection = 'l';
	else if(ent[level.entNo].frame == 3 && ent[level.entNo].flippedX) ent[level.entNo].currentDirection = 'r';

	ent[level.entNo].sprite 	= &spr_buffer[level.entNo];
	if(level.entNo != game.currentPlr){
		ent[level.entNo].global_x 	= pos_x;
		ent[level.entNo].global_y 	= pos_y;
	}

	if(level.entNo == 0){
		if(game.currentMap == 0){ 
			ent[level.entNo].local_x = 120;
			ent[level.entNo].local_y = 15;
		}
		else{
			ent[level.entNo].local_x = pos_x;
			ent[level.entNo].local_y = pos_y;
		}
	}

	if(level.entNo > 0){
		ent[level.entNo].local_x = pos_x;
		ent[level.entNo].local_y = pos_y;
	}

	ent[level.entNo].startFrame = ent[level.entNo].frame;
	ent[level.entNo].prio = layer;

	SetSpriteAttributes(ent[level.entNo].sprite, ent[level.entNo].shape,ent[level.entNo].size,ATTR2_PALBANK(ent[level.entNo].palette) | ent[level.entNo].frame,ATTR2_PRIO(layer));

	level.entNo++;
}

//Set the variables each entity will start with.
void AssignSprites(){
	
	int posX = 50; int posY = 50;

	if(game.currentMap == 0) playerTaken[file[0].playerPal] = true; else playerTaken[file[game.fileNo].playerPal] = true;
	for(int entNo = 0; entNo < level.plrNo + level.botNo; entNo++){
		
      	ent[entNo].animTimer = 0;
      	ent[entNo].animTick = 0;
      	ent[entNo].idleTick = 0;
      	ent[entNo].idleTimer = 0;
     	ent[entNo].context = 0;
		ent[entNo].state = state_idle;
		
		switch(game.currentMap){
			case fileMap:
				if(entNo == 0) {posX = 120; posY = 100; spriteLayer = 1; sprite = mainChar; frame = 8; ent[entNo].context = 2;}	
				if(entNo == 1) {posX = 29;  posY = 169;  spriteLayer = 4; sprite = 1; 		frame = 0; state = state_frozen; pal = 5; } 
			break;

			default:
				if(entNo < level.plrNo){
					if(		entNo == 0) {posX = 108; posY = 60; spriteLayer = 2; sprite = mainChar; frame = 0; state = state_idle;}
					else if(entNo == 1) {posX = 172; posY = 24; spriteLayer = 0; sprite = mainChar; frame = 2; state = state_frozen;}
					else if(entNo == 2) {posX = 156; posY = 24; spriteLayer = 2; sprite = mainChar; frame = 0; state = state_idle;}
					else if(entNo == 3) {posX = 140; posY = 24; spriteLayer = 2; sprite = mainChar; frame = 0; state = state_idle;}
				}
				else if(entNo >= level.plrNo){
					if(entNo == 1 + level.plrNo - 1) {posX = 120; posY = 120; spriteLayer = 2; sprite = mainChar; frame = 6; state = state_idle; pal = 4; }
					else if(entNo == 2 + level.plrNo - 1) {posX = 172; posY = 98; spriteLayer = 2; sprite = mainChar; frame = 6; state = state_idle; pal = 4;}
					else if(entNo == 3 + level.plrNo - 1) {posX = 156; posY = 98; spriteLayer = 2; sprite = mainChar; frame = 6; state = state_idle; pal = 4;}
					else if(entNo == 4 + level.plrNo - 1) {posX = 140; posY = 98; spriteLayer = 2; sprite = mainChar; frame = 6; state = state_idle; pal = 4;}
				}
				break;
			}
		if(sprite == mainChar && entNo < level.plrNo){	
			if(entNo == 0) pal = file[0].playerPal; else{
				for(int i = 0; i < 5; i++){
					if(!playerTaken[i]) {pal = i; playerTaken[i] = true; i = 5;}
				}
			}	  
		}

		RegisterEnt(sprite,pal,posX,posY,spriteLayer,frame,state);
	}
}

//--------------------------------------------------------------------------------------------------------
//	Animation
//--------------------------------------------------------------------------------------------------------

//Loads the chosen frame into memory
void UpdateSpriteFrame(int entNo,int frame){
	unsigned int spr[512];
	for(int i = 0; i != 512; i++) spr[i] = (unsigned int)s_select[i + (128 * frame)];
	memcpy(&tile_mem_obj[0][(ent[entNo].tile_size * entNo)], spr, 512);
}

//A state machine that manages all sprite animation.
void AnimateEntity(){

	for(int entityNo = 0; entityNo < level.entNo; entityNo++){
		ent[entityNo].animTimer++;
		if(ent[entityNo].animTimer > 2 ){
			ent[entityNo].animTick++;
			ent[entityNo].animTimer = 0;
		}

			if(ent[entityNo].state == state_idle){		
				if(ent[entityNo].context == 1){
					if(ent[entityNo].animTick == 1){
						if(ent[entityNo].frame > ent[entityNo].frames + 19) ent[entityNo].frame = ent[entityNo].ani_walk_u;
						else ent[entityNo].frame++;
						ent[entityNo].animTick = 0;
					}
					
				}
				else if(ent[entityNo].context == 2){
					if(ent[entityNo].animTick == 1){
						if(ent[entityNo].frame > ent[entityNo].frames) ent[entityNo].frame = ent[entityNo].ani_walk_d;				
						else ent[entityNo].frame++;		
						ent[entityNo].animTick = 0;
					}
				}
				else if(ent[entityNo].context == 3){
					if(ent[entityNo].animTick == 1){
						if(ent[entityNo].frame > ent[entityNo].frames + 10){
							ent[entityNo].frame =  ent[entityNo].ani_walk_l;
						}
						else ent[entityNo].frame++;     
						ent[entityNo].animTick = 0;
					}
				}
				else if(ent[entityNo].context == 4){
					if(ent[entityNo].animTick == 1){
						if(ent[entityNo].frame > ent[entityNo].frames + 10){
							ent[entityNo].frame = ent[entityNo].ani_walk_r;
						}
						else ent[entityNo].frame++;   
						ent[entityNo].animTick = 0;
					}
				}
				else if(ent[entityNo].context == 5){
					ent[entityNo].currentDirection = 'u';
					ent[entityNo].frame = 6;
					ent[entityNo].startAnim = 0;
				}
				else if(ent[entityNo].context == 6){
					ent[entityNo].currentDirection = 'd';
					ent[entityNo].frame = 0;
					ent[entityNo].startAnim = 0;
				}
				else if(ent[entityNo].context == 7){
					ent[entityNo].currentDirection = 'l';
					ent[entityNo].frame = 3;
					ent[entityNo].startAnim = 3;
				}
				else if(ent[entityNo].context == 8){
					ent[entityNo].currentDirection = 'r';
					ent[entityNo].frame = 3;
					ent[entityNo].startAnim = 3;
				}
				else if(ent[entityNo].currentDirection != 'u'){
					if(ent[entityNo].animTick > 4){	
						if(ent[entityNo].animTick == 38 + ent[entityNo].idleTick){
							ent[entityNo].frame++; 
							ent[entityNo].idleTick++;
						}
						if(ent[entityNo].idleTick == 3){
							ent[entityNo].frame = ent[entityNo].startAnim; 
							ent[entityNo].animTick = 0; 
							ent[entityNo].idleTick = 0;
						}
					}
				}
			}
			if(ent[entityNo].state == state_walking){
					switch(ent[entityNo].currentDirection){
						case 'u': ent[entityNo].startAnim = ent[entityNo].ani_walk_u; break;
						case 'd': ent[entityNo].startAnim = ent[entityNo].ani_walk_d; break;
						case 'l': ent[entityNo].startAnim = ent[entityNo].ani_walk_l; break;
						case 'r': ent[entityNo].startAnim = ent[entityNo].ani_walk_r; break;	
					}
				}		
	}
}

//Changes the palette of a given sprite.
void ChangePalette(int entNo,int palBank){
	ent[entNo].palette = palBank;
}

//Changes the frame of animation that is currently playing on a sprite.
void ChangeFrame(int entNo, int frameNo){
	ent[entNo].frame = frameNo;
}

//Ensures the player animations match up after resetting the entities.
void ResetPlayerAnim(){
	if(key_is_down(KEY_DIR)){
		ent[game.currentPlr].state = state_walking;
		ent[game.currentPlr].animTick = 0; 
		ent[game.currentPlr].animTimer = 0;
	
		if(key_is_down(KEY_UP)){	
			ent[game.currentPlr].currentDirection = 'u'; 
			ent[game.currentPlr].frame = ent[game.currentPlr].ani_walk_u;
		}
		else if(key_is_down(KEY_DOWN)){
			ent[game.currentPlr].currentDirection = 'd';
			ent[game.currentPlr].frame = ent[game.currentPlr].ani_walk_d;
		}
		else if(key_is_down(KEY_LEFT)){	ent[game.currentPlr].currentDirection = 'l';
			ent[game.currentPlr].frame = ent[game.currentPlr].ani_walk_l;
			if(ent[game.currentPlr].flippedX){
				ent[game.currentPlr].flippedX = false;
				ent[game.currentPlr].sprite->attr1 ^= ATTR1_HFLIP;
			}	
		}
			
		else if(key_is_down(KEY_RIGHT)){
			ent[game.currentPlr].currentDirection = 'r';
			ent[game.currentPlr].frame = ent[game.currentPlr].ani_walk_r; 
			if(!ent[game.currentPlr].flippedX){
				ent[game.currentPlr].flippedX = true;
				ent[game.currentPlr].sprite->attr1 ^= ATTR1_HFLIP;
			}
		}
			
		game.faded[1] = false;
	}
}

//--------------------------------------------------------------------------------------------------------
//	Movement
//--------------------------------------------------------------------------------------------------------

//Set the position of of a chosen sprite.
	// \param obj Chosen Sprite.
	// \param sprite_x x coordinate.
	// \param sprite_y y coordanite.
void MoveSprite(OBJ_ATTR *obj, int x, int y){
	BFN_SET(obj->attr0, y, ATTR0_Y);
	BFN_SET(obj->attr1, x, ATTR1_X);
}

//Update the frames, palette and position of every entity in play.
void UpdateSprites(){
	for(int i = 0; i < level.entNo; i++){
		UpdateSpriteFrame(i,ent[i].frame);
		ent[i].sprite->attr2= ATTR2_BUILD((ent[i].tile_size * i), ent[i].palette, ent[i].prio);
		MoveSprite(ent[i].sprite, ent[i].local_x, ent[i].local_y);
	}
	CopySprite(obj_mem, spr_buffer, level.entNo);
	AnimateEntity();
}

//--------------------------------------------------------------------------------------------------------
//	Visibility
//--------------------------------------------------------------------------------------------------------

//	Hide a sprite. 
	// \param obj Object to hide.
void HideSprite(OBJ_ATTR *obj){
	BFN_SET2(obj->attr0, ATTR0_HIDE, ATTR0_MODE);
}

//  Unhide a sprite. 
	//  \param obj	Object to unhide.
	//	\param mode	Object mode to unhide to. Necessary because this affects
	//	  the affine-ness of the object.
	//
void UnhideSprite(OBJ_ATTR *obj, u16 mode){
	BFN_SET2(obj->attr0, mode, ATTR0_MODE);
}

//--------------------------------------------------------------------------------------------------------
//	Manager
//--------------------------------------------------------------------------------------------------------

//Manages overall mechanics of the sprite functions.
void SpriteManager(){
	if(!game.init[Sprites]){ game.init[Sprites] = true;
		AssignSprites();
	}
	UpdateSprites();
}


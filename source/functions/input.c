#include "headers/input.h"

bool key_capitals;

int menuItem;
int menuItems;						
int pauseTimer;
int pauseTick;
int saveTimer;
int saveTick;

void Save();
int dialogDirection;
extern MapLayer lyr[5]; //bool animationPlaying;

//TONC CODE I DONT UNDERSTAND.
//Gets a boolean value out of an input.
int bit_tribool(u32 flags, int plus, int minus){ 
	/* Gives a tribool (-1, 0, or +1) depending on the state of some bits.
 * Looks at the \a plus and \a minus bits of \a flags, and subtracts 
 *	  their status to give a +1, -1 or 0 result. Useful for direction flags.
 *	\param plus		Bit number for positive result
 *	\param minus	Bit number for negative result
 *	\return	<b>+1</b> if \a plus bit is set but \a minus bit isn't<br>
 *	  <b>-1</b> if \a minus bit is set and \a plus bit isn't</br>
 *	  <b>0</b> if neither or both are set.
 */
	return ((flags>>plus)&1) - ((flags>>minus)&1);
} 

//variables for holding input states.
u16 __key_prev,__key_curr; 														//Variables for keeping track of button presses.

//returns whatever keys are currently being pressed.
u32 key_curr_state(){		return __key_curr;}

//returns whatever keys were previously pressed
u32 key_prev_state(){		return __key_prev;}

//Returns a boolean of wether the set key was pressed.
u32 key_hit(u32 key){		return (__key_curr&~__key_prev) & key;}

//Returns a boolean of wether the key is currently down.
u32 key_is_down(u32 key){	return  __key_curr & key;}							//Chosen button is down.

//Returns a boolean of wether the key is currently up.
u32 key_is_up(u32 key){		return ~__key_curr & key;}    						//Chosen button is up.

//Returns a boolean of wether the key was previously down.
u32 key_was_down(u32 key){	return  __key_prev & key;}   						//Chosen button was down.

//Returns a boolean of wether the key was previously released.
u32 key_was_up(u32 key){	return ~__key_prev & key;}    						//Chosen button was up.

//I dunno fully what this does but I think it's supposed to 
//return a boolean if the current key state was changed.
u32 key_transit(u32 key){ 	return ( __key_curr ^  __key_prev) & key;}			//Key is changing state.

//Haven't tested but it's supposed to return a boolean if the key has
// been held down for a duration.
u32 key_held(u32 key){ 		return ( __key_curr &  __key_prev) & key;}			//Key is held (down now and before).

//Returns a boolean if this key was just released.
u32 key_released(u32 key){	return (~__key_curr &  __key_prev) & key;}			//Key is being released (up now but down before)

//Not tested but it's supposed to return a boolean if left or right keys are down,
int key_tri_horz(){ 		return bit_tribool(__key_curr, KI_RIGHT, KI_LEFT);}	// right/left : +/-

//Not tested but it's supposed to return a boolean if up or down keys are down,
int key_tri_vert(){ 		return bit_tribool(__key_curr, KI_DOWN, KI_UP);}	// down/up : +/-

//Not tested but it's supposed to return a boolean if the left or right keys are down,
int key_tri_shoulder(){ 	return bit_tribool(__key_curr, KI_R, KI_L);}		// R/L : +/-
int key_tri_fire(){ 		return bit_tribool(__key_curr, KI_A, KI_B);}		// B/A : -/+

//checks wether one or the other key is being pressed and returns a bool value.
bool key_tribool(u32 minus,u32 plus) { 
	if(key_hit(minus)) return false; 
	if(key_hit(plus)) return true;
	return false;
}

//stores wether the directional pad is in use.
bool DIR_Detection(){
	if(key_is_up(KEY_DIR)) return true;
	else if(key_is_down(KEY_UP) ||
	   key_is_down(KEY_DOWN) ||
	   key_is_down(KEY_LEFT) ||
	   key_is_down(KEY_UP)) return false;
	return false;
} 

//basic menu navigation with sounds.
void NavigateMenu(){
	if(key_hit(KEY_DOWN)){
		if(menuItem > menuItems) menuItem = 0;
		else menuItem++;
		PlaySound(MenuCursor);
			
	} 
	if(key_hit(KEY_UP)){
		if(menuItem == 0) menuItem = menuItems + 1;
		else menuItem--;
		PlaySound(MenuCursor);
	}
}

//Checks the key input variables.
void key_poll(){
    __key_prev= __key_curr;
    __key_curr= ~REG_KEYINPUT & KEY_ANY;
}

//Trigger for your basic soft reset feature.
void Reset(){
	game.timer[Resets]++; 
	if(game.timer[Resets] == 10)
	{	
		game.timer[Resets] = 0;
		game.tick[Resets]++;
	}
	if(game.tick[Resets] > 0){
		if(key_is_down(KEY_A) && key_is_down(KEY_B))
		if(key_is_down(KEY_START) && key_hit(KEY_SELECT)) HardReset();
	}
}

//Handles the animation of the player.
void AnimatePlayer(){
	
	//===================================================================================
	//       Escape Manager
	//===================================================================================
	ent[game.currentPlr].context = 0;
	
		if(key_is_up(KEY_LEFT) 		&& key_is_up(KEY_RIGHT) && key_hit(KEY_DOWN)){									ent[game.currentPlr].currentDirection = 'd'; ent[game.currentPlr].frame = ent[game.currentPlr].ani_walk_d ;if(ent[game.currentPlr].flippedX){ent[game.currentPlr].sprite->attr1 ^= ATTR1_HFLIP; ent[game.currentPlr].flippedX = false;} ent[game.currentPlr].animTimer = 0; ent[game.currentPlr].animTick = 0;}
	else if(key_is_up(KEY_LEFT) 	&& key_is_up(KEY_RIGHT) && key_hit(KEY_UP)){ 									ent[game.currentPlr].currentDirection = 'u'; ent[game.currentPlr].frame = ent[game.currentPlr].ani_walk_u ;if(ent[game.currentPlr].flippedX){ent[game.currentPlr].sprite->attr1 ^= ATTR1_HFLIP; ent[game.currentPlr].flippedX = false;} ent[game.currentPlr].animTimer = 0; ent[game.currentPlr].animTick = 0;}
		
	else if(key_is_up(KEY_UP) 		&& key_is_up(KEY_DOWN) 	&& key_hit(KEY_LEFT)){									ent[game.currentPlr].currentDirection = 'l'; ent[game.currentPlr].frame = ent[game.currentPlr].ani_walk_l ;if(ent[game.currentPlr].flippedX){ent[game.currentPlr].sprite->attr1 ^= ATTR1_HFLIP; ent[game.currentPlr].flippedX = false;} ent[game.currentPlr].animTimer = 0; ent[game.currentPlr].animTick = 0;}
	else if(key_is_up(KEY_UP) 		&& key_is_up(KEY_DOWN) 	&& key_hit(KEY_RIGHT)){									ent[game.currentPlr].currentDirection = 'r'; ent[game.currentPlr].frame = ent[game.currentPlr].ani_walk_r ;if(!ent[game.currentPlr].flippedX){ent[game.currentPlr].sprite->attr1 ^= ATTR1_HFLIP; ent[game.currentPlr].flippedX = true;} ent[game.currentPlr].animTimer = 0; ent[game.currentPlr].animTick = 0;}
	
	else if(key_is_up(KEY_RIGHT) 	&& key_hit(KEY_UP) 		&& key_hit(KEY_LEFT)){									ent[game.currentPlr].currentDirection = 'u'; ent[game.currentPlr].frame = ent[game.currentPlr].ani_walk_u ;if(ent[game.currentPlr].flippedX){ent[game.currentPlr].sprite->attr1 ^= ATTR1_HFLIP; ent[game.currentPlr].flippedX = false;} ent[game.currentPlr].animTimer = 0; ent[game.currentPlr].animTick = 0;}
	else if(key_is_up(KEY_LEFT) 	&& key_hit(KEY_UP) 		&& key_hit(KEY_RIGHT)){									ent[game.currentPlr].currentDirection = 'u'; ent[game.currentPlr].frame = ent[game.currentPlr].ani_walk_u ;if(ent[game.currentPlr].flippedX){ent[game.currentPlr].sprite->attr1 ^= ATTR1_HFLIP; ent[game.currentPlr].flippedX = false;} ent[game.currentPlr].animTimer = 0; ent[game.currentPlr].animTick = 0;}

	else if(key_is_up(KEY_RIGHT) 	&& key_hit(KEY_DOWN) 	&& key_hit(KEY_LEFT)){									ent[game.currentPlr].currentDirection = 'd'; ent[game.currentPlr].frame = ent[game.currentPlr].ani_walk_d ;if(ent[game.currentPlr].flippedX){ent[game.currentPlr].sprite->attr1 ^= ATTR1_HFLIP; ent[game.currentPlr].flippedX = false;} ent[game.currentPlr].animTimer = 0; ent[game.currentPlr].animTick = 0;}
	else if(key_is_up(KEY_LEFT) 	&& key_hit(KEY_DOWN) 	&& key_hit(KEY_RIGHT)){									ent[game.currentPlr].currentDirection = 'd'; ent[game.currentPlr].frame = ent[game.currentPlr].ani_walk_d ;if(ent[game.currentPlr].flippedX){ent[game.currentPlr].sprite->attr1 ^= ATTR1_HFLIP; ent[game.currentPlr].flippedX = false;} ent[game.currentPlr].animTimer = 0; ent[game.currentPlr].animTick = 0;}

	else if(key_was_down(KEY_DOWN) 	&& key_is_up(KEY_DOWN) 	&& key_was_down(KEY_LEFT) 	&& key_is_down(KEY_LEFT)){	ent[game.currentPlr].currentDirection = 'l'; ent[game.currentPlr].frame = ent[game.currentPlr].ani_walk_l ;if(ent[game.currentPlr].flippedX){ent[game.currentPlr].sprite->attr1 ^= ATTR1_HFLIP; ent[game.currentPlr].flippedX = false;} ent[game.currentPlr].animTimer = 0; ent[game.currentPlr].animTick = 0;}
	else if(key_was_down(KEY_LEFT) 	&& key_is_up(KEY_LEFT) 	&& key_was_down(KEY_UP) 	&& key_is_down(KEY_UP)){	ent[game.currentPlr].currentDirection = 'u'; ent[game.currentPlr].frame	= ent[game.currentPlr].ani_walk_u ;if(ent[game.currentPlr].flippedX){ent[game.currentPlr].sprite->attr1 ^= ATTR1_HFLIP; ent[game.currentPlr].flippedX = false;} ent[game.currentPlr].animTimer = 0; ent[game.currentPlr].animTick = 0;}
	else if(key_was_down(KEY_UP) 	&& key_is_up(KEY_UP) 	&& key_was_down(KEY_RIGHT) 	&& key_is_down(KEY_RIGHT)){	ent[game.currentPlr].currentDirection = 'r'; ent[game.currentPlr].frame = ent[game.currentPlr].ani_walk_r ;if(!ent[game.currentPlr].flippedX){ent[game.currentPlr].sprite->attr1 ^= ATTR1_HFLIP; ent[game.currentPlr].flippedX = true;} ent[game.currentPlr].animTimer = 0; ent[game.currentPlr].animTick = 0;}
	else if(key_was_down(KEY_RIGHT) && key_is_up(KEY_RIGHT) && key_was_down(KEY_DOWN) 	&& key_is_down(KEY_DOWN)){	ent[game.currentPlr].currentDirection = 'd'; ent[game.currentPlr].frame = ent[game.currentPlr].ani_walk_d ;if(ent[game.currentPlr].flippedX){ent[game.currentPlr].sprite->attr1 ^= ATTR1_HFLIP; ent[game.currentPlr].flippedX = false;} ent[game.currentPlr].animTimer = 0; ent[game.currentPlr].animTick = 0;}

	else if(key_was_down(KEY_DOWN) 	&& key_is_up(KEY_DOWN) 	&& key_was_down(KEY_RIGHT) 	&& key_is_down(KEY_RIGHT)){	ent[game.currentPlr].currentDirection = 'r'; ent[game.currentPlr].frame = ent[game.currentPlr].ani_walk_r ;if(!ent[game.currentPlr].flippedX){ent[game.currentPlr].sprite->attr1 ^= ATTR1_HFLIP; ent[game.currentPlr].flippedX = true;} ent[game.currentPlr].animTimer = 0; ent[game.currentPlr].animTick = 0;}
	else if(key_was_down(KEY_RIGHT) && key_is_up(KEY_RIGHT) && key_was_down(KEY_UP) 	&& key_is_down(KEY_UP)){	ent[game.currentPlr].currentDirection = 'u'; ent[game.currentPlr].frame	= ent[game.currentPlr].ani_walk_u ;if(ent[game.currentPlr].flippedX){ent[game.currentPlr].sprite->attr1 ^= ATTR1_HFLIP; ent[game.currentPlr].flippedX = false;} ent[game.currentPlr].animTimer = 0; ent[game.currentPlr].animTick = 0;}
	else if(key_was_down(KEY_UP) 	&& key_is_up(KEY_UP) 	&& key_was_down(KEY_LEFT) 	&& key_is_down(KEY_LEFT)){	ent[game.currentPlr].currentDirection = 'l'; ent[game.currentPlr].frame	= ent[game.currentPlr].ani_walk_l ;if(ent[game.currentPlr].flippedX){ent[game.currentPlr].sprite->attr1 ^= ATTR1_HFLIP; ent[game.currentPlr].flippedX = false;} ent[game.currentPlr].animTimer = 0; ent[game.currentPlr].animTick = 0;}
	else if(key_was_down(KEY_LEFT) 	&& key_is_up(KEY_LEFT) 	&& key_was_down(KEY_DOWN) 	&& key_is_down(KEY_DOWN)){	ent[game.currentPlr].currentDirection = 'd'; ent[game.currentPlr].frame = ent[game.currentPlr].ani_walk_d ;if(ent[game.currentPlr].flippedX){ent[game.currentPlr].sprite->attr1 ^= ATTR1_HFLIP; ent[game.currentPlr].flippedX = false;} ent[game.currentPlr].animTimer = 0; ent[game.currentPlr].animTick = 0;}
	 
	//===================================================================================
	//       State Manager
	//==================================================================================
	if(DIR_Detection() && ent[game.currentPlr].state != state_idle) {
		ent[game.currentPlr].state = state_idle; 
		ent[game.currentPlr].animTick = 0; 
		ent[game.currentPlr].animTimer = 0;
	}
	else if(!DIR_Detection() && ent[game.currentPlr].state != state_walking){ 
		ent[game.currentPlr].state = state_walking;
		ent[game.currentPlr].animTick = 0;
		ent[game.currentPlr].animTimer = 0;
	}
	
	//===================================================================================
	//       Idle Manager
	//==================================================================================	
	if(key_was_down(KEY_UP) && key_released(KEY_UP)) ent[game.currentPlr].context = 5;
	if(key_was_down(KEY_DOWN) && key_released(KEY_DOWN)) ent[game.currentPlr].context = 6;
	if(key_was_down(KEY_LEFT) && key_released(KEY_LEFT)) ent[game.currentPlr].context = 7;
	if(key_was_down(KEY_RIGHT) && key_released(KEY_RIGHT)) ent[game.currentPlr].context = 8;
	
	//===================================================================================
	//       Walk Manager
	//==================================================================================
	if(ent[game.currentPlr].currentDirection == 'u' && key_is_down(KEY_UP)){
    	ent[game.currentPlr].context = 1; //Walk Up
	}						
	if(ent[game.currentPlr].currentDirection == 'd' && key_is_down(KEY_DOWN)){
        ent[game.currentPlr].context = 2; //Walk Down
	}
	if(ent[game.currentPlr].currentDirection == 'l' && key_is_down(KEY_LEFT)){
		ent[game.currentPlr].context = 3; //Walk Left
	}				
	if(ent[game.currentPlr].currentDirection == 'r' && key_is_down(KEY_RIGHT)){
		ent[game.currentPlr].context = 4; //Walk Right
	}
}

//Maps your controls to the next entity.
void ChangeCharacter(u32 key){
	if(cartType != cartFLASH) return;
	
	if(key_hit(key)){
		if(key_is_down(KEY_UP) && ent[game.currentPlr].currentDirection == 'u') ent[game.currentPlr].frame = 6;
		if(key_is_down(KEY_DOWN) && ent[game.currentPlr].currentDirection == 'd') ent[game.currentPlr].frame = 0;
		if(key_is_down(KEY_LEFT) && ent[game.currentPlr].currentDirection == 'l') ent[game.currentPlr].frame = 3;
		if(key_is_down(KEY_RIGHT) && ent[game.currentPlr].currentDirection == 'r') ent[game.currentPlr].frame = 3;

		ent[game.currentPlr].context = 0;
		ent[game.currentPlr].state = state_idle;
		ent[game.currentPlr].animTimer = 0;
		ent[game.currentPlr].animTick = 0;

    	if(game.currentPlr == level.entNo - 1) game.currentPlr = 0;
    	else game.currentPlr++;

		if(!DIR_Detection()){
				 if(ent[game.currentPlr].frame == 6) ent[game.currentPlr].currentDirection = 'u';
			else if(ent[game.currentPlr].frame == 0) ent[game.currentPlr].currentDirection = 'd';
			else if(ent[game.currentPlr].frame == 3) ent[game.currentPlr].currentDirection = 'l';
			else if(ent[game.currentPlr].frame == 3 && ent[game.currentPlr].flippedX) ent[game.currentPlr].currentDirection = 'r';
		}
		else{		
			if(key_is_down(KEY_DOWN))	ent[game.currentPlr].frame =  7;
			if(key_is_down(KEY_LEFT))	ent[game.currentPlr].frame = 17;
			if(key_is_down(KEY_RIGHT))	ent[game.currentPlr].frame = 17;
			if(key_is_down(KEY_UP))		ent[game.currentPlr].frame = 27;
		}	
    }
}

//Handles the movement of the player on the screen.
void MovePlayer(){
    if(key_is_down(KEY_UP))   ent[game.currentPlr].local_y--;
    if(key_is_down(KEY_DOWN)) ent[game.currentPlr].local_y++;
    if(key_is_down(KEY_LEFT)) ent[game.currentPlr].local_x--;
    if(key_is_down(KEY_RIGHT))ent[game.currentPlr].local_x++;
}

//Manages the pause screen.
void Pause(){
   if(pauseTick > 2){
      if(key_hit(KEY_START) && game.currentMap > 0){
         pauseTick = 0;
         pauseTimer = 0;
         menuItem = 0;
         if(game.paused){
            game.inputType = inputPlayer;
            DrawRect(8,4,12,10,0,0,0,1);
            ResetPlayerAnim();
         } 
         else{ 
            game.inputType = inputMenu;
            menuItems = 2;
            DrawFrame(8,4,12,10,0,1,false);
            DrawChar(11,6,'>',0,1); 
            DrawText(12,6 ,"RESUME",0,1);
            DrawText(12,8 ,"OPTION",0,1);
            DrawText(12,10,"SAVE",0,1);
            DrawText(12,12,"EXIT",0,1);
         }
         game.paused = !game.paused;
      }
   }  
   if(game.paused && !game.fadeOut){
      if(key_hit(KEY_DIR)){
         for(int i = 0; i < 8; i++) DrawTile(11,6 + i,1,0,0,0,1);
         DrawChar(11,6 + (2 * menuItem),'>',0,1);
      }
      if(key_hit(KEY_A)){
        
         switch (menuItem){
            case 0:
            	PlaySound(MenuConfirm);
			   	game.inputType = inputPlayer;
               	game.paused = false;
               	ResetPlayerAnim();
               	DrawRect(8,4,12,10,0,0,0,1); 
               	menuItem = 0;
            break;

			case 2:
			level.saving = true;
			DrawFrame(10,12,8,2,0,0,false);
			DrawText(11,13,"SAVING",0,0);
			break;

            case 3:
				PlaySound(MenuConfirm);
               	game.fadeOut = true;
               	game.resetting = true;
               	break;
            
            default:
			 PlaySound(MenuError);
               break;
         }
      }
   }
   
   if(game.paused){
      if(!game.fadeOut && game.resetting){ 
            
			//game.inputType = inputMenu;
			//menuItem = 0;
			//game.levelLoaded = false;
            //game.currentMap = 0;
            //game.paused = false;
            //game.resetting = false;
            //DrawRect(8,4,12,10,0,0,0,1); 
            //ResetVariables();   
            HardReset();
      }
	  if(level.saving){
		saveTimer++;
		if(saveTimer == 14){
			saveTick++;
			saveTimer = 0;
		}

		if(saveTick == 4)DrawChar(17,13,'.',0,0);
		if(saveTick == 8)DrawChar(18,13,'.',0,0);
		Save();
		if(saveTick == 12)DrawText(11,13,"SAVED.",0,0);
		 
		 if(saveTick == 14){
			PlaySound(MenuConfirm);
			DrawRect(8,4,12,10,0,0,0,1);
			DrawRect(10,12,8,2,0,0,0,0);
			ResetPlayerAnim(); saveTick = 0;
			game.inputType = inputPlayer;
			level.saving = false;
			game.paused = false;
		}
		
	  }
	}      
   pauseTimer++;
   if(pauseTimer > 5){
      pauseTick++;
      pauseTimer = 0;
   }
}

//My file menu function.
void MyMenu(u32 input){
	if(game.currentMap == 0){
		if(input == KEY_DIR){
			ConstructMap(mapManual,0);
		}
		if(input == KEY_A){
			
			if(menuItem < 2){
				game.fileNo = menuItem;
				if(file[menuItem].init){
					PlaySound(MenuConfirm);
					game.inputType = -5;
					game.fileNo = menuItem;
					menuItem = 0;
					game.fadeOut = true;
					level.loaded = false;
					game.currentMap = file[game.fileNo].currentMap;
					file[menuItem].playerPal = ent[0].palette;
				}
				else if(!file[menuItem].init){
					menuItem = 0;
					PlaySound(MenuConfirm);
					game.inputType = inputKeyboard;
					game.scrollType = 1;
					lyr[0].scrollY = -81;
					SetCamera();
					
					dialogDirection = 1;
					
					DrawFrame(1,10,25,10,0,0,false);
					DrawKeyboard(true);
				}
			}
			else if(menuItem == 2) PlaySound(MenuError); 
		}		
		if(input == KEY_SHOULDER){
			if(key_hit(KEY_SHOULDER) && file[menuItem].init) {
				ScrollPalette(key_tribool(KEY_L,KEY_R),0); UpdateSprites();
			}
		}
	}
}
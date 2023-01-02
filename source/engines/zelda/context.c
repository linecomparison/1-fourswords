#include "headers/project/context.h"
#include <stdio.h>

int cursorDestX = 29;
int cursorDestY = 88;
char nameChar = 'Q';
char* newGame = "NEW GAME";
bool nameChanged;
int nameIndex;
int chosenFile;
int delay = 6;
int scrollIndex;

char scrollDirection;
int scrollIndexCheck = 0;
int screenHeight = 256;
int screenWidth = 256;
int room;
bool playerHide = false;

int scrollBack;

unsigned short groundScreen[640];
unsigned short overheadScreen[640];
void ScrollMap(unsigned short* ground, unsigned short* overhead,int scrollBuffer,int dir);
void ScrollPlan(unsigned short* ground, unsigned short* overhead,int room);


//Handles the scrolling of screens(Currently Under Development)
//TODO, hide player sprite when zelda_classic scrolling, create seamless tile loading when zelda_classic scrolling.
int scrollBuffer = 0; int scrollBufferX;
void Scrolling(){
	
	//nt scrollBufferIndex = 0;
	if(level.scrolling){
		if(!playerHide){ HideSprite(ent[0].sprite); playerHide = true;}
		if(scrollIndex < scrollIndexCheck){
			
			switch(scrollDirection){
				case 'u':
				//level.scrollY--;
				for(int i = 0; i != level.entNo; i++){
					//if(scrollIndex < scrollIndexCheck - 110)ent[i].local_y++;
					
				}
				break;

				case 'd':
				//level.scrollY++;
				for(int i = 0; i != level.entNo; i++){
					//if(scrollIndex < scrollIndexCheck - 100)ent[i].local_y--;
				}
				break;

				case 'l':
				//level.scrollX--;
				for(int i = 0; i != level.entNo; i++){
					//if(scrollIndex < scrollIndexCheck)ent[i].local_x++;
				}
				break;

				case 'r':
				//level.scrollX++;
				
				//ScrollMap(groundScreen, overheadScreen,scrollBuffer,scrollDirection);
				
				for(int i = 0; i != level.entNo; i++){
					//if(scrollIndex < scrollIndexCheck)ent[i].local_x--;
				}
				break;
			}
				switch(scrollDirection){
					case 'u':
					//if(level.scrollY == 0){level.scrollY = 8;scrollBuffer++;}
					break;

					case 'd':
					//if(level.scrollY == 8){level.scrollY = 0;scrollBuffer++;}
					break;

					case 'l':
					
					//if(level.scrollX == 0){level.scrollX = 8;scrollBuffer++;}
					if(scrollBufferX == 8){scrollBufferX = 0;scrollBuffer++;}
					break;

					case 'r':
					//if(level.scrollX == 9){level.scrollX = 0;scrollBuffer++;}
					if(scrollBufferX == 8){scrollBufferX = 0;scrollBuffer++;}
					break;
				}
			ScrollMap(groundScreen, overheadScreen,scrollBuffer,scrollDirection);
			scrollIndex++;
			SetCamera();
			scrollBufferX++;
		}	
		else if(scrollIndex == scrollIndexCheck){
			scrollIndex = 0;
			scrollBufferX = 0;
			scrollBuffer = 0;
			level.scrolling = false;
			UnhideSprite(ent[0].sprite,0);
			playerHide = false;
			switch(scrollDirection){
				case 'l':
				ent[0].local_x= 210;
				level.scrollX = 0;
				ChangeFrame(0, 3);
				break;

				case 'r':
				ent[0].local_x= 0;
				level.scrollX = 8;
				ChangeFrame(0, 3);
				break;

				case 'u':
				ent[0].local_y-= 8;
				ChangeFrame(0, 6);
				break;

				case 'd':
				ent[0].local_y+= 8;
				ChangeFrame(0, 0);
				break;
			}
			SetCamera();
		
		}
		
	}
	else{ 
		for(int i = 0; i < 10; i++){
			if(ent[game.currentPlr].local_x / 8 == level.DoorCoordX[i] && ent[game.currentPlr].local_y / 8 == level.DoorCoordY[i])
			{
				level.scrolling =  true;
				scrollDirection = level.doorDir[i];
				level.doorCount = 0;
				switch(level.doorDir[i])
				{
					case 'u':
					room -=3;
					//level.scrollY = 8;
					break;

					case 'd':
					room +=3;
					//level.scrollY = 0;
					break;

					case 'l':
					room--;
					//level.scrollX = 8;
					break;

					case 'r':
					room++;
					//level.scrollY = 0;
					break;
				}
				//Fill(0,0,0,2);
				//Fill(0,0,0,1);
				ScrollPlan(groundScreen,overheadScreen,room);
				
				
			}
		}
		switch(scrollDirection){
		
			case 'u':
			scrollIndexCheck = screenHeight;
			break;

			case 'd':
			scrollIndexCheck = screenHeight;
			break;

			case 'l':
			scrollIndexCheck = screenWidth;
			break;

			case 'r':
			scrollIndexCheck = screenWidth;
			break;
		}
	}
}


void EventHandler(){
	Scrolling();
}




//Loads palettes into memory in accordance to the next selected level.
//zelda - context.c
//TODO, make this more streamlined, using arrays to select Palettes.
void LoadPalettes(){
	switch ((game.currentMap)){
		case 0:
		AddPaletteT(0);
		AddPaletteT(1);
		AddLinkPalettes();
		AddPaletteS(5);
		break;
		
		default:
		AddPaletteT(0);
		AddPaletteT(2);

		AddLinkPalettes();
		break;
	}
}

//Loads tilesets into memory according to the next loaded level.
//Located in: zelda - context.c
//TODO, make this more streamlined, using arrays to select Tilesets.
void LoadTilesets(){
	ResetVariables();
	LoadPalettes();
	switch ((game.currentMap)){
		case 0:
		AddTileset(0,1);
		break;
		
		case 1:
		AddTileset(0,1);	
		AddTileset(1,1);
		break;

		case 2:
		AddTileset(0,1);	
		AddTileset(1,1);
		break;
	}
	SpriteManager();	
}

//Constructs levels from either a map table or from components.
//Located in: zelda - context.c
void ConstructMap(int type,int map){

	if(type == mapCopy){

	}
    if(type == mapManual) {
		switch(map){
			
			case 0:
			Fill(1,1,0,2);
			DrawFrame(15,1,11,4,0,2,true);
			DrawFrame(15,6,11,2,0,1,false);

			for(int i = 0; i != 2; i++){
				if(i == menuItem){
					DrawFrame(1,1 + (5 * i),8,2,0,1,false);
					
					if(i < 2){
						if(!file[i].init) DrawText(2,2 + (5 * i),"NEW GAME",0,1); 
						else DrawText(2,2 + (5 * i),(char*)file[i].fileName,0,1);
					}
				}
				else{
					DrawFrame(1,1 + (5 * i),8,2,1,1,false);
					if(i < 2){
						if(!file[i].init) DrawText(2,2 + (5 * i),"NEW GAME",1,1);
						else DrawText(2,2 + (5 * i),(char*)file[i].fileName,1,1);
					}
				}
			}
			if(menuItem == 2){
				HideSprite(ent[0].sprite);
				DrawFrame(1,15,8,2,0,1,false);
				DrawText(2,16,"OPTIONS",0,1);
			}
			else{
				UnhideSprite(ent[0].sprite,0);
				DrawFrame(1,15,8,2,1,1,false);
				DrawText(2,16,(char*)"OPTIONS",1,1);
			}
					
			if(menuItem < 2 && file[menuItem].init) DrawText(16,7,LoadLocation(file[menuItem].currentMap),0,1);
			if(!file[menuItem].init) ChangePalette(0,4);
			else ChangePalette(0,file[menuItem].playerPal);
			if(file[menuItem].init)
			{	
				if(ent[0].state != state_idle){
					ent[0].animTick = 0;
					ent[0].animTimer = 0;
					ent[0].idleTimer = 0;
					ent[0].idleTick = 0;
					ent[0].state = state_idle;
					ent[0].frame = 8;
				}
			}
			else{
				if(ent[0].state != state_frozen){
					ent[0].state = state_frozen;
					ent[0].frame = 1;
				}
			}	
			break;

		}

	}
	if(type == mapPlan){


		
		for(int i = 0; i < 640; i++) overheadScreen[i] = 0;
		Fill(21,1,1,2);
		switch(map){
			
			default:
			RoomPlan(0,groundScreen);
			DoorPlan(13,0, 'u', groundScreen, overheadScreen);
			DoorPlan(13,17, 'd', groundScreen, overheadScreen);
			DoorPlan(0,8, 'l', groundScreen, overheadScreen);
			DoorPlan(27,8, 'r', groundScreen, overheadScreen);
			LoadMap(groundScreen,overheadScreen);
			
			break;

			case 1:			
			RoomPlan(0,groundScreen);
			DoorPlan(13,0, 'u', groundScreen, overheadScreen);
			DoorPlan(13,17, 'd', groundScreen, overheadScreen);
			DoorPlan(0,8, 'l', groundScreen, overheadScreen);
			DoorPlan(27,8, 'r', groundScreen, overheadScreen);
			LoadMap(groundScreen,overheadScreen);
			room = 4;
			break;

			case 2:
			Fill(21,1,1,2);
			
			RoomPlan(0,groundScreen);
			
			DoorPlan(5,0, 'u', groundScreen, overheadScreen);
			DoorPlan(13,17, 'd', groundScreen, overheadScreen);
			DoorPlan(0,8, 'l', groundScreen, overheadScreen);
			DoorPlan(27,8, 'r', groundScreen, overheadScreen);
			LoadMap(groundScreen,overheadScreen);
			RoomPlan(0,groundScreen);
			break;


		}
	}
}


//A Keyboard I'm working on.
//zelda - context.c
//TODO: Finish this routine.
void KeyboardMode(){
	
    if(dialogDirection == 1){ 
		key_capitals = true;
		lyr[0].scrollY++; SetCamera();
		ent[1].local_y--;
		
		if(lyr[0].scrollY == 0){ 
			dialogDirection = 0;
			for(int i = 0; i < 8; i++) file[game.fileNo].fileName[i] = ' ';
			DrawText(2,2 + (5 * (game.fileNo)),(char*)file[game.fileNo].fileName,0,1);
		}
	}
	if(dialogDirection == 0){
		if(lyr[0].scrollY == 0)
		{
			game.timer[Menus]++;
			if(game.timer[Menus] == delay){ game.timer[Menus] = 0; game.tick[Menus]++; }

			if(game.tick[Menus] > 1){
				if(key_is_down(KEY_DIR)) game.tick[Menus] = 0;
				
				if(key_is_down(KEY_UP)){
					if(menuItem >= 10 && menuItem <= 18) { menuItem -= 10; PlaySound(MenuCursor); }
					else if(menuItem >=19 && menuItem <= 25){ menuItem -= 9; PlaySound(MenuCursor); }
				}
				else if(key_is_down(KEY_DOWN)) {
					if(menuItem >= 0 && menuItem <= 8){ menuItem += 10; PlaySound(MenuCursor); }
					else if(menuItem >= 10 && menuItem <= 16){ menuItem += 9; PlaySound(MenuCursor); }
				}
				else if(key_is_down(KEY_LEFT)) {
					if(menuItem > 0 && menuItem <= 9){ menuItem--; PlaySound(MenuCursor); }
					else if(menuItem > 10 && menuItem <= 18){ menuItem--; PlaySound(MenuCursor); }
					else if(menuItem >= 20 && menuItem <= 25){ menuItem--; PlaySound(MenuCursor); }
				}
				else if(key_is_down(KEY_RIGHT)){ 
					if(menuItem >= 0 && menuItem <= 8){ menuItem++; PlaySound(MenuCursor); }
					else if(menuItem >= 10 && menuItem <= 17){ menuItem++; PlaySound(MenuCursor); }
					else if(menuItem >= 19 && menuItem <= 24){ menuItem++; PlaySound(MenuCursor); }
				}
				else if(key_hit(KEY_A)){
					
					if(nameIndex < 8){
						file[game.fileNo].fileName[nameIndex] = nameChar;
						DrawText(2,2 + (5 * (game.fileNo)),(char*)file[game.fileNo].fileName,0,1);
						PlaySound(MenuConfirm);
						nameIndex++;
						
					}
					else{
						PlaySound(MenuError);
					}
				}
				else if(key_hit(KEY_B)){	
					if(nameIndex > 0){
						file[game.fileNo].fileName[nameIndex - 1] = ' ';
						DrawText(2,2 + (5 * (game.fileNo)),(char*)file[game.fileNo].fileName,0,1);
						PlaySound(MenuConfirm);
						nameIndex--;
					}
					else if(nameIndex == 0){ 
						dialogDirection = -1; 
						PlaySound(MenuError); 
						nameChanged = false;
					}
				}
				else if(key_hit(KEY_START)){
					if(nameIndex > 0){ 
						dialogDirection = -1;  
						nameChanged = true;
						PlaySound(MenuConfirm);
					}
					else PlaySound(MenuError);
				}
				else if(key_hit(KEY_SELECT)){
					key_capitals = !key_capitals;
					DrawKeyboard(key_capitals);
					PlaySound(MenuCursor);
				}

				switch(menuItem)
				{
					case 0:
						cursorDestX = 29;
						cursorDestY = 88;
						nameChar  =	key_capitals ? 'Q' : 'q';
					break;

					case 1:
						cursorDestX = 45;
						cursorDestY = 88;
						nameChar  =	key_capitals ? 'W' : 'w';
						
					break;

					case 2:
						cursorDestX = 61;
						cursorDestY = 88;
						nameChar  =	key_capitals ? 'E' : 'e';
					break;

					case 3:
						cursorDestX = 77;
						cursorDestY = 88;
						nameChar  =	key_capitals ? 'R' : 'r';
					break;

					case 4:
						cursorDestX = 93;
						cursorDestY = 88;
						nameChar  =	key_capitals ? 'T' : 't';
					break;

					case 5:
						cursorDestX = 109;
						cursorDestY = 88;
						nameChar  =	key_capitals ? 'Y' : 'y';
					break;

					case 6:
						cursorDestX = 125;
						cursorDestY = 88;
						nameChar  =	key_capitals ? 'U' : 'u';
					break;

					case 7:
						cursorDestX = 141;
						cursorDestY = 88;
						nameChar  =	key_capitals ? 'I' : 'i';
					break;

					case 8:
						cursorDestX = 157;
						cursorDestY = 88;
						nameChar  =	key_capitals ? 'O' : 'o';
					break;

					case 9:
						cursorDestX = 173;
						cursorDestY = 88;
						nameChar  =	key_capitals ? 'P' : 'p';
					break;

					case 10:
						cursorDestX = 29;
						cursorDestY = 104;
						nameChar  =	key_capitals ? 'A' : 'a';
					break;

					case 11:
						cursorDestX = 45;
						cursorDestY = 104;
						nameChar  =	key_capitals ? 'S' : 's';
					break;

					case 12:
						cursorDestX = 61;
						cursorDestY = 104;
						nameChar  =	key_capitals ? 'D' : 'd';
					break;

					case 13:
						cursorDestX = 77;
						cursorDestY = 104;
						nameChar  =	key_capitals ? 'F' : 'f';
					break;

					case 14:
						cursorDestX = 93;
						cursorDestY = 104;
						nameChar  =	key_capitals ? 'G' : 'g';
					break;

					case 15:
						cursorDestX = 109;
						cursorDestY = 104;
						nameChar  =	key_capitals ? 'H' : 'h';
					break;

					case 16:
						cursorDestX = 125;
						cursorDestY = 104;
						nameChar  =	key_capitals ? 'J' : 'j';
					break;

					case 17:
						cursorDestX = 141;
						cursorDestY = 104;
						nameChar  =	key_capitals ? 'K' : 'k';
					break;

					case 18:
						cursorDestX = 157;
						cursorDestY = 104;
						nameChar  =	key_capitals ? 'L' : 'l';
					break;


					case 19:
						cursorDestX = 29;
						cursorDestY = 120;
						nameChar  =	key_capitals ? 'Z' : 'z';
					break;

					case 20:
						cursorDestX = 45;
						cursorDestY = 120;
						nameChar  =	key_capitals ? 'X' : 'x';
					break;

					case 21:
						cursorDestX = 61;
						cursorDestY = 120;
						nameChar  =	key_capitals ? 'C' : 'c';
					break;

					case 22:
						cursorDestX = 77;
						cursorDestY = 120;
						nameChar  =	key_capitals ? 'V' : 'v';
					break;

					case 23:
						cursorDestX = 93;
						cursorDestY = 120;
						nameChar  =	key_capitals ? 'B' : 'b';
					break;

					case 24:
						cursorDestX = 109;
						cursorDestY = 120;
						nameChar  =	key_capitals ? 'N' : 'n';
					break;

					case 25:
						cursorDestX = 125;
						cursorDestY = 120;
						nameChar  =	key_capitals ? 'M' : 'm';
					break;
				} 
			}
			if(ent[1].local_x  < cursorDestX) ent[1].local_x+= 4;
			if(ent[1].local_x  > cursorDestX) ent[1].local_x-= 4;
			if(ent[1].local_y  < cursorDestY) ent[1].local_y+= 4;
			if(ent[1].local_y  > cursorDestY) ent[1].local_y-= 4;
		}
	}
    
	if(dialogDirection == -1){ 
		lyr[0].scrollY--;SetCamera();
		ent[1].local_y++;

		if(lyr[0].scrollY == -81){ 
			dialogDirection = 0;
			if(!nameChanged){
				for(int i = 0; i < 8; i++) file[game.fileNo].fileName[i] = newGame[i];
				DrawText(2,2 + (5 * (game.fileNo)),(char*)file[game.fileNo].fileName,0,1);
				
			}
			else{
				file[game.fileNo].init = true;
				file[game.fileNo].playerPal = 0;
				file[game.fileNo].currentMap = 1;
				if(file[game.fileNo].fileName[0] == 'R' && file[game.fileNo].fileName[1] == 'E' && file[game.fileNo].fileName[2] == 'D'){ file[game.fileNo].playerPal = 1; file[game.fileNo].currentMap = 2; }
				else if(file[game.fileNo].fileName[0] == 'B' && file[game.fileNo].fileName[1] == 'L' && file[game.fileNo].fileName[2] == 'U' && file[game.fileNo].fileName[3] == 'E'){ file[game.fileNo].playerPal = 2; file[game.fileNo].currentMap = 3; }
				else if(file[game.fileNo].fileName[0] == 'P' && file[game.fileNo].fileName[1] == 'I' && file[game.fileNo].fileName[2] == 'N' && file[game.fileNo].fileName[3] == 'K'){ file[game.fileNo].playerPal = 3; file[game.fileNo].currentMap = 4; }
				else if(file[game.fileNo].fileName[0] == 'D' && file[game.fileNo].fileName[1] == 'A' && file[game.fileNo].fileName[2] == 'R' && file[game.fileNo].fileName[3] == 'K'){ file[game.fileNo].playerPal = 4; file[game.fileNo].currentMap = 5; }

				ChangePalette(0,file[game.fileNo].playerPal);
				ent[0].animTick = 0;
				ent[0].animTimer = 0;
				ent[0].idleTimer = 0;
				ent[0].idleTick = 0;
				ent[0].state = state_idle;
				ent[0].frame = 8;
				
				DrawText(16,7,LoadLocation(file[game.fileNo].currentMap),0,1);
				nameIndex = 0;

				CreateFile();
			}
			
			ent[1].local_x = 29;
			ent[1].local_y = 169;
			DrawRect(1,10,25,10,0,0,0,0);

			menuItem = game.fileNo;
			game.inputType = inputMenu;
		}
	}

	

}

//Manages all basic input.
//zelda - context.c
//\param customMenu calls for additional menu functions depending on the context.
void InputManager(void customMenu()){	
	if(game.tick[Resets] > 2){
		if(game.inputType == inputMenu){
			if(!game.fadeOut){
				NavigateMenu();
				
				//Insert Custom Menu Functions here.
				if(key_hit(KEY_DIR)) customMenu(KEY_DIR);
				if(key_hit(KEY_A)) 	customMenu(KEY_A);
				if(key_hit(KEY_SHOULDER)) customMenu(KEY_SHOULDER);	
			}		
			
		}
		if(game.inputType == inputPlayer){
			
			if(game.faded[1]){
				ResetPlayerAnim();	
			}
			else{
				if(!level.scrolling){ 
					MovePlayer();
					AnimatePlayer();
				}
				EventHandler();
				ChangeCharacter(KEY_SELECT);
			}
		}	
		if(game.inputType == inputKeyboard){
			KeyboardMode();
		}		
	}
	Reset();
	Pause();
}

//The main map manager.
void LoadLevel(){

	switch(game.currentMap){
		default:
		ConstructMap(mapPlan,game.currentMap);
		break;
	}
	
}




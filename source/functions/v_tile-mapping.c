#include "headers/v_tile-mapping.h"

//--------------------------------------------------------------------------------------------------------
//	Misc
//--------------------------------------------------------------------------------------------------------

//Writes layer settings to the RAM.
void SetupLayers(){
     REG_BG0CNT= BG_PRIO(0) | BG_CBB(1) | BG_SBB(1) | BG_16_COLOR | BG_SIZE_256 | 0;
	 REG_BG1CNT= BG_PRIO(1) | BG_CBB(1) | BG_SBB(2) | BG_16_COLOR | BG_SIZE_256 | 0;
	 REG_BG2CNT= BG_PRIO(2) | BG_CBB(1) | BG_SBB(3) | BG_16_COLOR | BG_SIZE_256 | 0;
	 REG_BG3CNT= BG_PRIO(3) | BG_CBB(4) | BG_SBB(4) | BG_16_COLOR | BG_SIZE_256 | 0;
}

//sets the camera.
void SetCamera(){
	
	REG_BG0HOFS= lyr[0].scrollX;
	REG_BG0VOFS= lyr[0].scrollY;
	
	REG_BG1HOFS= lyr[1].scrollX + level.scrollX;
	REG_BG1VOFS= lyr[1].scrollY + level.scrollY;

	REG_BG2HOFS= lyr[2].scrollX + level.scrollX;
	REG_BG2VOFS= lyr[2].scrollY + level.scrollY;
	
	REG_BG3HOFS= lyr[3].scrollX + level.scrollX;
	REG_BG3VOFS= lyr[3].scrollY + level.scrollY;
}

//Handles camera movement.
void MoveCamera(u32 scrollForce){

	switch(game.scrollType)
	{
		default:
		if(key_is_down(KEY_DIR)){
			switch(scrollForce){
			
				case KEY_UP:	if(level.scrollY > 0) 				level.scrollY--;
				case KEY_DOWN:	if(level.scrollY < level.mapHeight)	level.scrollY++;
				case KEY_LEFT:	if(level.scrollX > 0)				level.scrollX--;
				case KEY_RIGHT: if(level.scrollX < level.mapWidth)	level.scrollX++;
			}
		}
		
		case 0:
		if(level.scrollX > 0 && level.scrollX < level.mapWidth) level.scrollX = ent[game.currentPlr].global_x;
		if(level.scrollY > 0 && level.scrollY < level.mapHeight) level.scrollX = ent[game.currentPlr].global_y;
		break;

		case 1:
		switch(scrollForce){
			case KEY_L:
			lyr[0].scrollY--;
			break;

			case KEY_R:
			lyr[0].scrollY++;
			break;
		}
		break;
	}
	SetCamera();
}

//Writes a tileset to memory.
void AddTileset(int tileSet,int charBase){
	int size;

	switch(tileSet)
	{
		default:
		memcpy(&tile_mem[charBase][game.tilesCount], pauseTiles, sizeof(pauseTiles)); 
		size = 67;
		break;

		case 1:
		memcpy(&tile_mem[charBase][game.tilesCount], house1Tiles, sizeof(house1Tiles));
		size = 20;
		break;
	}
	
	tileset[level.tileSetCount].tilesCount = size;
	tileset[level.tileSetCount].charStart = game.tilesCount; 
   
    game.tilesCount += tileset[level.tileSetCount].tilesCount;
	level.tileSetCount++;
}

//--------------------------------------------------------------------------------------------------------
//	Palette
//--------------------------------------------------------------------------------------------------------

//Adds a pallete to a table which can be referenced for later use.
void AddPaletteT(int pal){
	palIndexesT[level.tilePalCount] = pal;
	level.tilePalCount++;
}

//Sets up a table of a set palette bank for use.
void GetPaletteT(int pal){
	switch(pal){
		
		default:
		for(int i = 0; i < 16; i++) palData[i] = pausePal[i];
		break;

		case 1:
		for(int i = 0; i < 16; i++) palData[i] = fileSelectDim[i];
		break;

		case 2:
		for(int i = 0; i < 16; i++) palData[i] = house1Pal[i];
		break;
	}
}

//--------------------------------------------------------------------------------------------------------
//	Draw Tiles
//--------------------------------------------------------------------------------------------------------

//Draws a tile on the screen.
void DrawTile(int x, int y, int tileID,int tileSetNo,int flipped,int pal,int layer){
	int flipvalue;
	if(flipped == 0) flipvalue = 0;
	if(flipped == 1) flipvalue = 0x400;
	if(flipped == 2) flipvalue = 0x800;
	if(flipped == 3) flipvalue = 0xC00;

	se_mem[layer + 1][(32 * y) + x] = tileID + flipvalue + tileset[tileSetNo].charStart + (0x1000 * pal);
}

//Fills the entire layer with a single tile.
void Fill(int tileID,int tileSetNo,int pal,int layer){
	for(int x = 0; x != 32; x++) for(int y = 0; y != 32; y++) {DrawTile(x,y,tileID,tileSetNo,0,pal,layer);}
}

//Draws a simple rectangle with the tileID of your choosing.
void DrawRect(int posX,int posY,int width, int height, int tileID,int tileSetNo, int pal,int layer){
	for(int y = 0; y < height + 2; y++) for(int x = 0; x < width +2; x++){
		DrawTile(x + posX,y + posY,tileID,tileSetNo,0,pal,layer);
	}
}

//Draw anything that resembles a text box.
void DrawFrame(int posX, int posY, int width, int height, int pal,int layer, bool overlapped){
	int corner; int hor; int vert; int center;
	if(overlapped){ corner = 62; hor = 63; vert = 93;}
	else{ 			corner = 2; hor = 32; vert = 33; center = 1;}
	
	DrawTile(posX,posY,corner,0,0,pal,layer);
	
	for(int x = 0; x != width; x++)
	{
		DrawTile(posX + x + 1,posY,hor,0,0,pal,layer);
		DrawTile(posX + x + 1,posY + height + 1,hor,0,2,pal,layer);
	}
	DrawTile(posX + width + 1,posY,corner,0,1,pal,layer);	
	for(int y = 0; y != height; y++)
	for(int x = 0; x != width + 1 ; x++){
		if(x == 0) DrawTile(posX + x,posY + y + 1,vert,0,0,pal,layer);
		if(x == width - 1) DrawTile(posX + x + 2,posY + y + 1,vert,0,1,pal,layer);
		if(x > 0 && x < width + 1) DrawTile(x + posX,posY + y + 1,center,0,0,pal,layer);
	}
	DrawTile(posX,posY + height + 1,corner,0,2,pal,layer);
	DrawTile(posX + width + 1,posY + height + 1,corner,0,3,pal,layer);
}


//--------------------------------------------------------------------------------------------------------
//	Map Creation
//--------------------------------------------------------------------------------------------------------

//draws a room layout of your choosing,
//best used if scroll type is of type house.
void AddRoom(int type){
	switch(type){	
		case 0: //Basic Square
		for(int y = 0; y != 20; y++) for(int x = 0; x != 30; x++)DrawTile(x,y,0,0,0,1,1);
		
		// == Walls ==
		DrawTile(0,0,0,1,0,1,2);   DrawTile(29,0,0,1,1,1,2);  
		DrawTile(1,1,4,1,0,1,2);   DrawTile(28,1,4,1,1,1,2); 
		DrawTile(1,18,4,1,2,1,2);  DrawTile(28,18,4,1,3,1,2);		
		DrawTile(0,19,0,1,2,1,2);  DrawTile(29,19,0,1,3,1,2);
		for(int i = 0; i != 28; i++) {DrawTile(1 + i,0,1,1,1,1,2); DrawTile(1 + i,19,1,1,2,1,2);}
		for(int i = 0; i != 26; i++) {DrawTile(2 + i,1,6,1,0,1,2); DrawTile(2 + i,18,6,1,2,1,2);}      
		for(int i = 0; i != 18; i++) {DrawTile(0,1 + i,2,1,0,1,2); DrawTile(29,1 + i,2,1,1,1,2);}
		for(int i = 0; i != 16; i++) {DrawTile(1,2 + i,7,1,0,1,2); DrawTile(28,2 + i,7,1,1,1,2);}
        
		// == Floor ==
		DrawTile(2,2,16,1,0,1,2);  DrawTile(27,2,16,1,1,1,2); 
		DrawTile(2,17,16,1,2,1,2); DrawTile(27,17,16,1,3,1,2);
		for(int i = 0; i != 24; i++) {DrawTile(3 + i,2,17,1,0,1,2); DrawTile(3 + i,17,17,1,2,1,2);} 
        for(int i = 0; i != 14; i++) {DrawTile(2,3 + i,18,1,0,1,2); DrawTile(27,3 + i,18,1,1,1,2);}
        for(int y = 0; y < 14; y+= 2) for(int x = 0; x < 23; x+=2){DrawTile(3 + x,3 + y,19,1,0,1,2); DrawTile(4 + x,3 + y,20,1,0,1,2);}
        for(int y = 0; y < 14; y+= 2) for(int x = 0; x < 23; x+=2){DrawTile(3 + x,4 + y,20,1,0,1,2); DrawTile(4 + x,4 + y,19,1,0,1,2);}
		break;

		case 1: //Horizontal
		for(int i = 0; i != 28; i++) {DrawTile(1 + i,0,1,1,1,1,2); DrawTile(1 + i,19,1,1,2,1,2);}
		break;
	}
}

//draws a door with the position and orientation of your choosing,
//best used if scroll type is of type house.
void AddDoor(char orientation,int x,int y){
	switch(orientation)
	{
		case 'u': //A door on a Northern wall.
		DrawTile(x + 1,y,21,1,0,1,2);
		DrawTile(x + 2,y,21,1,0,1,2);

		DrawTile(x,y,11,1,0,1,1);
		DrawTile(x + 1,y,12,1,0,1,1);
		DrawTile(x + 2,y,12,1,1,1,1);
		DrawTile(x + 3,y,11,1,1,1,1);

		DrawTile(x,y + 1,13,1,0,1,2);
		DrawTile(x + 1,y + 1,21,1,0,1,2);
		DrawTile(x + 2,y + 1,21,1,1,1,2);
		DrawTile(x + 3,y + 1,13,1,1,1,2);

		DrawTile(x,y + 2,14,1,0,1,2);
		DrawTile(x + 1,y + 2,15,1,0,1,2);
		DrawTile(x + 2,y + 2,15,1,1,1,2);
		DrawTile(x + 3,y + 2,14,1,1,1,2);

		level.DoorCoordX[level.doorCount] = x;
		level.DoorCoordY[level.doorCount] = y -2;
		break;
	}
	level.doorDir[level.doorCount] = orientation;
	level.doorCount++;
}

void LoadMap(unsigned short* ground, unsigned short* overhead){
	for(int y = 0; y < 20; y++)
		for(int x = 0; x < 30; x++){
			se_mem[2][GetCoord(x,y)] = overhead[GetCoord(x,y)];
			se_mem[3][GetCoord(x,y)] = ground[GetCoord(x,y)];
		}
}



//--------------------------------------------------------------------------------------------------------
//	Pre-emptive Mapping
//--------------------------------------------------------------------------------------------------------

//To be used with the other functions in the planning series, 
//adds a tile to an array instead of directly to VRAM.
unsigned short TilePlan(int tileID,int tileSetNo,int flipped,int pal){
	int flipvalue = 0;
	
	switch(flipped){
		
		case 0:
		flipvalue = 0;
		break;

		case 1:
		flipvalue = 0x400;
		break;

		case 2:
		flipvalue = 0x800;
		break;

		case 3:
		flipvalue = 0xC00;
		break;
	}

	return tileID + flipvalue + tileset[tileSetNo].charStart + (0x1000 * pal);
}

//Plan a room and set it as the next loaded map.
//(Mainly meant for Zelda 1 style scrolling)
void RoomPlan(int type,short unsigned int* cont){

	switch(type){	
		case 0: //Basic Square
		//for(int y = 0; y != 20; y++) for(int x = 0; x != 30; x++) cont[x + (32 * y)] =  TilePlan(0,0,0,1);
		// == Walls ==
		cont[GetCoord(0,0)] = TilePlan(0,1,0,1);	cont[GetCoord(29,0)] = TilePlan(0,1,1,1);
		cont[GetCoord(1,1)] = TilePlan(4,1,0,1);	cont[GetCoord(28,1)] = TilePlan(4,1,1,1);
		cont[GetCoord(1,18)] = TilePlan(4,1,2,1);	cont[GetCoord(28,18)] = TilePlan(4,1,3,1);
		cont[GetCoord(0,19)] = TilePlan(0,1,2,1);	cont[GetCoord(29,19)] = TilePlan(0,1,3,1);
		
		for(int x = 0; x != 28; x++) {cont[GetCoord(1 + x,0)] = TilePlan(1,1,1,1); cont[GetCoord(1 + x,19)] = TilePlan(1,1,2,1);}
		for(int x = 0; x != 26; x++) {cont[GetCoord(2 + x,1)] = TilePlan(6,1,0,1); cont[GetCoord(2 + x,18)] = TilePlan(6,1,2,1);}
		for(int y = 1; y != 19; y++) {cont[GetCoord(0,y)] = TilePlan(2,1,0,1); cont[GetCoord(29,y)] = TilePlan(2,1,1,1);}
		for(int y = 0; y != 16; y++) {cont[GetCoord(1,2 + y)] = TilePlan(7,1,0,1); cont[GetCoord(28,2 + y)] = TilePlan(7,1,1,1);}

		// == Floor ==
		cont[GetCoord(2,2)]  = TilePlan(16,1,0,1);	cont[GetCoord(27,2)]  = TilePlan(16,1,1,1);
		cont[GetCoord(2,17)] = TilePlan(16,1,2,1);	cont[GetCoord(27,17)] = TilePlan(16,1,3,1);

		for(int x = 0; x != 24; x++) {cont[GetCoord(3 + x,2)] = TilePlan(17,1,0,1); cont[GetCoord(3 + x,17)] = TilePlan(17,1,2,1);}
		for(int y = 0; y != 14; y++) {cont[GetCoord(2,3 + y)] = TilePlan(18,1,0,1); cont[GetCoord(27,3 + y)] = TilePlan(18,1,1,1);}

		for(int y = 0; y < 14; y+= 2)for(int x = 0; x < 23; x += 2) {cont[GetCoord(3 + x, 3 + y)] = TilePlan(19,1,0,1); cont[GetCoord(4 + x,3 + y)] = TilePlan(20,1,0,1);}
		for(int y = 0; y < 14; y+= 2)for(int x = 0; x < 23; x += 2) {cont[GetCoord(3 + x, 4 + y)] = TilePlan(20,1,0,1); cont[GetCoord(4 + x,4 + y)] = TilePlan(19,1,0,1);}
		break;
	}

	level.doorCount++;
}

//Plan a room and set it as the next loaded map.
//(Mainly meant for Zelda 1 style scrolling)
void DoorPlan(int x, int y, char orientation, unsigned short* ground, unsigned short* overhead){
	switch(orientation)
	{
		case 'u': //A door on a Northern wall.
		ground[GetCoord(x + 1,y)] = TilePlan(21,1,0,1);
		ground[GetCoord(x + 2,y)] = TilePlan(21,1,0,1);
		
		overhead[GetCoord(x    ,y)] = TilePlan(11,1,0,1);
		overhead[GetCoord(x + 1,y)] = TilePlan(12,1,0,1);
		overhead[GetCoord(x + 2,y)] = TilePlan(12,1,1,1);
		overhead[GetCoord(x + 3,y)] = TilePlan(11,1,1,1);
				
		ground[GetCoord(x    ,y + 1)] = TilePlan(13,1,0,1);
		ground[GetCoord(x + 1,y + 1)] = TilePlan(21,1,0,1);
		ground[GetCoord(x + 2,y + 1)] = TilePlan(21,1,1,1);
		ground[GetCoord(x + 3,y + 1)] = TilePlan(13,1,1,1);

		ground[GetCoord(x    ,y + 2)] = TilePlan(14,1,0,1);
		ground[GetCoord(x + 1,y + 2)] = TilePlan(15,1,0,1);
		ground[GetCoord(x + 2,y + 2)] = TilePlan(15,1,1,1);
		ground[GetCoord(x + 3,y + 2)] = TilePlan(14,1,1,1);

		level.DoorCoordX[level.doorCount] = x;
		level.DoorCoordY[level.doorCount] = y -2;
		break;
		
		case 'd': //A door on a Northern wall.
		ground[GetCoord(x    ,y)] = TilePlan(14,1,2,1);
		ground[GetCoord(x + 1,y)] = TilePlan(15,1,2,1);
		ground[GetCoord(x + 2,y)] = TilePlan(15,1,3,1);
		ground[GetCoord(x + 3,y)] = TilePlan(14,1,3,1);
		
		ground[GetCoord(x    ,y + 1)] = TilePlan(13,1,2,1);
		ground[GetCoord(x + 1,y + 1)] = TilePlan(21,1,2,1);
		ground[GetCoord(x + 2,y + 1)] = TilePlan(21,1,3,1);
		ground[GetCoord(x + 3,y + 1)] = TilePlan(13,1,3,1);

		ground[GetCoord(x + 1,y + 2)] = TilePlan(21,1,0,1);
		ground[GetCoord(x + 2,y + 2)] = TilePlan(21,1,0,1);
		
		overhead[GetCoord(x    ,y + 2)] = TilePlan(11,1,2,1);
		overhead[GetCoord(x + 1,y + 2)] = TilePlan(12,1,2,1);
		overhead[GetCoord(x + 2,y + 2)] = TilePlan(12,1,3,1);
		overhead[GetCoord(x + 3,y + 2)] = TilePlan(11,1,3,1);

		level.DoorCoordX[level.doorCount] = x;
		level.DoorCoordY[level.doorCount] = y +2;
		break;

		case 'l': //A door on a Northern wall.
		overhead[GetCoord(x,y)] = TilePlan(27,1,1,1);
		ground[GetCoord(x + 1,y)] = TilePlan(26,1,1,1);
		ground[GetCoord(x + 2,y)] = TilePlan(25,1,1,1);

		
		overhead[GetCoord(x + 0,y + 1)] = TilePlan(29,1,3,1);
		overhead[GetCoord(x + 0,y + 2)] = TilePlan(29,1,1,1);
		

		ground[GetCoord(x + 1,y + 1)] = TilePlan(21,1,3,1);
		ground[GetCoord(x + 1,y + 2)] = TilePlan(21,1,1,1);
		
		ground[GetCoord(x +2,y + 1)] = TilePlan(28,1,3,1);
		ground[GetCoord(x +2,y + 2)] = TilePlan(28,1,1,1);
		
		overhead[GetCoord(x + 0,y + 3)] = TilePlan(27,1,3,1);
		ground[GetCoord(x + 1,y + 3)] = TilePlan(26,1,3,1);
		ground[GetCoord(x + 2,y + 3)] = TilePlan(25,1,3,1);

		level.DoorCoordX[level.doorCount] = x - 3;
		level.DoorCoordY[level.doorCount] = y - 1;
		break;

		case 'r': //A door on a Northern wall.
		ground[GetCoord(x,y)] = TilePlan(25,1,0,1);
		ground[GetCoord(x + 1,y)] = TilePlan(26,1,0,1);
		overhead[GetCoord(x + 2,y)] = TilePlan(27,1,0,1);

		ground[GetCoord(x,y + 1)] = TilePlan(28,1,0,1);
		ground[GetCoord(x,y + 2)] = TilePlan(28,1,2,1);
		
		ground[GetCoord(x + 1,y + 1)] = TilePlan(21,1,0,1);
		ground[GetCoord(x + 1,y + 2)] = TilePlan(21,1,2,1);

		overhead[GetCoord(x + 2,y + 1)] = TilePlan(29,1,0,1);
		overhead[GetCoord(x + 2,y + 2)] = TilePlan(29,1,2,1);

		ground[GetCoord(x,y + 3)] = TilePlan(25,1,2,1);
		ground[GetCoord(x + 1,y + 3)] = TilePlan(26,1,2,1);
		overhead[GetCoord(x + 2,y + 3)] = TilePlan(27,1,2,1);

		level.DoorCoordX[level.doorCount] = x + 3;
		level.DoorCoordY[level.doorCount] = y - 1;
		break;
	}

	level.doorDir[level.doorCount] = orientation;
	level.doorCount++;
}

void ScrollPlan(unsigned short* ground, unsigned short* overhead,int room){
	for(int i = 0; i < 641; i++)
	{
		ground[i] = 0;
		overhead[i] = 0;
	}
	switch(game.currentMap)
	{
		
		case 1:

			switch(room)
			{
				default:
				Fill(0,0,0,1);
				break;

				case 0:
				RoomPlan(0,ground);
				DoorPlan(13,17, 'd', ground, overhead);
				DoorPlan(27,8, 'r', ground, overhead);
				break;

				case 1:
				RoomPlan(0,ground);
				DoorPlan(13,17, 'd', ground, overhead);
				DoorPlan(0,8, 'l', ground, overhead);
				DoorPlan(27,8, 'r', ground, overhead);
				break;

				case 2:
				RoomPlan(0,ground);
				DoorPlan(13,17, 'd', ground, overhead);
				DoorPlan(0,8, 'l', ground, overhead);
				break;

				case 3:
				RoomPlan(0,ground);
				DoorPlan(13,0, 'u', ground, overhead);
				DoorPlan(13,17, 'd', ground, overhead);
				DoorPlan(27,8, 'r', ground, overhead);
				break;
				
				case 4:
				RoomPlan(0,ground);
				DoorPlan(13,0, 'u', ground, overhead);
				DoorPlan(13,17, 'd', ground, overhead);
				DoorPlan(0,8, 'l', ground, overhead);
				DoorPlan(27,8, 'r', ground, overhead);
				break;

				case 5:
	
				RoomPlan(0,ground);
				DoorPlan(13,0, 'u', ground, overhead);
				DoorPlan(13,17, 'd', ground, overhead);
				DoorPlan(0,8, 'l', ground, overhead);
				break;




				case 6:
				RoomPlan(0,ground);
				DoorPlan(13,0, 'u', ground, overhead);
				DoorPlan(27,8, 'r', ground, overhead);
				break;

				case 7:
				RoomPlan(0,ground);
				DoorPlan(13,0, 'u', ground, overhead);
				DoorPlan(0,8, 'l', ground, overhead);
				DoorPlan(27,8, 'r', ground, overhead);
				break;

				case 8:
				RoomPlan(0,ground);
				DoorPlan(13,0, 'u', ground, overhead);
				DoorPlan(0,8, 'l', ground, overhead);
				break;
			}

		break;
	}
}

void ScrollMap(unsigned short* ground, unsigned short* overhead,int scrollBuffer,int dir){
	//Fill(0,0,0,2);
	switch(dir)
	{
		case 'r':
		
			for(int i = 0; i < 20; i++){

				for(int o = 0; o < scrollBuffer; o++){
					se_mem[3][GetCoord((31 - scrollBuffer) + o + 1,i)] = ground[GetCoord(o,i)];
					se_mem[2][GetCoord((31 - scrollBuffer) + o + 1,i)] = overhead[GetCoord(o,i)];
				}
			}
		
		break;

		case 'l':
		
			for(int i = 0; i < 20; i++){

				for(int o = 0; o < scrollBuffer; o++){
					if(scrollBuffer < 31){
					se_mem[3][GetCoord((0 + scrollBuffer) - o - 1,i)] = ground[GetCoord(29 - o,i)];
					se_mem[2][GetCoord((0 + scrollBuffer) - o - 1,i)] = overhead[GetCoord(29 - o,i)];
					}
				}
			}
		
		break;
	}
	
}


//--------------------------------------------------------------------------------------------------------
//	Draw Text
//--------------------------------------------------------------------------------------------------------

//Place tiled letters on the screen.
void DrawChar(int posX, int posY, char letter,int pal,int layer){
	
	int letterID[2];
	switch(letter)
	{
		default:
		letterID[0] = 1; 
		letterID[1] = 1;
		break;

		case 'A':
		letterID[0] = 3;
		letterID[1] = 34;
		break;

		case 'B':
		letterID[0] = 4;
		letterID[1] = 35;
		break;

		case 'C':
		letterID[0] = 5;
		letterID[1] = 36;
		break;

		case 'D':
		letterID[0] = 6;
		letterID[1] = 37;
		break;

		case 'E':
		letterID[0] = 7;
		letterID[1] = 38;
		break;

		case 'F':
		letterID[0] = 8;
		letterID[1] = 39;
		break;

		case 'G':
		letterID[0] = 9;
		letterID[1] = 40;
		break;

		case 'H':
		letterID[0] = 10;
		letterID[1] = 41;
		break;

		case 'I':
		letterID[0] = 11;
		letterID[1] = 42;
		break;

		case 'J':
		letterID[0] = 12;
		letterID[1] = 43;
		break;

		case 'K':
		letterID[0] = 13;
		letterID[1] = 44;
		break;

		case 'L':
		letterID[0] = 14;
		letterID[1] = 45;
		break;

		case 'M':
		letterID[0] = 15;
		letterID[1] = 46;
		break;

		case 'N':
		letterID[0] = 16;
		letterID[1] = 47;
		break;

		case 'O':
		letterID[0] = 17;
		letterID[1] = 48;
		break;

		case 'P':
		letterID[0] = 18;
		letterID[1] = 49;
		break;

		case 'Q':
		letterID[0] = 19;
		letterID[1] = 50;
		break;

		case 'R':
		letterID[0] = 20;
		letterID[1] = 51;
		break;

		case 'S':
		letterID[0] = 21;
		letterID[1] = 52;
		break;

		case 'T':
		letterID[0] = 22;
		letterID[1] = 53;
		break;

		case 'U':
		letterID[0] = 23;
		letterID[1] = 54;
		break;

		case 'V':
		letterID[0] = 24;
		letterID[1] = 55;
		break;

		case 'W':
		letterID[0] = 25;
		letterID[1] = 56;
		break;

		case 'X':
		letterID[0] = 26;
		letterID[1] = 57;
		break;


		case 'Y':
		letterID[0] = 27;
		letterID[1] = 58;
		break;

		case 'Z':
		letterID[0] = 28;
		letterID[1] = 59;
		break;

		

		case 'a':
		letterID[0] = 64;
		letterID[1] = 95;
		break;

		case 'b':
		letterID[0] = 65;
		letterID[1] = 96;
		break;

		case 'c':
		letterID[0] = 66;
		letterID[1] = 97;
		break;

		case 'd':
		letterID[0] = 67;
		letterID[1] = 98;
		break;

		case 'e':
		letterID[0] = 68;
		letterID[1] = 99;
		break;

		case 'f':
		letterID[0] = 69;
		letterID[1] = 100;
		break;

		case 'g':
		letterID[0] = 70;
		letterID[1] = 101;
		break;

		case 'h':
		letterID[0] = 71;
		letterID[1] = 102;
		break;

		case 'i':
		letterID[0] = 72;
		letterID[1] = 103;
		break;

		case 'j':
		letterID[0] = 73;
		letterID[1] = 104;
		break;

		case 'k':
		letterID[0] = 74;
		letterID[1] = 105;
		break;

		case 'l':
		letterID[0] = 75;
		letterID[1] = 106;
		break;

		case 'm':
		letterID[0] = 76;
		letterID[1] = 107;
		break;

		case 'n':
		letterID[0] = 77;
		letterID[1] = 108;
		break;

		case 'o':
		letterID[0] = 78;
		letterID[1] = 109;
		break;

		case 'p':
		letterID[0] = 79;
		letterID[1] = 110;
		break;

		case 'q':
		letterID[0] = 80;
		letterID[1] = 111;
		break;

		case 'r':
		letterID[0] = 81;
		letterID[1] = 112;
		break;

		case 's':
		letterID[0] = 82;
		letterID[1] = 113;
		break;

		case 't':
		letterID[0] = 83;
		letterID[1] = 114;
		break;

		case 'u':
		letterID[0] = 84;
		letterID[1] = 115;
		break;

		case 'v':
		letterID[0] = 85;
		letterID[1] = 116;
		break;

		case 'w':
		letterID[0] = 86;
		letterID[1] = 117;
		break;

		case 'x':
		letterID[0] = 87;
		letterID[1] = 118;
		break;


		case 'y':
		letterID[0] = 88;
		letterID[1] = 119;
		break;

		case 'z':
		letterID[0] = 89;
		letterID[1] = 120;
		break;




		case '>':
		letterID[0] = 29;
		letterID[1] = 60;
		break;

		case '.':
		letterID[0] = 01;
		letterID[1] = 61;
		break;

		case '!':
		letterID[0] = 53;
		letterID[1] = 61;
		break;


	}

	DrawTile(posX,posY,letterID[0],0,0,pal,layer);
	DrawTile(posX,posY + 1,letterID[1],0,0,pal,layer);
}

//Writes whole sentences to a layer.
void DrawText(int posX, int posY, char* message, int pal, int layer){
	for(int i = 0; i < 8; i++)  DrawChar(posX + i,posY,message[i],pal,layer);
}

//Draws the QWERTY alphabet for keyboard use.
////\param caps determines wether the letters should be in capitals or not.
void DrawKeyboard(bool caps){
	if(caps){
		DrawChar(5, 12,'Q',0,0);
		DrawChar(7, 12,'W',0,0);
		DrawChar(9, 12,'E',0,0);
		DrawChar(11, 12,'R',0,0);
		DrawChar(13, 12,'T',0,0);
		DrawChar(15, 12,'Y',0,0);
		DrawChar(17, 12,'U',0,0);
		DrawChar(19, 12,'I',0,0);
		DrawChar(21, 12,'O',0,0);
		DrawChar(23, 12,'P',0,0);
		
		DrawChar(5, 14,'A',0,0);
		DrawChar(7, 14,'S',0,0);
		DrawChar(9, 14,'D',0,0);
		DrawChar(11, 14,'F',0,0);
		DrawChar(13, 14,'G',0,0);
		DrawChar(15, 14,'H',0,0);
		DrawChar(17, 14,'J',0,0);
		DrawChar(19, 14,'K',0,0);
		DrawChar(21, 14,'L',0,0);

		DrawChar(5, 16,'Z',0,0);
		DrawChar(7, 16,'X',0,0);
		DrawChar(9, 16,'C',0,0);
		DrawChar(11, 16,'V',0,0);
		DrawChar(13, 16,'B',0,0);
		DrawChar(15, 16,'N',0,0);
		DrawChar(17, 16,'M',0,0);
	}
	else{

		DrawChar(5, 12,'q',0,0);
		DrawChar(7, 12,'w',0,0);
		DrawChar(9, 12,'e',0,0);
		DrawChar(11, 12,'r',0,0);
		DrawChar(13, 12,'t',0,0);
		DrawChar(15, 12,'y',0,0);
		DrawChar(17, 12,'u',0,0);
		DrawChar(19, 12,'i',0,0);
		DrawChar(21, 12,'o',0,0);
		DrawChar(23, 12,'p',0,0);
		
		DrawChar(5, 14,'a',0,0);
		DrawChar(7, 14,'s',0,0);
		DrawChar(9, 14,'d',0,0);
		DrawChar(11, 14,'f',0,0);
		DrawChar(13, 14,'g',0,0);
		DrawChar(15, 14,'h',0,0);
		DrawChar(17, 14,'j',0,0);
		DrawChar(19, 14,'k',0,0);
		DrawChar(21, 14,'l',0,0);

		DrawChar(5, 16,'z',0,0);
		DrawChar(7, 16,'x',0,0);
		DrawChar(9, 16,'c',0,0);
		DrawChar(11, 16,'v',0,0);
		DrawChar(13, 16,'b',0,0);
		DrawChar(15, 16,'n',0,0);
		DrawChar(17, 16,'m',0,0);

	}
}

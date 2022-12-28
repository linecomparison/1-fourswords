#include "base.h"

//Keps track of certain file variables so the game doesn't need to constantly load data.
typedef struct FileData{
	u8 fileName[8];			//The name given to the file in question.
	u8 entX[20], entY[20];	//The coordinates for each given sprite.
	
	u8 playerPal;				//Tunic colour index of the player.
	u8 currentMap;			//The map the file was saved on.
	u8 init;				//If this file was initialised this will be 1.
	
}ALIGN4 FileData;

//A struct that handles global variables.
typedef struct GameData{
	bool paused;							//Stores wether the game is currently paused.
	int currentPlr;							//Which sprite the player has control over.
	
	bool init[10];							//Whenever a manager needs initialised, one of these are used.
	bool faded[2];							//Determines if the fading out transition has completed.
	bool fadeIn;							//Allows the fade in effects to function.
	bool fadeOut;
	bool resetting;

	int inputType;							//Depending on the context the controls may change.
	int scrollType;							//Camera style changes depending what this is set to.
	int currentMap; 						//The currently loaded Level. 					
	int fileNo;								//The file number selected.						

	int tilesCount;
	int timer[10];
	int tick[10];							//The total tile count.
}ALIGN4 GameData;

//A struct that stores data for a given entity.
typedef struct Entity{

	OBJ_ATTR *sprite; // The array data of the sprite.
	int index; //The index number the sprite is given upon creation.
	
	int frames;					//For Admin purposes, stores the amount of animation frames this sprite has.		
	int colours;				//For Admin purposes, stores the amount of palettes are compatible with this sprite.
	
	int startFrame;				//For Admin purposes, stores the first frame of this sprite.
	int context;
	int steps; 					//Stores the amount of steps this sprite must take to get to it's destination.
	int directions; 			//Stores the amount of turns this sprite must take to get to it's destination.

 
	int frame;					//The index of the frame this sprite is set to.
	int tile_size;				//The size of the frames that make up this sprite.		
	int palette;				//Stores which palette bank this sprite is using.	
	int shape; 					//Stores data for the shape of this sprite.
	int size;					//Stores the array size of the given sprite.	
	char currentDirection; 		//Stores which direction the sprite is facing.	
	int startAnim;				//Used to determine the starting frame of any given animation.
	int state; 					//Stores the current animation type being played.	
	int prio; 					//Stores the layer the sprite is in.
	bool stuck;					//Used for collision scenarios to prevent the entity from moving.
	bool levelSelected;
				
	int screenNo[2];
	int mapPosition_X;
	int mapPosition_Y;
	int plrSprites;
	int global_x,	global_y;	//Positional coordinates within the level.
 	int local_x,	local_y;	//Positional coordinates within the boundaries of the screen.
	int playerPosX, playerPosY;	//Positional coordinates based on tile map number.

	int idleTimer, idleTick;	//Tracks the amount of time a sprite has not animated.
	int animTimer, animTick;	//Tracks time so that animation can happen.

	int ani_walk_u,ani_walk_d,ani_walk_l,ani_walk_r; //Stores the frame at which the walking animation starts.
	short hitBoxUp,hitBoxDown,hitBoxLeft,hitBoxRight; // Stores what tiles in the collision map this sprite surrounded with.

	bool flippedX,	flippedY;	//Stores wether the sprite has been flipped or not.
	bool visible;


}ALIGN4 Entity;

typedef struct MapLayer{
int mapNo;
int mode;
int char_base;
int map_base;
int size;
int colors;
int priority;
int mosaic;
int overflow;
int scrollX; 			//The camera scroll coordinates.
int scrollY; 			//The camera scroll coordinates.
}ALIGN4 MapLayer;

//A Tileset for keeping track of used tiles.
typedef struct TileSet {	
	int charStart;
	int tilesCount;
	int tileSetNo;
	int tileIndex;
}ALIGN4 TileSet;

typedef struct LevelData{
int plrNo;				//Counts how many players are playing.
int botNo;				//Determines how many bots are spawned.

int mapHeight;			//The total height of the map.
int mapWidth;			//The total width of the map.

int scrollX;
int scrollY;

bool saving;
bool loaded;			//When a level loads this triggers to tell when it has begun and finished. (May be redundant due to fade in effects)

char scrolling;			//If the map is scrolling and is set to Zelda1 style, this will lead to the input being frozen.

int tileSetCount;		//The total amount of tilesets loaded at once.
int tilePalCount;		//The amount of palette banks that are used up.

int entNo;				//Counts how many entities are enabled.
int spritePalCount;

int screensX;
int screensY;
int doorCount;

int DoorCoordX[10];
int DoorCoordY[10];
bool doorLocked[10];
bool doorWarp[10];
char doorDir[10];

int warpCount;
int warpPointX[10];
int warpPointY[10];
int warpMap[10];
int warpDest[10];

}ALIGN4 LevelData;


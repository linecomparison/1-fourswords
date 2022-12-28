enum inputTypes{		
	inputMenu, 
	inputPlayer, 
	inputDialogue,
	inputKeyboard
};
enum layerTypes{		
	UI, 
	Overhead,
	Ground,
	Animations
};
enum constTypes{
	mapCopy,mapManual,mapPlan
};
enum scrollTypes{	
	None,
	fixedCamera,
	followCamera
};
enum stateTypes{	
	state_idle,
	state_walking,
	state_bot_walk,
	state_frozen
};

enum musicList{
	NoSound = -1, 
	Title,
	Tutorial,
	DefaultLevel
};
enum soundList{
	MenuConfirm,
	MenuError,
	MenuCursor
};
enum levelList{		
	pauseMap = -1, 
	fileMap, 
	titleMap,
	houseMap
};
enum paletteList{
	menuPal,
	menuDim,
	housePal
};
enum tilesetList{
	menuTiles,
	houseTiles
};
enum spriteList{
	numberChar = -1,
	mainChar,
};
enum dataList{ 
	dataName = 1,
	dataInit = 251,
	dataPlayerPal = 252,
	dataMap = 253, 
	dataEntX = 31, 
	dataEntY = 32, 
	dataEntD = 33
};

enum categories{	
	Game,
	Sound,
	Map,
	Sprites,
	Fading,
	Menus,
	Resets
};
enum directions{	
	up,
	down,
	left,
	right
};

enum cartTypes{
	cartSRAM,
	cartFLASH
};

#define cartType cartSRAM
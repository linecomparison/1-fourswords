#include "headers/project/game.h"


//What the game starts up in after the fade in effect.
void Startup(){	
   DefaultValues();           //Anything the game needs to reset back to default is stored here.
	SetLevel(0,0);             //Sets all of the necessary assets for the chosen level.
	LoadTilesets();            //Sets the tileset up to be used for the chosen level.
	ConstructMap(mapManual,0); //Tells the game how to load up the level geometry.
   
	game.fadeIn = true;
	level.loaded = true;
}


//Handles Game Initialisation
void GameManager() {

   #pragma region Startup Main
      if(!game.init[Game]){  game.init[Game] = true;
         REG_DISPCNT = 0;  //Reset the video channels that have been turned on.
         ResetMemory(3);   //Reset all necessary areas of memory during a soft reset or level.
         LoadFiles();      //Load up all saved data used during gameplay.
         Startup();        //Setup the level and video memory to be used.
      }
   #pragma endregion
   
   
   if(!game.faded[0] && !game.paused){
      UpdateSprites();  //Update sprites on each frame.
   }
   InputManager(MyMenu);   //Basic control manager.
   SoundManager();         //Basic sound manager.
}

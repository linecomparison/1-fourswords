#include "headers/project/main.h"

int main(){

	irqInit();
	irqSet(IRQ_VBLANK, mmVBlank);
	irqEnable(IRQ_VBLANK);

	while(1){
		key_poll();
		if(game.tick[Resets] == 0){
			if(key_is_down(KEY_L)) InitFiles();
			if(SaveData[dataInit] != 1 && SaveData[dataInit + 310] != 1) InitFiles();
		}
		GameManager(); Fade();
		VBlankIntrWait();
    }
}
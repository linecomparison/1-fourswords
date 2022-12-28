#include "headers/audio.h"

bool trackPlaying; 
int scndInstrument = 1;
int trackNo; 							//The music track being played.
int vol[3]; 							//The sound volume.
mm_sound_effect sounds[40];
mm_sfxhand sfxhands[40];

//(Currently Unused)
//Debug feature - Scroll through the many sound effects.
void ScrollSounds(){
	if(scndInstrument == 25)scndInstrument++;
	if(scndInstrument == 26)scndInstrument= 31;
	if(scndInstrument == 36)scndInstrument= 0;
	else scndInstrument++;
}

//(Currently Unused)
//music controls for playing music meant for a sound test like mode.
void MusicControls(int action){
 
}

//all it does for now is buffer the music.
void MusicPlayer() {
	mmFrame();
}

//play a specific sound.
void PlaySound(int sfx){	
	mmEffectEx(&sounds[sfx]);
}

//stop a specific sound.
//@param sfx  the soundID to stop, should be hooked to the corresponding enum
//@see soundList for more info. 
//@note test fns 
//@attention test test
//
void StopSound(int sfx){
	mmEffectCancel(sfxhands[sfx]);
}

//for sounds that loop, use this.
void PlaySoundLoop(int sfx){
	sfxhands[sfx] = mmEffectEx(&sounds[sfx]);
}

//initialises all sound.
void InitMusic(){
	mmInitDefault( (mm_addr)soundbank_bin, 8 );
	
	for( int i = 0; i != 38; i++){

		if(i		== MenuConfirm)	sounds[i].id = SFX_MENU_SELECT;
		else if(i	== MenuError)	sounds[i].id = SFX_MENU_ERROR;
		else if(i	== MenuCursor)	sounds[i].id = SFX_MENU_CURSOR;
		else sounds[i].id = i;
	
		sounds[i].rate = (int)(1.0f * (1 << 10));
		sounds[i].volume = 255;
		sounds[i].panning = 127;
	}
	game.init[Sound] = true;
	if(trackNo == -1) return;
}

//manages sound.
void SoundManager(){
	if(!game.init[Sound]) InitMusic();
	else MusicPlayer();
}
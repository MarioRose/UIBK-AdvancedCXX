#include <Room.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <Settings.h>
#include <stdio.h>
#include <string>

Room::Room() {
	SDL_Surface *background_surface = NULL;
	SDL_Texture *background_texture = NULL;
	SDL_Renderer *renderer;
	Mix_Music *music = NULL;

}

void Room::playMusic() {
	//Load music
	music = Mix_LoadMUS( "../../sound/172561_45941-lq.mp3" );

	if( music == NULL ) {
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	else {
		Mix_VolumeMusic(MIX_MAX_VOLUME);
		Mix_PlayMusic( music, -1 );
	}
}

Room::~Room(){
	//Free the music
	free();
}

void Room::free() {
	//Free the music
	if( music != NULL ) {
  		Mix_FreeMusic(music);
  		music = NULL;
  	}
}

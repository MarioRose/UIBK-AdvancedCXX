#include "sprite.h"
#include <string.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include "Texture.h"

Sprite::Sprite(){

}

Sprite::Sprite(int x, int y, std::string path, SDL_Renderer *renderer, SpriteType type)
            : GameObject(x,y,10,10), renderer(renderer), type(type) {
    cTexture.loadFromFile(path, renderer);
    cTexture.scaleToHeight(SCREEN_HEIGHT * 0.05);

    // Load music
	sound = Mix_LoadWAV("../../assets/music/money-001.wav");

	if (sound == NULL) {
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}

}

void Sprite::playSound() {
    // Load music
	if (sound != nullptr) {
		Mix_VolumeMusic(MIX_MAX_VOLUME);
		// Mix_PlayMusic( shout_sound, 1 );
		Mix_PlayChannel(-1, sound, 0);
	}
}

Sprite::~Sprite(){
    this->free();
    Mix_FreeChunk(sound);
    sound=NULL;
}

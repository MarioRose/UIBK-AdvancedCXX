#include "Sprite.h"
#include "Texture.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <string.h>
#include <vector>

Sprite::Sprite() {}

Sprite::Sprite(int x, int y, Texture &texture, SDL_Renderer *renderer, SpriteType type)
    : GameObject(x, y, 10, 10), renderer(renderer), type(type)
{
	cTexture = texture;
	if(type == SpriteType::STAR) {
		cTexture.scaleToHeight(SCREEN_HEIGHT * 0.05);

		// Load music
		sound = Mix_LoadWAV("assets/music/money-001.wav");
	} else if (type == SpriteType::BOW) {
		cTexture.scaleToHeight(SCREEN_HEIGHT * 0.1);

		// Load music
		sound = Mix_LoadWAV("assets/music/money-001.wav");
	}

	if (sound == NULL) {
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}
}

void Sprite::playSound()
{
	// Load music
	if (sound != nullptr) {
		Mix_VolumeMusic(MIX_MAX_VOLUME);
		// Mix_PlayMusic( shout_sound, 1 );
		Mix_PlayChannel(-1, sound, 0);
	}
}

Sprite::~Sprite()
{
	this->free();
	Mix_FreeChunk(sound);
	sound = NULL;
}

SpriteType Sprite::getSpriteType() {
	return type;
}

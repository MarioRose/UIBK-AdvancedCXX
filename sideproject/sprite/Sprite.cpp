#include "Sprite.h"
#include "Texture.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <string.h>
#include <vector>

Sprite::Sprite() {}

Sprite::Sprite(int x, int y, Texture &texture, SDL_Renderer *renderer, SpriteType type, Mix_Chunk *sound)
    : GameObject(x, SCREEN_HEIGHT - y, 10, 10), renderer(renderer), type(type), sound(sound)
{
	cTexture = texture;
	if(type == SpriteType::STAR) {
		cTexture.scaleToHeight(SCREEN_HEIGHT * 0.05);
	} else if (type == SpriteType::BOW) {
		cTexture.scaleToHeight(SCREEN_HEIGHT * 0.1);
	} else if (type == SpriteType::FLAG) {
		cTexture.scaleToHeight(SCREEN_HEIGHT * 0.1);
	}

//	if (sound == NULL) {
//		printf("Failed to load beat music2! SDL_mixer Error: %s\n", Mix_GetError());
//	}
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
}

SpriteType Sprite::getSpriteType() {
	return type;
}

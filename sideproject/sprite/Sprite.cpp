#include "Sprite.h"
#include "Texture.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <string.h>
#include <vector>

Sprite::Sprite() {}

Sprite::Sprite(int x, int y, Texture &texture, SDL_Renderer *renderer, SpriteType type, Mix_Chunk *sound, int roomIndex)
    : GameObject(x, SCREEN_HEIGHT - y, 10, 10), renderer(renderer), type(type), sound(sound), roomIndex(roomIndex)
{
	cTexture = texture;
	if (type == SpriteType::STAR) {
		cTexture.scaleToHeight(SCREEN_HEIGHT * 0.05);
	} else if (type == SpriteType::BOW) {
		cTexture.scaleToHeight(SCREEN_HEIGHT * 0.1);
	} else if (type == SpriteType::SILVERBOW) {
		cTexture.scaleToHeight(SCREEN_HEIGHT * 0.1);
	} else if (type == SpriteType::GOLDENBOW) {
		cTexture.scaleToHeight(SCREEN_HEIGHT * 0.1);
	} else if (type == SpriteType::HEART) {
		cTexture.scaleToHeight(SCREEN_HEIGHT * 0.05);
	} else if (type == SpriteType::FLAME) {
		cTexture.scaleToHeight(SCREEN_HEIGHT * 0.1);
	} else if (type == SpriteType::FLAG) {
		cTexture.scaleToHeight(SCREEN_HEIGHT * 0.1);
	} else if (type == SpriteType::JUMP) {
		cTexture.scaleToHeight(SCREEN_HEIGHT * 0.05);
	}
}

void Sprite::playSound()
{
	// Load music
	if (sound != nullptr) {
		Mix_VolumeMusic(MIX_MAX_VOLUME);
		Mix_PlayChannel(-1, sound, 0);
	}
}

void Sprite::showSprite(double x, double y, bool notAlreadyShown)
{
	this->posX = x;
	this->posY = y;
	if (notAlreadyShown)
		this->visible = true;
}

Sprite::~Sprite()
{
	this->free();
}

SpriteType Sprite::getSpriteType()
{
	return type;
}

void Sprite::reset()
{
    visible = true;
}
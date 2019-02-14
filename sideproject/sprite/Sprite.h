#ifndef SPRITE_H
#define SPRITE_H

#include "GameObject.h"
#include "Texture.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <Settings.h>
#include <string.h>
#include <vector>

enum class SpriteType { BOW, SILVERBOW, GOLDENBOW, STAR, FLAG, JUMP, HEART, FLAME, FIREITEM };

class Sprite : public GameObject {
  public:
	// Initializes the variables
	Sprite();

	Sprite(int x, int y, Texture &texture, SDL_Renderer *renderer, SpriteType type, Mix_Chunk *sound, int roomIndex);

	~Sprite();

	void playSound();

	void showSprite(double x, double y, bool notAlreadyShown);

	bool visible = true;
	int roomIndex;

	SpriteType getSpriteType();

  private:
	SDL_Renderer *renderer;
	SpriteType type;
	Mix_Chunk *sound;
};

#endif

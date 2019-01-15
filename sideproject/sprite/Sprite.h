#ifndef SPRITE_H
#define SPRITE_H

#include "GameObject.h"
#include "Texture.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <Settings.h>
#include <string.h>
#include <vector>

enum class SpriteType { BOW, STAR };

class Sprite : public GameObject {
  public:
	// Initializes the variables
	Sprite();

	Sprite(int x, int y, Texture &texture, SDL_Renderer *renderer, SpriteType type);

	~Sprite();

	void playSound();

	bool visible = true;

	SpriteType getSpriteType();

  private:
	SDL_Renderer *renderer;
	SpriteType type;

	Mix_Chunk *sound;
};

#endif

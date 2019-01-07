#ifndef SPRITE_H
#define SPRITE_H

#include <Settings.h>
#include <string.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include "Texture.h"
#include "GameObject.h"

enum class SpriteType {
    STAR
};

class Sprite: public GameObject {
public:

	//Initializes the variables
	Sprite();

    Sprite(int x, int y, std::string path, SDL_Renderer *renderer, SpriteType type);

	~Sprite();

    void playSound();

    bool visible = true;

private:
    SDL_Renderer* renderer;
    SpriteType type;

    Mix_Chunk *sound;

};

#endif

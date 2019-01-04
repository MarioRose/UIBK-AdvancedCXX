#ifndef SPRITE_H
#define SPRITE_H

#include <Settings.h>
#include <string.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include "Texture.h"
#include "GameObject.h"

class Sprite: public GameObject {
public:

	//Initializes the variables
	Sprite();

    Sprite(int x, int y, std::string path, SDL_Renderer *renderer);

	~Sprite();

    bool visible = true;

private:
    SDL_Renderer* renderer;

};

#endif

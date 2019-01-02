#ifndef ARROW_H
#define ARROW_H

#include <Moveable.h>
#include <Enemy.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <Texture.h>
#include <vector>

class Arrow {
public:
	// Initializes the variables
	Arrow(SDL_Renderer* renderer, SDL_RendererFlip flipType);

	~Arrow();

	void free();

	void render(SDL_Renderer *renderer);

	bool collisionDetection(Enemy* enemy);

	void shoot(int x, int y);

	// Scene textures
	Texture arrowTexture;

protected:
	int mX = 0;
	int mY = 0;
	bool active = false;
	SDL_RendererFlip flipType;
	bool loadTexture(SDL_Renderer *renderer);
};

#endif
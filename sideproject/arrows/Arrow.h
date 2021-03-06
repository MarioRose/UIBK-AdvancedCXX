#ifndef ARROW_H
#define ARROW_H

#include <Enemy.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <Texture.h>
#include <vector>

class Arrow {
public:

	explicit Arrow(SDL_RendererFlip flipType);

	~Arrow();

	void render(SDL_Renderer *renderer, Texture& arrowTexture);

	bool collisionDetection(IEnemy* enemy);

	void shoot(int x, int y, int speed, int strength);

	bool isActive() const;

private:
	int mX = 0;
	int mY = 0;
	bool active = false;
	int speed;
	int strength;
	SDL_RendererFlip flipType;
};

#endif
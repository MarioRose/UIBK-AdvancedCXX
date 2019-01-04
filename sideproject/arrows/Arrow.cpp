#include <Arrow.h>
#include <Player.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <Settings.h>
#include <exception>
#include <sstream>
#include <string>
#include <vector>

Arrow::Arrow(SDL_Renderer* renderer, SDL_RendererFlip flipType) : flipType{flipType} {
	loadTexture(renderer);
}

Arrow::~Arrow()
{
	free();
}

bool Arrow::loadTexture(SDL_Renderer *renderer)
{
	// Loading success flag
	bool success = true;

	arrowTexture.loadFromFile("assets/images/arrow.png", renderer);
	arrowTexture.scaleToWidth(35);
	return success;
}

void Arrow::shoot(int x, int y) {
	mX = x;
	mY = y;
	active = true;
}

void Arrow::render(SDL_Renderer *renderer)
{
	if(active) {
		arrowTexture.render(mX, mY, renderer, NULL, 0, NULL, flipType);
		if(flipType == SDL_FLIP_NONE) {
			mX += 5;
		} else {
			mX -= 5;
		}

	}
}

void Arrow::free()
{
	arrowTexture.free();
}

bool Arrow::collisionDetection(Enemy* enemy)
{
	if(active) {
		if (abs(mX - enemy->getPosX()) < 20) {
			if (abs(mY - enemy->getPosY()) < 35) {
				if (mX > enemy->getPosX()) {
					enemy->setPosX(enemy->getPosX()-50);
				}
				else {
					enemy->setPosX(enemy->getPosX()+50);
				}
				active = false;
				free();
			}
		}
	}

	//more intuitive to return false for no hit - true for hit
	return !active;
}

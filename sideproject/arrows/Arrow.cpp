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

Arrow::Arrow(SDL_RendererFlip flipType) : flipType{flipType} {}

Arrow::~Arrow() {}

void Arrow::shoot(int x, int y)
{
	mX = x;
	mY = y;
	active = true;
}

void Arrow::render(SDL_Renderer *renderer, Texture &arrowTexture)
{
	if (active) {
		arrowTexture.render(mX, mY, renderer, NULL, 0, NULL, flipType);
		if (flipType == SDL_FLIP_NONE) {
			mX += 5;
		} else {
			mX -= 5;
		}
	}
}

bool Arrow::collisionDetection(IEnemy *enemy)
{
	if (enemy->getStatus() == CharacterStatus::DEAD) {
		return false;
	}

	if (active) {
		if (abs(mX - enemy->getPosX()) < 20) {
			if (abs(mY - enemy->getPosY()) < 35) {
				if (mX > enemy->getPosX()) {
					enemy->setPosX(enemy->getPosX() - 10);
				} else {
					enemy->setPosX(enemy->getPosX() + 10);
				}
				enemy->loseHealth();
				active = false;
			}
		}
	}

	if(mX < -20 || mX > SCREEN_WIDTH){
	    active = false;
	}

	// more intuitive to return false for no hit - true for hit
	return !active;
}

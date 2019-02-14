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

void Arrow::shoot(int x, int y, int s, int st)
{
	mX = x;
	mY = y;
	active = true;
	speed = s;
	strength = st;
}

void Arrow::render(SDL_Renderer *renderer, Texture &arrowTexture)
{
	if (active) {
		arrowTexture.render(mX, mY, renderer, NULL, 0, NULL, flipType);
		if (flipType == SDL_FLIP_NONE) {
			mX += speed;
		} else {
			mX -= speed;
		}
		if(mX < -20 || mX > SCREEN_WIDTH){
			active = false;
		}
	}
}

bool Arrow::collisionDetection(IEnemy *enemy)
{
	if (enemy->getStatus() == CharacterStatus::DEAD) {
		return false;
	}

	if (active) {
		if (mX > enemy->getPosX() && mX < enemy->getPosX()+enemy->getWidth()) {
			if (mY > enemy->getPosY() && mY < enemy->getPosY()+enemy->getHeight()) {
				if (flipType != SDL_FLIP_NONE) {
					if(enemy->getPosX() - 10 < 0) {
						enemy->setPosX(0);
					} else {
						enemy->setPosX(enemy->getPosX() - 10);
					}
				} else {
					if(enemy->getPosX() + 10 > SCREEN_WIDTH) {
						enemy->setPosX(SCREEN_WIDTH);
					} else {
						enemy->setPosX(enemy->getPosX() + 10);
					}

				}
				enemy->loseHealth(strength);
				active = false;
			}
		}
	}

	// more intuitive to return false for no hit - true for hit
	return !active;
}

bool Arrow::isActive() const {
	return active;
}
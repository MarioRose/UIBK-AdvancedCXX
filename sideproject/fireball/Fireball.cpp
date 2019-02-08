#include <Fireball.h>
#include <Player.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <Settings.h>
#include <exception>
#include <sstream>
#include <string>
#include <vector>

Fireball::Fireball() {
}

Fireball::~Fireball() {
	fireballTexture.free();
}

void Fireball::loadTexture(SDL_Renderer *renderer) {
	fireballTexture.loadFromFile("assets/images/fireball.png", renderer);
	fireballTexture.scaleToWidth(35);
}

void Fireball::shoot(int x, int y, SDL_RendererFlip flipType)
{
	mX = x;
	mY = y;
	mFlipType = flipType;
	state = FireballState::CHANNELING;
}

void Fireball::render(SDL_Renderer *renderer, int playerX, int playerY, SDL_RendererFlip flipType)
{
	if (state == FireballState::CHANNELING) {
		channelingTicks++;
		if(flipType == SDL_FLIP_HORIZONTAL) {
			playerX = playerX-25;
		}
		fireballTexture.render(playerX, playerY, renderer, NULL, 0, NULL, flipType);
		if(channelingTicks % 5 == 0) {
			fireballTexture.scaleToWidth(fireballTexture.getWidth()+5);
		}
		if(fireballTexture.getWidth() == 65) {
			mX = playerX;
			mY = playerY;
			state = FireballState::ACTIVE;
			mFlipType = flipType;
			channelingTicks = 0;
		}
	}
	if (state == FireballState::ACTIVE) {
		fireballTexture.render(mX, mY, renderer, NULL, 0, NULL, mFlipType);
		if (mFlipType == SDL_FLIP_NONE) {
			mX += 5;
		} else {
			mX -= 5;
		}
	}
	if(mX < 0 || mX > 640) {
		state = FireballState::INACTIVE;
		fireballTexture.scaleToWidth(25);
	}
}

void Fireball::collisionDetectionEnemies(std::vector<IEnemy*> enemies) {
	for (auto &enemy : enemies) {
		collisionDetection(enemy);
	}
}

void Fireball::collisionDetection(IEnemy *enemy)
{
	if (enemy->getStatus() != CharacterStatus::DEAD) {
		if (state == FireballState::ACTIVE) {
			if (abs(mX - enemy->getPosX()) < 20) {
				if (abs(mY - enemy->getPosY()) < 35) {
					if (mX > enemy->getPosX()) {
						enemy->setPosX(enemy->getPosX() - 20);
					} else {
						enemy->setPosX(enemy->getPosX() + 20);
					}
					enemy->loseHealth();
					enemy->loseHealth();
					enemy->loseHealth();
					state = FireballState::INACTIVE;
					fireballTexture.scaleToWidth(25);
				}
			}
		}
	}
}

FireballState Fireball::getState() const {
	return state;
}

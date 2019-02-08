#ifndef SIDESCROLLER_FIREBALL_H
#define SIDESCROLLER_FIREBALL_H

#include <Enemy.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <Texture.h>

enum class FireballState {INACTIVE, CHANNELING, ACTIVE};

class Fireball {
public:

	Fireball();

	~Fireball();

	void render(SDL_Renderer *renderer, int playerX, int playerY, SDL_RendererFlip flipType);

	void  loadTexture(SDL_Renderer *renderer);

	void collisionDetectionEnemies(std::vector<IEnemy*> enemies);
	void collisionDetection(IEnemy* enemy);

	void shoot(int x, int y, SDL_RendererFlip flipType);

	FireballState getState() const;

private:
	int mX = 0;
	int mY = 0;
	int channelingTicks = 0;
	FireballState state = FireballState::INACTIVE;
	Texture fireballTexture;
	SDL_RendererFlip mFlipType;
};

#endif //SIDESCROLLER_FIREBALL_H

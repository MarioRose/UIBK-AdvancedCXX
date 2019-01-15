#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Enemy.h"
#include "Sprite.h"
// The player that will move around on the screen

class Player : public Character {
  public:
	// Initializes the variables
	Player();

	~Player();

	// Takes key presses and adjusts the Character's velocity
	void control(SDL_Event &e);

	void collisionDetectionEnemies(std::vector<IEnemy *> enemies);
	bool collisionDetectionSprites(std::vector<Sprite *> sprites);

	void takeDamage();

	int getPoints();

	bool onLeftBorder();
	bool onRightBorder();
	bool onTopBorder();
	bool onBottomBorder();

	bool getHasBow() const;

  private:
	bool hasBow = false;
	int keypressCount = 0;
	int points = 0;

	void collisionDetection(IEnemy *enemy);
	bool collisionDetection(Sprite *sprite);
};

#endif

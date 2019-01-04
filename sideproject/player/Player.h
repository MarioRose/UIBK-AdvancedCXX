#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Enemy.h"
#include "sprite.h"
//The player that will move around on the screen

class Player: public Character {
public:

	//Initializes the variables
	Player();

	~Player();

	//Takes key presses and adjusts the Character's velocity
	void control( SDL_Event& e );

    void collisionDetectionEnemies(std::vector<Enemy*> enemies);
    void collisionDetectionSprites(std::vector<Sprite*> sprites);

	void takeDamage();

	int getLifeCount();
    int getPoints();

	bool onLeftBorder();
    bool onRightBorder();
    bool onTopBorder();
    bool onBottomBorder();

private:
	int keypressCount = 0;
	int lifeCount = 3;
    int points = 0;

    void collisionDetection(Enemy* enemy);
    void collisionDetection(Sprite* sprite);
};

#endif

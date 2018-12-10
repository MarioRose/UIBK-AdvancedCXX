#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Enemy.h"
//The player that will move around on the screen

class Player: public Character {
public:

	//Initializes the variables
	Player();

	~Player();

	//Takes key presses and adjusts the Character's velocity
	void control( SDL_Event& e );

    void collisionDetection(Enemy &enemy);

	void takeDamage();

	int getLifeCount();

private:
	int keypressCount = 0;
	int lifeCount = 3;

};

#endif

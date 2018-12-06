#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
//The player that will move around on the screen

class Player: public Character {
public:

	//Initializes the variables
	Player();

	~Player();

	//Takes key presses and adjusts the Character's velocity
	void control( SDL_Event& e );


private:
	//The force of the Character
	//double forceY;

	int keypressCount = 0;
};

#endif

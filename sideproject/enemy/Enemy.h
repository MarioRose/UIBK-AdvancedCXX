#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"

class Enemy : public Character {
  public:
	// Initializes the variables
	Enemy();

	~Enemy();

	void moveAI(Character *character);
};

#endif // ENEMY_H

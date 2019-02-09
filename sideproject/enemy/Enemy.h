#ifndef ENEMY_H
#define ENEMY_H

#include "IEnemy.h"

class Enemy : public IEnemy {
  public:
	// Initializes the variables
	Enemy();

	~Enemy();

	void moveAI(Character *character) override;

private:
    void melee(Character *character);
    void range(Character *character);
};

#endif // ENEMY_H

#ifndef ENEMY_H
#define ENEMY_H

#include <memory>
#include "IEnemy.h"
#include "Projectile_vert.h"

class Enemy : public IEnemy {
  public:
	// Initializes the variables
	Enemy();

	~Enemy();

	void moveAI(Character *character) override;
    void setProjectiles(std::shared_ptr<std::vector<Projectile_vert*>> projectiles);
    void setStartPos();
    void reset() override;

private:
    int startX;
    int startY;
    int startHealth;
    void melee(Character *character);
    void range(Character *character);
    std::shared_ptr<std::vector<Projectile_vert *>> projectiles;
};

#endif // ENEMY_H

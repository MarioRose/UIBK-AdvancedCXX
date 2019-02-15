#ifndef SIDESCROLLER_THIRDBOSS_H
#define SIDESCROLLER_THIRDBOSS_H

#include "../sprite/Sprite.h"
#include "Boss.h"
#include "IEnemy.h"
#include "Projectile.h"
#include <memory>

class ThirdBoss : public IEnemy {
  public:
	// Initializes the variables
	ThirdBoss();

	~ThirdBoss();

	void moveAI(Character *character) override;

	bool isGoingRight();
	void setIsGoingRight(bool right);
	void setProjectiles(std::shared_ptr<std::vector<Projectile *>> projectiles);
	void shootProjectiles();

	void setStartPos();
	void reset() override;

	bool isProjectile() const override;
    void freeItem() override;

  private:
	void attackTwo();

	AttackPhase currentPhase = AttackPhase::NONE;
	std::shared_ptr<std::vector<Projectile *>> projectiles;

	int startX;
	int startY;
	int startHealth;

	bool goingRight;
	bool itemNotShowed = true;
	void goBackAndForth();
	int frameCount = 0;
};

#endif // SIDESCROLLER_THIRDBOSS_H

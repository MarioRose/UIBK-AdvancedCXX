#ifndef SIDESCROLLER_BOSS_H
#define SIDESCROLLER_BOSS_H

#include "../sprite/Sprite.h"
#include "IEnemy.h"
#include "Projectile.h"
#include <memory>

enum AttackPhase { NONE, GOING, ONE, TWO };

class Boss : public IEnemy {
  public:
	// Initializes the variables
	Boss();

	~Boss();

	void moveAI(Character *character) override;

	bool isGoingRight();
	void setIsGoingRight(bool right);
	void setItem(Sprite *sprite);
	void setProjectiles(std::shared_ptr<std::vector<Projectile *>> projectiles);
	void addProjectile(Projectile *projectile);
	void shootProjectiles();

	void setStartPos();
	void reset() override;

	bool isProjectile() const override;
	void freeItem() override;

  private:
	void attackOne();
	void attackTwo();

	Sprite *item = nullptr;
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

#endif // SIDESCROLLER_BOSS_H

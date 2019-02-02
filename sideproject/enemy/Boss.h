#ifndef SIDESCROLLER_BOSS_H
#define SIDESCROLLER_BOSS_H

#include "IEnemy.h"
#include "../sprite/Sprite.h"

enum AttackPhase {NONE, GOING, ONE , TWO};

class Boss : public IEnemy {
  public:
	// Initializes the variables
	Boss();

	~Boss();

	void moveAI(Character *character) override;

	bool isGoingRight();
	void setIsGoingRight(bool right);
	void setItem(Sprite *sprite);

  private:
    void attackOne();
    void attackTwo();

    AttackPhase currentPhase = AttackPhase::NONE;
	bool goingRight;
	void goBackAndForth();
    Sprite *item = nullptr;
    bool itemNotShowed = true;
    int frameCount = 0;
};

#endif // SIDESCROLLER_BOSS_H

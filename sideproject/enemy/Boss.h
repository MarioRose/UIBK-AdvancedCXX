#ifndef SIDESCROLLER_BOSS_H
#define SIDESCROLLER_BOSS_H

#include "IEnemy.h"
#include "../sprite/Sprite.h"

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
	bool goingRight;
	void goBackAndForth();
    Sprite *item = nullptr;
    bool itemNotShowed = true;
};

#endif // SIDESCROLLER_BOSS_H

#ifndef SIDESCROLLER_BOSS_H
#define SIDESCROLLER_BOSS_H

#include "IEnemy.h"

class Boss : public IEnemy {
  public:
	// Initializes the variables
	Boss();

	~Boss();

	void moveAI(Character *character) override;

	bool isGoingRight();
	void setIsGoingRight(bool right);

  private:
	bool goingRight;
	void goBackAndForth();
};

#endif // SIDESCROLLER_BOSS_H

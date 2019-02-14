//
// Created by christoph on 15.01.19.
//

#ifndef SIDESCROLLER_IENEMY_H
#define SIDESCROLLER_IENEMY_H

#include "Character.h"

class IEnemy : public Character {
  public:
	virtual void moveAI(Character *character) = 0;
	virtual void reset() = 0;
};

#endif // SIDESCROLLER_IENEMY_H

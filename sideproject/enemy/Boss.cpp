#include "Boss.h"
#include "Settings.h"
#include <iostream>

Boss::Boss()
{
	flipType = SDL_FLIP_HORIZONTAL;
	clip = new SDL_Rect{0, 0, 60, 108};
}

bool Boss::isGoingRight()
{
	return goingRight;
}

void Boss::setIsGoingRight(bool right)
{
	goingRight = right;
}

void Boss::setItem(Sprite *sprite)
{
	this->item = sprite;
}

void Boss::goBackAndForth()
{
	status = CharacterStatus::RUNNING;
    currentPhase = AttackPhase::GOING;

	if (isGoingRight()) {
		if (posX >= SCREEN_WIDTH - 30) {
			setIsGoingRight(false);
		} else {
			posX += 3;
		}
	} else {
		if (posX < SCREEN_WIDTH - 200) {
			setIsGoingRight(true);
		} else {
			posX -= 3;
		}
	}

    frameCount++;
    if (frameCount == 70) {
        currentPhase = AttackPhase::NONE;
    }
}

void Boss::attackOne()
{
    currentPhase = AttackPhase::ONE;
    status = CharacterStatus::RUNNING;
    frameCount++;

    if (frameCount < 40) {
        return;
	}

    if (isGoingRight()) {
        if (posX >= SCREEN_WIDTH - 100) {
            currentPhase = AttackPhase::NONE;
        } else {
            posX += 5;
        }
    } else {
        if (posX < 0) {
            setIsGoingRight(true);
        } else {
            posX -= 8;
        }
    }

}

void Boss::attackTwo()
{
    currentPhase = AttackPhase::TWO;
    status = CharacterStatus::IDLE;

    if (frameCount < 50) {
        frameCount++;
        return;
    }

        currentPhase = AttackPhase::NONE;

}

void Boss::moveAI(Character *character)
{
	if (status == CharacterStatus::DEAD) {
		if (!itemNotShowed)
			return;
		item->showSprite(this->posX + (this->getWidth() / 2), this->posY + (this->getHeight() / 2), itemNotShowed);
		itemNotShowed = false;
		return;
	}

	if (status == CharacterStatus::DYING || status == CharacterStatus::ATTACK) {
		return;
	}

	if(currentPhase == AttackPhase::ONE){
        attackOne();
	} else if (currentPhase == AttackPhase::TWO) {
	    attackTwo();
	} else if (currentPhase == AttackPhase::GOING) {
        goBackAndForth();
    } else {
	    //choose new Attack
	    currentPhase = AttackPhase::NONE;
	    frameCount = 0;
        int randNum = rand() % 10;

        std::cout << randNum << std::endl;

        if (randNum < 6) {
            goBackAndForth();
        } else if (randNum < 8) {
            setIsGoingRight(false);
            attackOne();
        } else {
            attackTwo();
        }
	}
}
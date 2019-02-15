#include "Boss.h"
#include "Settings.h"
#include <iostream>

Boss::Boss()
{
	flipType = SDL_FLIP_HORIZONTAL;
	clip = new SDL_Rect{0, 0, 60, 108};
}

Boss::~Boss()
{
	item->free();
	delete item;
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

void Boss::setProjectiles(std::shared_ptr<std::vector<Projectile*>> projectiles)
{
    this->projectiles = projectiles;
}


void Boss::addProjectile(Projectile *projectile)
{
    this->projectiles->push_back(projectile);
}

void Boss::goBackAndForth()
{
	status = CharacterStatus::RUNNING;
    currentPhase = AttackPhase::GOING;

	if (isGoingRight()) {
		if (posX >= SCREEN_WIDTH - 100) {
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
        if (posX < 50) {
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

    shootProjectiles();

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

        if (randNum < 5) {
            goBackAndForth();
        } else if (randNum < 7) {
            setIsGoingRight(false);
            attackOne();
        } else {
            attackTwo();
        }
	}
}

void Boss::shootProjectiles()
{
    int force = 3;

    for(Projectile *p : *projectiles){
        p->setStartPosition(this->posX, this->posY, force);
        force += force;
    }
}

void Boss::setStartPos()
{
    startX = posX;
    startY = posY;
    startHealth = health;
}

void Boss::reset() {
	status = CharacterStatus::IDLE;
	posX = startX;
	posY = startY;
	health = startHealth;
}

bool Boss::isProjectile() const {
	return false;
}
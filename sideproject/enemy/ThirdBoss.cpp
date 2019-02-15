#include "ThirdBoss.h"
#include "Settings.h"
#include <iostream>

ThirdBoss::ThirdBoss()
{
    flipType = SDL_FLIP_HORIZONTAL;
    clip = new SDL_Rect{0, 0, 60, 108};
}

bool ThirdBoss::isGoingRight()
{
    return goingRight;
}

void ThirdBoss::setIsGoingRight(bool right)
{
    goingRight = right;
}

void ThirdBoss::setProjectiles(std::shared_ptr<std::vector<Projectile*>> projectiles)
{
    this->projectiles = projectiles;
}

void ThirdBoss::goBackAndForth()
{
    status = CharacterStatus::RUNNING;
    currentPhase = AttackPhase::GOING;

    if (isGoingRight()) {
        if (posX >= SCREEN_WIDTH - 230) {
            setIsGoingRight(false);
        } else {
            posX += 6;
        }
    } else {
        if (posX < 250) {
            setIsGoingRight(true);
        } else {
            posX -= 6;
        }
    }

    frameCount++;
    if (frameCount == 70) {
        currentPhase = AttackPhase::NONE;
    }
}

void ThirdBoss::attackTwo()
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

void ThirdBoss::moveAI(Character *character)
{
    if (status == CharacterStatus::DEAD) {
        character->setBeatGame(true);
        return;
    }

    if (status == CharacterStatus::DYING || status == CharacterStatus::ATTACK) {
        return;
    }

    if(character->getPosX() < ((posX+(posX+object_width))/2)){
        flipType = SDL_FLIP_NONE;
    } else {
        flipType = SDL_FLIP_HORIZONTAL;
    }

    if (currentPhase == AttackPhase::TWO) {
        attackTwo();
    } else if (currentPhase == AttackPhase::GOING) {
        goBackAndForth();
    } else {
        //choose new Attack
        currentPhase = AttackPhase::NONE;
        frameCount = 0;
        int randNum = rand() % 10;

        if (randNum < 7) {
            goBackAndForth();
        } else {
            attackTwo();
        }
    }
}

void ThirdBoss::shootProjectiles()
{
    int force1 = 1;
    int force2 = 1;

    int i = 0;

    for(Projectile *p : *projectiles){
        if(i%2==0){
            p->setStartPosition(this->posX, this->posY, force1, true);
            force1 += force1;
        }
        else{
            p->setStartPosition(this->posX, this->posY, force2, false);
            force2 += force2;
        }
        i++;
    }
}

void ThirdBoss::setStartPos()
{
    startX = posX;
    startY = posY;
    startHealth = health;
}

void ThirdBoss::reset() {
    status = CharacterStatus::IDLE;
    posX = startX;
    posY = startY;
    health = startHealth;
}

bool ThirdBoss::isProjectile() const {
    return false;
}

void ThirdBoss::freeItem() {
}
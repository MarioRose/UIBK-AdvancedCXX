#include "SecondBoss.h"
#include "Settings.h"
#include <iostream>

SecondBoss::SecondBoss()
{
//    flipType = SDL_FLIP_HORIZONTAL;
//    clip = new SDL_Rect{0, 0, 144, 160};
    posX = 300;
    posY = 200;
}

bool SecondBoss::isGoingUp()
{
    return goingUp;
}

void SecondBoss::setIsGoingUp(bool up)
{
    goingUp = up;
}

bool SecondBoss::isGoingRight()
{
    return goRight;
}

void SecondBoss::setGoRight(bool goRight)
{
    this->goRight = goRight;
}

void SecondBoss::setItem(Sprite *sprite)
{
    this->item = sprite;
}

void SecondBoss::setProjectiles(std::shared_ptr<std::vector<Projectile*>> projectiles)
{
    this->projectiles = projectiles;
}

void SecondBoss::goBackAndForth()
{
    status = CharacterStatus::RUNNING;
    currentPhase = AttackPhase::GOING;

    if (isGoingUp()) {
        if (posY <= 100) {
            setIsGoingUp(false);
        } else {
            posY -= 10;
        }
    } else {
        if (posY > SCREEN_HEIGHT - object_height - 50) {
            setIsGoingUp(true);
        } else {
            posY += 2;
        }
    }

    frameCount++;
    if (frameCount == 70) {
        currentPhase = AttackPhase::NONE;
    }
}

void SecondBoss::attackOne()
{
    currentPhase = AttackPhase::ONE;
    status = CharacterStatus::RUNNING;
    frameCount++;

    if (frameCount < 40) {
        posY -= 5;
        return;
    }

    if(isGoingRight()){
        if(returnToMiddle){
             if((posX+(posX+object_width))/2 > SCREEN_WIDTH/2){
                 posX -= 5;
             } else{
                 currentPhase = AttackPhase::NONE;
             }
        } else{
            if (posX >= SCREEN_WIDTH - object_width - 50) {
                returnToMiddle = true;
            } else {
                posX += 8;
            }
        }
    } else {
        if(returnToMiddle){
            if((posX+(posX+object_width))/2 < SCREEN_WIDTH/2){
                posX += 5;
            } else{
                currentPhase = AttackPhase::NONE;
            }
        } else{
            if (posX < 100) {
                returnToMiddle = true;
            } else {
                posX -= 8;
            }
        }
    }
    posY -= 5;
}

void SecondBoss::attackTwo()
{
    currentPhase = AttackPhase::TWO;
    status = CharacterStatus::RUNNING;

    posY -= 5;

    if (frameCount < 50) {
        frameCount++;
        return;
    }

    shootProjectiles();

    currentPhase = AttackPhase::NONE;

}

void SecondBoss::moveAI(Character *character)
{
    if(health <= 0){
        status = CharacterStatus::DEAD;
    }
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

    if(character->getPosX() < ((posX+(posX+object_width))/2)){
        flipType = SDL_FLIP_NONE;
    } else {
        flipType = SDL_FLIP_HORIZONTAL;
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
            if(flipType == SDL_FLIP_NONE)
                setGoRight(false);
            else
                setGoRight(true);
            returnToMiddle = false;
            attackOne();
        } else {
            attackTwo();
        }
    }
}

void SecondBoss::shootProjectiles()
{
    int force = 1;
    bool shootLeft;
    if(flipType == SDL_FLIP_NONE){
        shootLeft = true;
    } else {
        shootLeft = false;
    }

    for(Projectile *p : *projectiles){
        p->setStartPosition(this->posX, this->posY, force, shootLeft);
        force += force;
    }
}
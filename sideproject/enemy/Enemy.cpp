#include "Enemy.h"
#include "Settings.h"

Enemy::Enemy()
{ }

Enemy::~Enemy()
{ }

void Enemy::moveAI(Character *character)
{

    if(this->posX < 0) {
        this->direction = Direction::RIGHT;
    }
    else if(this->posX > SCREEN_WIDTH){
        this->direction = Direction::LEFT;
    }

    //if(rand() % 200 < 10){
        if(character->getPosX() > this->posX)
        {
            this->direction = Direction::RIGHT;
        }
        else
        {
            this->direction = Direction::LEFT;
        }
    //}

    switch(this->direction)
    {
        case Direction::RIGHT:
            this->posX += 0.75;
            break;
        case Direction::LEFT:
            this->posX -= 0.75;
            break;
    }


}

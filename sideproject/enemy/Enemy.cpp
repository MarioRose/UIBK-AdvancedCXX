#include "Enemy.h"

Enemy::Enemy()
{ }

Enemy::~Enemy()
{ }

void Enemy::moveAI(Character *character)
{

    if(character->getPosX() > this->posX)
    {
        this->posX += 0.75;
    }
    else
    {
        this->posX -= 0.75;
    }
}

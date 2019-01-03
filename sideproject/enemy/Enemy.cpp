#include "Enemy.h"
#include "Settings.h"

Enemy::Enemy()
{ }

Enemy::~Enemy()
{ }

void Enemy::moveAI(Character *character)
{

    status = CharacterStatus::RUNNING;

    if(posX < 0) {
        direction = Direction::RIGHT;
    }
    else if(this->posX > SCREEN_WIDTH){
        direction = Direction::LEFT;
    }

    if(rand() % 100 < 10){
        if(character->getPosX() > posX)
        {
            direction = Direction::RIGHT;
            flipType = SDL_FLIP_NONE;
        }
        else
        {
            direction = Direction::LEFT;
            flipType = SDL_FLIP_HORIZONTAL;
        }
    }

    switch(direction)
    {
        case Direction::RIGHT:
            posX += 0.75;
            break;
        case Direction::LEFT:
            posX -= 0.75;
            break;
    }

    if(contactWall) {
        changeDirection();
    }


}

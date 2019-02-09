#include "Enemy.h"
#include "Settings.h"

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::moveAI(Character *character)
{
	if(isRangeEnemy){
	    range(character);
	} else {
	    melee(character);
	}
}

void Enemy::melee(Character *character)
{
    if (status == CharacterStatus::DEAD || status == CharacterStatus::DYING || status == CharacterStatus::ATTACK) {
        return;
    }
    status = CharacterStatus::RUNNING;

    if (posX < 0) {
        direction = Direction::RIGHT;
    } else if (this->posX > SCREEN_WIDTH) {
        direction = Direction::LEFT;
    }

    if (std::abs(character->getPosY()+character->getHeight() - posY+object_height) < 8) {
        if (character->getPosX() > posX) {
            direction = Direction::RIGHT;
            flipType = SDL_FLIP_NONE;
        } else {
            direction = Direction::LEFT;
            flipType = SDL_FLIP_HORIZONTAL;
        }
        if (abs(character->getPosX() - posX) <= 25) {
            if(direction == Direction::LEFT) {
                posX -= 20;
            } else {
                posX += 20;
            }

            status = CharacterStatus::ATTACK;
        }
    }

    if (contactWall) {
        changeDirection();
    }

    switch (direction) {
        case Direction::RIGHT:
            posX += 0.75;
            break;
        case Direction::LEFT:
            posX -= 0.75;
            break;
    }
}

void Enemy::range(Character *character)
{
    if (status == CharacterStatus::DEAD || status == CharacterStatus::DYING || status == CharacterStatus::ATTACK) {
        return;
    }
    status = CharacterStatus::RUNNING;

    if (posX < 0) {
        direction = Direction::RIGHT;
    } else if (this->posX > SCREEN_WIDTH) {
        direction = Direction::LEFT;
    }

    if (std::abs(character->getPosY()+character->getHeight() - posY+object_height) < 8) {
        if (character->getPosX() > posX) {
            direction = Direction::RIGHT;
            flipType = SDL_FLIP_NONE;
        } else {
            direction = Direction::LEFT;
            flipType = SDL_FLIP_HORIZONTAL;
        }
        if (abs(character->getPosX() - posX) <= 25) {
            status = CharacterStatus::ATTACK;
        }
    }

}
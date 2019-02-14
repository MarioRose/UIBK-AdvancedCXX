#include <iostream>
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

    if (std::abs(character->getPosY()+character->getHeight() - (posY+object_height)) < 4) {
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
            posX += speed;
            break;
        case Direction::LEFT:
            posX -= speed;
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

    if (std::abs(character->getPosY()+character->getHeight() - (posY+object_height)) < 4) {
        if (character->getPosX() > posX) {
            direction = Direction::RIGHT;
            flipType = SDL_FLIP_NONE;
        } else {
            direction = Direction::LEFT;
            flipType = SDL_FLIP_HORIZONTAL;
        }
        if (abs(character->getPosX() - posX) <= 100) {

            int force = 75;

            for(Projectile_vert *p : *projectiles){
                if(p->getStatus() != CharacterStatus::DEAD){
                    continue;
                }
                if(direction == Direction::RIGHT){
                    p->setStartPosition(this->posX, this->posY, force, true);
                } else{
                    p->setStartPosition(this->posX, this->posY, force, false);
                }
                force += force;
            }

        }
    }


}

void Enemy::setProjectiles(std::shared_ptr<std::vector<Projectile_vert*>> projectiles)
{
    this->projectiles = projectiles;
}

void Enemy::setStartPos()
{
    startX = posX;
    startY = posY;
    startHealth = health;
}

void Enemy::reset() {
    status = CharacterStatus::IDLE;
    posX = startX;
    posY = startY;
    health = startHealth;
}

bool Enemy::isProjectile() const {
    return false;
}
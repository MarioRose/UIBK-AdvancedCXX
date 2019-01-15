#include "Enemy.h"
#include "Settings.h"

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::moveAI(Character *character)
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

	if (std::abs(character->getPosY() - posY) < 4) {
		if (character->getPosX() > posX) {
			direction = Direction::RIGHT;
			flipType = SDL_FLIP_NONE;
		} else {
			direction = Direction::LEFT;
			flipType = SDL_FLIP_HORIZONTAL;
		}
		if (abs(character->getPosX() - posX) <= 25) {
			posX -= 20;
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

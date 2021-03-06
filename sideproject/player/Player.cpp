#include <Character.h>
#include <Player.h>

#include "Enemy.h"
#include "Player.h"
#include "Sprite.h"

#include <SDL.h>
#include <SDL_image.h>
#include <Settings.h>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

Player::Player()
{
	setPosX(20);
	setPosY(400);
}

Player::~Player() {}

void Player::control(SDL_Event &e)
{
	// If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		// Adjust the velocity
		switch (e.key.keysym.sym) {
		// case SDLK_UP: mVelY -= CHAR_VEL; break;
		case SDLK_UP:
			keypressCount++;
			jump();
			break;
		case SDLK_DOWN:
			keypressCount++;
			velY += max_vel;
			break;
		case SDLK_LEFT:
			keypressCount++;
			flipType = SDL_FLIP_HORIZONTAL;
			velX -= max_vel;
			break;
		case SDLK_RIGHT:
			keypressCount++;
			flipType = SDL_FLIP_NONE;
			velX += max_vel;
			break;
		default:
			break;
		}
		if (keypressCount > 0) {
			if (status != CharacterStatus::FALLING) {
				status = CharacterStatus::RUNNING;
			}
		}
	}
	// If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
		// Adjust the velocity
		switch (e.key.keysym.sym) {
		case SDLK_UP:
			if(keypressCount > 0)
			    keypressCount--;
			break;
		case SDLK_DOWN:
            if(keypressCount > 0)
                keypressCount--;
			velY -= max_vel;
			break;
		case SDLK_LEFT:
            if(keypressCount > 0)
                keypressCount--;
			velX += max_vel;
			break;
		case SDLK_RIGHT:
            if(keypressCount > 0)
                keypressCount--;
			velX -= max_vel;
			break;
		default:
			break;
		}
		if (keypressCount == 0) {
			if (status != CharacterStatus::FALLING) {
				status = CharacterStatus::IDLE;
			}
		}
	}
}

void Player::move()
{
	// Move the character left or right
	posX += velX;

	// If the character went too far to the left or right
	if ((posX < 0) || (posX + object_width > SCREEN_WIDTH)) {
		// Move back
		posX -= velX;
	}

	// If the character went too far to the top
	if (posY < 0) {
		// Move back
		posY += 5;
	}

	// Jumping
	if (forceY > 0) {
		status = CharacterStatus::JUMPING;
		forceY--;
		posY -= 5;
	}

	// Falling
	if (!contactPlatform && forceY == 0 && posY <= SCREEN_HEIGHT) {
		status = CharacterStatus::FALLING;
		posY += 5;
	}

	if (contactPlatform && status == CharacterStatus::FALLING) {
		if (keypressCount > 0)
			status = CharacterStatus::RUNNING;
		else
			status = CharacterStatus::IDLE;
	}

	//if (contactWall) {
		//posX -= velX;
	//}

	if (posY > SCREEN_HEIGHT) {
		takeDamage();
		fallingDown = true;
		posX = lastSavePoint.x;
		posY = lastSavePoint.y;
	}
}

void Player::jump()
{
	if (status == CharacterStatus::JUMPING || status == CharacterStatus::FALLING) {
		if (getEquippedAbility() == EquippedAbility::JUMP && jumpCount == 0) {
			forceY = 15;
			jumpCount++;
			return;
		} else {
			return;
		}
	}
	status = CharacterStatus::JUMPING;
	contactPlatform = false;
	forceY = 15;
}

bool Player::collisionDetectionEnemies(std::vector<IEnemy *> enemies)
{

	bool collision = false;

	for (auto &enemy : enemies) {
		collision |= collisionDetection(enemy);
	}

	return collision;
}

bool Player::collisionDetectionSprites(std::vector<Sprite *> sprites, bool &flameCollision)
{

	bool collision = false;

	for (auto &sprite : sprites) {
		if (sprite->visible)
			collision |= collisionDetection(sprite, flameCollision);
	}

	return collision;
}

bool Player::collisionDetection(IEnemy *enemy)
{
	if (enemy->getStatus() == CharacterStatus::DEAD) {
		return false;
	}
        if (((posX+posX+object_width)/2 > enemy->getPosX()-15) && ((posX+posX+object_width)/2 < enemy->getPosX() + enemy->getWidth() +15))
		{
			if ((posY >= enemy->getPosY() && posY <= enemy->getPosY() + enemy->getHeight()) ||
                    (posY + object_height >= enemy->getPosY() &&
			        posY + object_height <= enemy->getPosY() + enemy->getHeight())) {
				takeDamage();

				return true;
			}
		}
	return false;
}

bool Player::collisionDetection(Sprite *sprite, bool &flameCollision)
{
	if (sprite->getSpriteType() == SpriteType::FLAME) {
		if (abs((posY + object_height) - sprite->getPosY()) < 15) {
			if (posX >= sprite->getPosX() && posX < sprite->getPosX() + 50) {
				if (equippedAbility != EquippedAbility::FIRE) {
					takeDamage();
					setPosX(lastSavePoint.x);
					setPosY(lastSavePoint.y);
					flameCollision = true;
					return true;
				}
			}
		}
	} else if (abs(posX - sprite->getPosX()) < 10) {
		if (abs(posY - sprite->getPosY()) < 15) {
			sprite->visible = false;
			sprite->playSound();

			switch (sprite->getSpriteType()) {
			case SpriteType::STAR:
				points++;
				break;
			case SpriteType::HEART:
				if (health < maxHealth) {
					health++;
				}
				break;
			case SpriteType::BOW:
				hasBow = true;
				if (equippedItem == EquippedItem::NONE)
					equippedItem = EquippedItem::BOW;
				break;
			case SpriteType::SILVERBOW:
				hasSilverBow = true;
				if (equippedItem == EquippedItem::NONE)
					equippedItem = EquippedItem::SILVERBOW;
				break;
			case SpriteType::GOLDENBOW:
				hasGoldenBow = true;
				if (equippedItem == EquippedItem::NONE)
					equippedItem = EquippedItem::GOLDENBOW;
				break;
			case SpriteType::FLAG:
				lastSavePoint.x = sprite->getPosX();
				lastSavePoint.y = sprite->getPosY();
				lastSavePoint.roomIndex = sprite->roomIndex;
				break;
			case SpriteType::JUMP:
				hasDoubleJumpItem = true;
				if (equippedAbility == EquippedAbility::NONE)
					equippedAbility = EquippedAbility::JUMP;
				break;
			case SpriteType::FIREITEM:
				hasFireImmunity = true;
				if (equippedAbility == EquippedAbility::NONE)
					equippedAbility = EquippedAbility::FIRE;
				break;
			}

			return true;
		}
	}
	return false;
}

void Player::takeDamage()
{
	this->loseHealth(1);
	this->shout();
}

int Player::getPoints()
{
	return points;
}

void Player::setLastSavePoint(int x, int y, int roomIndex)
{
	lastSavePoint.x = x;
	lastSavePoint.y = y;
	lastSavePoint.roomIndex = roomIndex;
}

void Player::setPoints(int p)
{
	points = p;
}

bool Player::onLeftBorder()
{
	return posX <= 5;
}

bool Player::onRightBorder()
{
	return posX >= (SCREEN_WIDTH - object_width - 5);
}

bool Player::onTopBorder()
{
	return posY <= 5;
}

bool Player::onBottomBorder()
{
	return posY >= (SCREEN_HEIGHT - object_height - 5);
}

bool Player::getHasBow() const
{
	return hasBow;
}

void Player::setHasBow(bool val)
{
	hasBow = val;
}

bool Player::getHasSilverBow() const
{
	return hasSilverBow;
}

void Player::setHasSilverBow(bool val)
{
	hasSilverBow = val;
}

bool Player::getHasGoldenBow() const
{
	return hasGoldenBow;
}

void Player::setHasGoldenBow(bool val)
{
	hasGoldenBow = val;
}

bool Player::hasDoubleJump() const
{
	return hasDoubleJumpItem;
}

bool Player::hasFireItem() const
{
	return hasFireImmunity;
}

void Player::setDoubleJumpItem(bool hasItem)
{
	hasDoubleJumpItem = hasItem;
}

void Player::setFireItem(bool hasItem)
{
	hasFireImmunity = hasItem;
}

EquippedItem Player::getEquippedItem() const
{
	return equippedItem;
}

void Player::setEquippedItem(EquippedItem item)
{
	this->equippedItem = item;
}

EquippedAbility Player::getEquippedAbility() const
{
	return equippedAbility;
}

void Player::setEquippedAbility(EquippedAbility ability)
{
	this->equippedAbility = ability;
}

void Player::increaseMaxHealth()
{
	maxHealth++;
}

bool Player::getFallingDown() const
{
	return fallingDown;
}

void Player::setFallingDown(bool val)
{
	fallingDown = val;
}

void Player::resetPlayer()
{
	posX = 20;
	posY = 400;

	fallingDown = false;

	hasBow = false;
	hasSilverBow = false;
	hasGoldenBow = false;

	hasDoubleJumpItem = false;
	hasFireImmunity = false;

	keypressCount = 0;
	points = 0;
	maxHealth = 5;
	health = maxHealth;

	velX = 0;
	velY = 0;
}

void Player::setKeypressCount(int count){
    this->keypressCount = count;
}

void Player::setStatus(CharacterStatus status)
{
    this->status = status;
}

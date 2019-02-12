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
			keypressCount--;
			break;
		case SDLK_DOWN:
			keypressCount--;
			velY -= max_vel;
			break;
		case SDLK_LEFT:
			keypressCount--;
			velX += max_vel;
			break;
		case SDLK_RIGHT:
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

	// Jumping
	if (forceY > 0) {
		status = CharacterStatus::JUMPING;
		forceY--;
		posY -= 5;
	}

	// Falling
	if (!contactPlatform && forceY == 0 && posY <= SCREEN_HEIGHT - object_height) {
		status = CharacterStatus::FALLING;
		posY += 5;
	}

	if (contactPlatform && status == CharacterStatus::FALLING) {
		if (keypressCount > 0)
			status = CharacterStatus::RUNNING;
		else
			status = CharacterStatus::IDLE;
	}

	if (contactWall) {
		posX -= velX;
	}

	if (posY > SCREEN_HEIGHT - object_height) {
		posY = SCREEN_HEIGHT - object_height;
	}

	// Jumping or Falling
	//    if(forceY != 0 || posY != SCREEN_HEIGHT - object_height) {
	//
	//        //Calculate Force
	//        if( posY + object_height < SCREEN_HEIGHT ) {
	//            forceY = forceY + GRAVITATION + ((SCREEN_HEIGHT - object_height) - posY)/4.5;
	//        }
	//
	//        //Calculate acceleration
	//        float a = forceY + velY;
	//
	//        //Calculate velocity
	//        velY = velY + a;
	//
	//        //Move the character up or down
	//        posY += velY;
	//
	//
	//        //Maximum Velocity downwards
	//        if(velY >= MAX_VELOCITY_DOWN) {
	//            velY = MAX_VELOCITY_DOWN;
	//        }
	//
	//        //If the character went too far up or down
	//        if( ( posY < 0 ) || ( posY + object_height >= SCREEN_HEIGHT ) ) {
	//            //Move back
	//            //mPosY -= mVelY;
	//            velY = 0;
	//            posY = SCREEN_HEIGHT - object_height;
	//        }
	//
	//        forceY = 0;
	//    }
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

bool Player::collisionDetectionSprites(std::vector<Sprite *> sprites)
{

	bool collision = false;

	for (auto &sprite : sprites) {
		if (sprite->visible)
			collision |= collisionDetection(sprite);
	}

	return collision;
}

bool Player::collisionDetection(IEnemy *enemy)
{
	if (enemy->getStatus() == CharacterStatus::DEAD) {
		return false;
	}
	if (abs(posX - enemy->getPosX()) < 20) {
		if (abs(posY - enemy->getPosY()) < 35) {
			takeDamage();
			//			if (posX > enemy->getPosX())
			//				posX += 50;
			//			else if (posX >= 50) {
			//				posX -= 50;
			//			} else {
			//				posX = 0;
			//			}
			return true;
		}
	}
	return false;
}

bool Player::collisionDetection(Sprite *sprite)
{
	if (sprite->getSpriteType() == SpriteType::FLAME) {
		if (abs((posY+object_height) - sprite->getPosY()) < 15) {
			if (posX >= sprite->getPosX() && posX < sprite->getPosX() + 50) {
				if (!hasFireImmunity) {
					takeDamage();
					setPosX(lastSavePoint.x);
					setPosY(lastSavePoint.y);
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
			case SpriteType::FLAG:
				lastSavePoint.x = sprite->getPosX();
				lastSavePoint.y = sprite->getPosY();
				lastSavePoint.roomIndex = sprite->roomIndex;
				break;
			case SpriteType::SPECIAL:
				hasDoubleJumpItem = true;
				if (equippedAbility == EquippedAbility::NONE)
					equippedAbility = EquippedAbility::JUMP;
				break;
			}

			return true;
		}
	}
	return false;
}

void Player::takeDamage()
{
	this->loseHealth();
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

bool Player::inSkyHole() const
{
	return posY >= (SCREEN_HEIGHT - 34);
}

bool Player::getHasBow() const
{
	return hasBow;
}

void Player::setHasBow(bool val)
{
	hasBow = val;
}

bool Player::hasDoubleJump() const
{
	return hasDoubleJumpItem;
}

void Player::setDoubleJumpItem(bool hasItem)
{
	hasDoubleJumpItem = hasItem;
}

EquippedItem Player::getEquippedItem() const
{
	return equippedItem;
}

EquippedAbility Player::getEquippedAbility() const
{
	return equippedAbility;
}

int Player::getMaxHealth() const
{
	return maxHealth;
}
void Player::increaseMaxHealth()
{
	maxHealth++;
}

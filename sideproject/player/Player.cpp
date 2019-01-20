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

Player::Player() {}

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
			if (posX > enemy->getPosX())
				posX += 50;
			else
				if(posX >= 50) {
					posX -= 50;
				} else {
					posX = 0;
				}
            return true;
		}
	}
    return false;
}

bool Player::collisionDetection(Sprite *sprite)
{
	if (abs(posX - sprite->getPosX()) < 10) {
		if (abs(posY - sprite->getPosY()) < 15) {
			sprite->visible = false;
			sprite->playSound();

            switch(sprite->getSpriteType()) {
                case SpriteType::STAR:
                    points++;
                    break;
                case SpriteType::BOW:
                    hasBow = true;
                    equippedItem = EquippedItem::BOW;
                    break;
                case SpriteType::FLAG:
                    lastSavePoint.x = sprite->getPosX();
                    lastSavePoint.y = sprite->getPosY();
                    lastSavePoint.roomIndex = sprite->roomIndex;
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

bool Player::inSkyHole() const {
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

EquippedItem Player::getEquippedItem() const
{
	return equippedItem;
}

#include <Character.h>
#include <Player.h>

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
#include "Enemy.h"

Player::Player()
{ }

Player::~Player()
{ }

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
			status = CharacterStatus::RUNNING;
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
			status = CharacterStatus::IDLE;
		}
	}
}

void Player::collisionDetection(Enemy &enemy) {
    if(abs(posX - enemy.getPosX()) < 20) {
        if (abs(posY - enemy.getPosY()) < 35) {
            takeDamage();
            if (posX > enemy.getPosX())
                posX += 50;
            else
                posX -= 50;
        }
    }
}


void Player::takeDamage(){
    lifeCount--;
    this->shout();
}

int Player::getLifeCount() {
    return lifeCount;
}

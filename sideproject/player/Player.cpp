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

Player::Player()
{ }

Player::~Player()
{ }

void Player::control(SDL_Event &e)
{
	// If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		keypressCount++;
		// Adjust the velocity
		switch (e.key.keysym.sym) {
		// case SDLK_UP: mVelY -= CHAR_VEL; break;
		case SDLK_UP:
			jump();
			break;
		case SDLK_DOWN:
			velY += max_vel;
			break;
		case SDLK_LEFT:
		    flipType = SDL_FLIP_HORIZONTAL;
			velX -= max_vel;
			break;
		case SDLK_RIGHT:
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
		keypressCount--;
		// Adjust the velocity
		switch (e.key.keysym.sym) {
		// case SDLK_UP: mVelY += DOT_VEL; break;
		case SDLK_DOWN:
			velY -= max_vel;
			break;
		case SDLK_LEFT:
			velX += max_vel;
			break;
		case SDLK_RIGHT:
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

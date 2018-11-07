#include "character.h"
#include "settings.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

Character::Character()
{
	//Initialize the offsets
	mPosX = 0;
	mPosY = 0;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;

	//Initialize the force
	mForceY = 0;

	//Initialize status of character
	status = 0;

  //Scene textures
  gCharacterTexture;

  gIdleCharacterTextures[11];
  gRunningCharacterTextures[8];

}

void Character::handleEvent( SDL_Event& e )
{
	//If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
	{
		//Adjust the velocity
		switch( e.key.keysym.sym )
		{
			//case SDLK_UP: mVelY -= CHAR_VEL; break;
			case SDLK_UP: jump(); break;
			case SDLK_DOWN: mVelY += CHAR_VEL; break;
			case SDLK_LEFT: mVelX -= CHAR_VEL; break;
			case SDLK_RIGHT: mVelX += CHAR_VEL; break;
			default: break;
		}
		status = 1;
	}
		//If a key was released
	else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
	{
		//Adjust the velocity
		switch( e.key.keysym.sym )
		{
			//case SDLK_UP: mVelY += DOT_VEL; break;
			case SDLK_DOWN: mVelY -= CHAR_VEL; break;
			case SDLK_LEFT: mVelX += CHAR_VEL; break;
			case SDLK_RIGHT: mVelX -= CHAR_VEL; break;
			default: break;
		}
		status = 0;
	}
}

void Character::jump(){
	mForceY = -1;
}

void Character::move()
{
	//Move the character left or right
	mPosX += mVelX;

	//If the character went too far to the left or right
	if( ( mPosX < 0 ) || ( mPosX + CHAR_WIDTH > SCREEN_WIDTH ) )
	{
		//Move back
		mPosX -= mVelX;
	}


	//Jumping of Falling
	if(mForceY != 0 || mPosY != SCREEN_HEIGHT - CHAR_HEIGHT){


		//Calculate Force
		if( mPosY + CHAR_HEIGHT < SCREEN_HEIGHT ){
			mForceY = mForceY + GRAVITATION + ((SCREEN_HEIGHT - CHAR_HEIGHT) - mPosY)/4.5;
		}

		//Calculate acceleration
		float a = mForceY + mVelY;

		//Calculate velocity
		mVelY = mVelY + a;

		//Move the character up or down
		mPosY += mVelY;


		//Maximum Velocity downwards
		if(mVelY >= MAX_VELOCITY_DOWN){
			mVelY = MAX_VELOCITY_DOWN;
		}

		//If the character went too far up or down
		if( ( mPosY < 0 ) || ( mPosY + CHAR_HEIGHT >= SCREEN_HEIGHT ) )
		{
			//Move back
			//mPosY -= mVelY;
			mVelY = 0;
			mPosY = SCREEN_HEIGHT - CHAR_HEIGHT;
		}

		mForceY = 0;
	}
}

int Character::getStatus()
{
    return status;
}


void Character::render(SDL_Renderer* gRenderer)
{
	 //Show the Character
   gCharacterTexture.render( mPosX, mPosY, gRenderer );
}

void Character::render(int spriteNumber, SDL_Renderer* gRenderer)
{
    //Show the Character
    switch (status){
        case 1: gRunningCharacterTextures[spriteNumber].render( mPosX, mPosY, gRenderer );
                break;
        default: gIdleCharacterTextures[spriteNumber].render( mPosX, mPosY, gRenderer );
                break;
    }
}

#include <Character.h>

#include <exception>
#include <iostream>
#include <Settings.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>


Character::Character() {
	//Initialize the offsets
	mPosX = 0;
	mPosY = 0;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;

	//Initialize the force
	mForceY = 0;

	//Initialize status of character
	status = CharacterStatus::IDLE;

}

Character::~Character() {
	free();
}

void Character::handleEvent( SDL_Event& e ) {
	//If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ) {
		//Adjust the velocity
		switch( e.key.keysym.sym ) {
			//case SDLK_UP: mVelY -= CHAR_VEL; break;
			case SDLK_UP: jump(); break;
			case SDLK_DOWN: mVelY += CHAR_VEL; break;
			case SDLK_LEFT: mVelX -= CHAR_VEL; break;
			case SDLK_RIGHT: mVelX += CHAR_VEL; break;
			default: break;
		}
		status = CharacterStatus::RUNNING;
	}
	//If a key was released
	else if( e.type == SDL_KEYUP && e.key.repeat == 0 ) {
		//Adjust the velocity
		switch( e.key.keysym.sym ) {
			//case SDLK_UP: mVelY += DOT_VEL; break;
			case SDLK_DOWN: mVelY -= CHAR_VEL; break;
			case SDLK_LEFT: mVelX += CHAR_VEL; break;
			case SDLK_RIGHT: mVelX -= CHAR_VEL; break;
			default: break;
		}
		status = CharacterStatus::IDLE;
	}
}

void Character::jump() {
	mForceY = -1;
}

void Character::move() {
	//Move the character left or right
	mPosX += mVelX;

	//If the character went too far to the left or right
	if( ( mPosX < 0 ) || ( mPosX + CHAR_WIDTH > SCREEN_WIDTH ) ) {
		//Move back
		mPosX -= mVelX;
	}


	//Jumping of Falling
	if(mForceY != 0 || mPosY != SCREEN_HEIGHT - CHAR_HEIGHT) {


		//Calculate Force
		if( mPosY + CHAR_HEIGHT < SCREEN_HEIGHT ) {
			mForceY = mForceY + GRAVITATION + ((SCREEN_HEIGHT - CHAR_HEIGHT) - mPosY)/4.5;
		}

		//Calculate acceleration
		float a = mForceY + mVelY;

		//Calculate velocity
		mVelY = mVelY + a;

		//Move the character up or down
		mPosY += mVelY;


		//Maximum Velocity downwards
		if(mVelY >= MAX_VELOCITY_DOWN) {
			mVelY = MAX_VELOCITY_DOWN;
		}

		//If the character went too far up or down
		if( ( mPosY < 0 ) || ( mPosY + CHAR_HEIGHT >= SCREEN_HEIGHT ) ) {
			//Move back
			//mPosY -= mVelY;
			mVelY = 0;
			mPosY = SCREEN_HEIGHT - CHAR_HEIGHT;
		}

		mForceY = 0;
	}
}

CharacterStatus Character::getStatus() {
    return status;
}

bool Character::loadTexture(std::string path, SDL_Renderer* renderer) {

	// Load Character texture
	if (!cTexture.loadFromFile(path, renderer)) {
		return false;
	}

	return true;
}

bool Character::loadIdleTextures(std::vector<std::string> paths, SDL_Renderer* renderer) {
	// Loading success flag
	bool success = true;

	for(auto path : paths) {

		auto texture = new Texture;

		if(texture->loadFromFile(path, renderer))
		{
			idleTextures.push_back(texture);
		}
		else
		{
			success = false;
			break;
		}
	}

	return success;
}

bool Character::loadRunningTextures(std::vector<std::string> paths, SDL_Renderer* renderer) {
	// Loading success flag
	bool success = true;

	for(auto path : paths) {

		auto texture = new Texture;

		if(texture->loadFromFile(path, renderer))
		{
			runningTextures.push_back(texture);
		}
		else
		{
			success = false;
			break;
		}
	}

	return success;
}


void Character::render(SDL_Renderer* renderer) {
	//Show the Character
	cTexture.render( mPosX, mPosY, renderer );
}

void Character::render(int spriteNumber, SDL_Renderer* renderer) {
	//Show the Character
	switch (status){

    		case CharacterStatus::RUNNING:
    		//runningTextures.at(spriteNumber).render( mPosX, mPosY, renderer );
		//std::cout << "File Path: " << runningTextures[spriteNumber]->filePath << "\n";
			runningTextures[spriteNumber]->render( mPosX, mPosY, renderer );
        		break;

    		case CharacterStatus::IDLE:
		//idleTextures.at(spriteNumber).render( mPosX, mPosY, renderer );
		//std::cout << "File Path: " << idleTextures[spriteNumber]->filePath << "\n";
			idleTextures[spriteNumber]->render( mPosX, mPosY, renderer );
			break;

		default:
			throw std::exception{};
	}
}

void Character::free() {
	// Free loaded images
	//if(cTexture != NULL) {
		cTexture.free();
	//}

	for (auto texture = idleTextures.begin(); texture != idleTextures.end(); ++texture)
	{
		if ((*texture) != NULL)
		{
			(*texture)->free();
			delete (*texture);
		}
	}

	for (auto texture = runningTextures.begin(); texture != runningTextures.end(); ++texture)
	{
		if ((*texture) != NULL)
		{
			(*texture)->free();
			delete (*texture);
		}
	}
}

#include <Character.h>

#include <exception>
#include <iostream>
#include <Settings.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>



Character::Character() {
    //Initialize the offsets
    posX = 0;
    posY = 0;

    //Initialize the velocity
    velX = 0;
    velY = 0;

    //Initialize the force
    forceY = 0;

    //Initialize status of character
    status = CharacterStatus::IDLE;
}



Character::~Character() {
	free();
}

void Character::control( SDL_Event& e ) {
	//If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ) {
		//Adjust the velocity
		switch( e.key.keysym.sym ) {
			//case SDLK_UP: mVelY -= CHAR_VEL; break;
			case SDLK_UP: jump(); break;
			case SDLK_DOWN: velY += max_vel; break;
			case SDLK_LEFT: velX -= max_vel; break;
			case SDLK_RIGHT: velX += max_vel; break;
			default: break;
		}
		status = CharacterStatus::RUNNING;
	}
		//If a key was released
	else if( e.type == SDL_KEYUP && e.key.repeat == 0 ) {
		//Adjust the velocity
		switch( e.key.keysym.sym ) {
			//case SDLK_UP: mVelY += DOT_VEL; break;
			case SDLK_DOWN: velY -= max_vel; break;
			case SDLK_LEFT: velX += max_vel; break;
			case SDLK_RIGHT: velX -= max_vel; break;
			default: break;
		}
		status = CharacterStatus::IDLE;
	}
}

void Character::jump() {
	forceY = 15;
}


CharacterStatus Character::getStatus() {
	return status;
}

bool Character::loadTexture(std::string path, SDL_Renderer* renderer) {

	// Load Character texture
	if (cTexture.loadFromFile(path, renderer)) {
		return true;
	}

	return false;
}

void Character::loadFromFile(std::string path, SDL_Renderer *renderer) {

	std::ifstream file(path);

	if(file.is_open()){

    std::vector<std::string> pathsIdleTextures;
    std::vector<std::string> pathsRunningTextures;

		std::string line;
		while (std::getline(file, line))
		{
		    std::istringstream iss(line);
		    std::string key, value;
		    if (!(iss >> key >> value)) { continue; } // error
		    //std::cout << key << ": " << value << std::endl;

				if(key == "MAIN_IDLE")
				{
						loadTexture(value, renderer);
				}
				else if(key == "IDLE")
				{
						pathsIdleTextures.push_back(value);
				}
        else if(key == "RUNNING")
				{
						pathsRunningTextures.push_back(value);
				}
		}

    loadIdleTextures(pathsIdleTextures, renderer);
    loadRunningTextures(pathsRunningTextures, renderer);

		file.close();
	}
	else {
		std::cout << "Error loading file " << path << std::endl;
	}

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

void Character::render(int spriteNumber, SDL_Renderer* renderer) {
	//Show the Character
	switch (status){

		case CharacterStatus::RUNNING:
			//runningTextures.at(spriteNumber).render( mPosX, mPosY, renderer );
			//std::cout << "File Path: " << runningTextures[spriteNumber]->filePath << "\n";
			runningTextures[spriteNumber]->render( posX, posY, renderer );
			break;

		case CharacterStatus::IDLE:
			//idleTextures.at(spriteNumber).render( mPosX, mPosY, renderer );
			//std::cout << "File Path: " << idleTextures[spriteNumber]->filePath << "\n";
			idleTextures[spriteNumber]->render( posX, posY, renderer );
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

#include <Character.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <Settings.h>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

Character::Character() : shout_sound(nullptr)
{
	// Initialize the offsets
	posX = 6;
	posY = 0;

	// Initialize the velocity
	velX = 0;
	velY = 0;

	// Initialize the force
	forceY = 0;

	// Initialize status of character
	status = CharacterStatus::IDLE;

	flipType = SDL_FLIP_NONE;
}

Character::Character(double x, double y)
{
	// Initialize the offsets
	posX = x;
	posY = y;

	// Initialize the velocity
	velX = 0;
	velY = 0;

	// Initialize the force
	forceY = 0;

	// Initialize status of character
	status = CharacterStatus::IDLE;

	direction = Direction::RIGHT;

	flipType = SDL_FLIP_NONE;
}

Character::~Character()
{
	free();
}

void Character::jump()
{
    contactPlatform = false;
	forceY = 15;
}

CharacterStatus Character::getStatus()
{
	return status;
}

void Character::loadFromFile(std::string path, SDL_Renderer *renderer)
{

	std::ifstream file(path);
	if (file.is_open()) {

		std::vector<std::string> pathsIdleTextures;
		std::vector<std::string> pathsRunningTextures;

		std::string line;
		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::string key, value;
			if (!(iss >> key >> value)) {
				continue;
			} // error
			  // std::cout << key << ": " << value << std::endl;
			if (key == "HEIGHT") {
				this->setHeight(std::stoi(value));
			} else if (key == "WIDTH") {
				this->setWidth(std::stoi(value));
			} else if (key == "SOUND_SHOUT") {
				addSound(value, CharacterSoundType::SHOUT);
			} else if (key == "IDLE") {
				pathsIdleTextures.push_back(value);
			} else if (key == "RUNNING") {
				pathsRunningTextures.push_back(value);
			}
		}

		loadTextures(pathsIdleTextures, TextureType::IDLE, renderer);
		loadTextures(pathsRunningTextures, TextureType::RUNNING, renderer);

		file.close();
	} else {
		std::cout << "Error loading file " << path << std::endl;
	}
}

bool Character::loadTextures(std::vector<std::string> paths, TextureType texture_type, SDL_Renderer *renderer)
{
	// Loading success flag
	bool success = true;

	for (auto const path : paths) {

		auto texture = new Texture;

		if (texture->loadFromFile(path, renderer)) {

			switch (texture_type) {
			case TextureType::IDLE:
				idleTextures.push_back(texture);
				break;
			case TextureType::RUNNING:
				runningTextures.push_back(texture);
				break;
			}
		} else {
			success = false;
			break;
		}
	}

	return success;
}

void Character::nextSpriteIndex()
{
	spriteIndexRunning = ++spriteIndexRunning % runningTextures.size();
	spriteIndexIdle = ++spriteIndexIdle % idleTextures.size();
}

void Character::render(SDL_Renderer *renderer)
{
	// Show the Character
	this->nextSpriteIndex();

	switch (status) {

	case CharacterStatus::RUNNING:
		// runningTextures.at(spriteNumber).render( mPosX, mPosY, renderer );
		// std::cout << "File Path: " << runningTextures[spriteNumber]->filePath << "\n";
		runningTextures[spriteIndexRunning]->render(posX, posY, renderer, NULL, 0, NULL, flipType);
		break;

	case CharacterStatus::IDLE:
		// idleTextures.at(spriteNumber).render( mPosX, mPosY, renderer );
		// std::cout << "File Path: " << idleTextures[spriteNumber]->filePath << "\n";
		idleTextures[spriteIndexIdle]->render(posX, posY, renderer, NULL, 0, NULL, flipType);
		break;

	default:
		throw std::exception{};
	}
}

void Character::addSound(std::string path, CharacterSoundType sound_type)
{
	// Load music
	shout_sound = Mix_LoadWAV(path.c_str());

	if (shout_sound == NULL) {
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}
}

void Character::shout()
{
	// Load music
	if (shout_sound != nullptr) {
		Mix_VolumeMusic(MIX_MAX_VOLUME);
		// Mix_PlayMusic( shout_sound, 1 );
		Mix_PlayChannel(-1, shout_sound, 0);
	}
}

void Character::free()
{
	for (auto texture = idleTextures.begin(); texture != idleTextures.end(); ++texture) {
		if ((*texture) != NULL) {
			(*texture)->free();
		}
	}

	for (auto texture = runningTextures.begin(); texture != runningTextures.end(); ++texture) {
		if ((*texture) != NULL) {
			(*texture)->free();
		}
	}

    Mix_FreeChunk(shout_sound);
    shout_sound=NULL;
}

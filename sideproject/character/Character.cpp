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

void Character::move()
{
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
		if (status != CharacterStatus::DEAD && status != CharacterStatus::DYING) {
			status = CharacterStatus::FALLING;
		}
		posY += 5;
	}

	if (contactPlatform && status == CharacterStatus::FALLING) {
		status = CharacterStatus::RUNNING;
	}

	if (contactWall) {
		posX -= velX;
	}

	if (posY > SCREEN_HEIGHT - object_height) {
        loseHealth(5);
        posY = SCREEN_HEIGHT;
    }
}

CharacterStatus Character::getStatus()
{
	return status;
}

bool Character::loadFromFile(std::string path, SDL_Renderer *renderer)
{
    bool isRangeCharacter = false;

	std::ifstream file(path);
	if (file.is_open()) {

		std::vector<std::string> pathsIdleTextures;
		std::vector<std::string> pathsRunningTextures;
		std::vector<std::string> pathsDyingTextures;
		std::vector<std::string> pathsAttackTextures;
		std::vector<std::string> pathsFallingTextures;
		std::vector<std::string> pathsJumpingTextures;

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
            } else if (key == "RANGE") {
                this->setRange(std::stoi(value));
                isRangeCharacter = true;
            } else if (key == "SOUND_SHOUT") {
				addSound(value, CharacterSoundType::SHOUT);
			} else if (key == "IDLE") {
				pathsIdleTextures.push_back(value);
			} else if (key == "RUNNING") {
				pathsRunningTextures.push_back(value);
			} else if (key == "DYING") {
				pathsDyingTextures.push_back(value);
			} else if (key == "ATTACK") {
				pathsAttackTextures.push_back(value);
			} else if (key == "FALLING") {
				pathsFallingTextures.push_back(value);
			} else if (key == "JUMPING") {
				pathsJumpingTextures.push_back(value);
			} else if (key == "HEALTH") {
				this->setHealth(std::stoi(value));
			} else if (key == "SPEED") {
                this->setSpeed(std::stof(value));
            }
		}

		loadTextures(pathsIdleTextures, TextureType::IDLE, renderer);
		loadTextures(pathsRunningTextures, TextureType::RUNNING, renderer);
		loadTextures(pathsDyingTextures, TextureType::DYING, renderer);
		loadTextures(pathsAttackTextures, TextureType::ATTACK, renderer);
		loadTextures(pathsFallingTextures, TextureType::FALLING, renderer);
		loadTextures(pathsJumpingTextures, TextureType::JUMPING, renderer);

		file.close();
	} else {
		std::cout << "Error loading file " << path << std::endl;
	}

	return isRangeCharacter;
}

bool Character::loadTextures(std::vector<std::string> paths, TextureType texture_type, SDL_Renderer *renderer)
{
	// Loading success flag
	bool success = true;

	for (auto const path : paths) {

		Texture *texture = new Texture;

		if (texture->loadFromFile(path, renderer)) {

			switch (texture_type) {
			case TextureType::IDLE:
				idleTextures.push_back(texture);
				break;
			case TextureType::RUNNING:
				runningTextures.push_back(texture);
				break;
			case TextureType::DYING:
				dyingTextures.push_back(texture);
				break;
			case TextureType::ATTACK:
				attackTextures.push_back(texture);
				break;
			case TextureType::FALLING:
				fallingTextures.push_back(texture);
				break;
			case TextureType::JUMPING:
				jumpingTextures.push_back(texture);
				break;
			}
		} else {
			success = false;
			std::cout << "ERROR Character::loadTextures" << '\n';
			break;
		}
	}

	return success;
}

void Character::nextSpriteIndex()
{
	switch (status) {
	case CharacterStatus::RUNNING:
		spriteIndexRunning = (spriteIndexRunning + 1) % runningTextures.size();
	case CharacterStatus::IDLE:
		spriteIndexIdle = (spriteIndexIdle + 1) % idleTextures.size();
	case CharacterStatus::DYING:
		spriteIndexDying = (spriteIndexDying + 1) % dyingTextures.size();
	case CharacterStatus::ATTACK:
		spriteIndexAttack = (spriteIndexAttack + 1) % attackTextures.size();
	case CharacterStatus::FALLING:
		spriteIndexFalling = (spriteIndexFalling + 1) % fallingTextures.size();
	case CharacterStatus::JUMPING:
		spriteIndexJumping = (spriteIndexJumping + 1) % jumpingTextures.size();
	case CharacterStatus::DEAD:
		break;
	}
}

void Character::render(SDL_Renderer *renderer)
{
	// Show the Character
	this->nextSpriteIndex();

	switch (status) {

	case CharacterStatus::RUNNING:
		// runningTextures.at(spriteNumber).render( mPosX, mPosY, renderer );
		// std::cout << "File Path: " << runningTextures[spriteNumber]->filePath << "\n";
		runningTextures[spriteIndexRunning]->render(posX, posY, renderer, clip, 0, NULL, flipType);
		break;

	case CharacterStatus::IDLE:
		// idleTextures.at(spriteNumber).render( mPosX, mPosY, renderer );
		// std::cout << "File Path: " << idleTextures[spriteNumber]->filePath << "\n";
		idleTextures[spriteIndexIdle]->render(posX, posY, renderer, clip, 0, NULL, flipType);
		break;

	case CharacterStatus::DEAD:
		// idleTextures.at(spriteNumber).render( mPosX, mPosY, renderer );
		// std::cout << "File Path: " << idleTextures[spriteNumber]->filePath << "\n";
		dyingTextures[dyingTextures.size() - 1]->render(posX, posY, renderer, clip, 0, NULL, flipType);
		break;

	case CharacterStatus::DYING:
		// idleTextures.at(spriteNumber).render( mPosX, mPosY, renderer );
		// std::cout << "File Path: " << idleTextures[spriteNumber]->filePath << "\n";
		dyingTextures[spriteIndexDying]->render(posX, posY, renderer, clip, 0, NULL, flipType);

		if (spriteIndexDying + 1 == dyingTextures.size()) {
			status = CharacterStatus::DEAD;
		}
		break;

	case CharacterStatus::ATTACK:
		// idleTextures.at(spriteNumber).render( mPosX, mPosY, renderer );
		// std::cout << "File Path: " << idleTextures[spriteNumber]->filePath << "\n";
		attackTextures[spriteIndexAttack]->render(posX, posY, renderer, clip, 0, NULL, flipType);

		if (spriteIndexAttack + 1 == attackTextures.size()) {
			setWidth(getWidth() - 20);
			status = CharacterStatus::IDLE;
		}
		break;

	case CharacterStatus::FALLING:
		// idleTextures.at(spriteNumber).render( mPosX, mPosY, renderer );
		// std::cout << "File Path: " << idleTextures[spriteNumber]->filePath << "\n";
		fallingTextures[spriteIndexFalling]->render(posX, posY, renderer, clip, 0, NULL, flipType);
		break;

	case CharacterStatus::JUMPING:
		// idleTextures.at(spriteNumber).render( mPosX, mPosY, renderer );
		// std::cout << "File Path: " << idleTextures[spriteNumber]->filePath << "\n";
		jumpingTextures[spriteIndexJumping]->render(posX, posY, renderer, clip, 0, NULL, flipType);
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
	for(auto texture : idleTextures) {
		if(texture != NULL) {
			texture->free();
			delete texture;
		}
	}

	for(auto texture : runningTextures) {
		if(texture != NULL) {
			texture->free();
			delete texture;
		}
	}

	for(auto texture : dyingTextures) {
		if(texture != NULL) {
			texture->free();
			delete texture;
		}
	}

	for(auto texture : attackTextures) {
		if(texture != NULL) {
			texture->free();
			delete texture;
		}
	}

	for(auto texture : fallingTextures) {
		if(texture != NULL) {
			texture->free();
			delete texture;
		}
	}

	for(auto texture : jumpingTextures) {
		if(texture != NULL) {
			texture->free();
			delete texture;
		}
	}

	delete clip;

	Mix_FreeChunk(shout_sound);
	shout_sound = NULL;
}

void Character::loseHealth(int strength)
{
    if(status == CharacterStatus::DYING || status == CharacterStatus::DEAD)
        return;
	this->health -= strength;
	if (health <= 0) {
        shout();
        status = CharacterStatus::DYING;
		// setHeight(1);
	}
}

int Character::getHealth()
{
	return health;
}

bool Character::isAlive()
{
	return getHealth() > 0;
}

void Character::setHealth(int health)
{
	this->health = health;
}

void Character::setDeath()
{
	this->health = 0;
        status = CharacterStatus::DEAD;
}

bool Character::contactsPlatform()
{
	return contactPlatform;
}

void Character::setContactPlatform(bool b)
{
    if(b)
        jumpCount = 0;
	contactPlatform = b;
}

bool Character::contactsWall()
{
	return contactWall;
}

void Character::setContactWall(bool b)
{
	contactWall = b;
}

void Character::changeDirection()
{
	switch (direction) {
	case Direction::RIGHT:
		direction = Direction::LEFT;
		break;
	case Direction::LEFT:
		direction = Direction::RIGHT;
		break;
	}
	contactWall = false;
}

SDL_RendererFlip Character::getFlipType() const
{
	return flipType;
}

void Character::setRange(int isRange)
{
    isRangeEnemy = isRange;
}

void Character::setSpeed(float speed)
{
    this->speed = speed;
}

void Character::setVelX(int vel)
{
    this->velX = vel;
}

void Character::setBeatGame(bool val)
{
    this->beatGame = val;
}

bool Character::hasBeatGame()
{
    return beatGame;
}

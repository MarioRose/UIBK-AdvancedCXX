#include <Room.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <Settings.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

Room::Room() : background_surface(nullptr), background_texture(nullptr), music(nullptr), danger_music(nullptr)
{
	// init Tiles
	for (int i = 0; i < SCREEN_WIDTH / Tile::TILE_WEIGHT; i++) {
		tiles.emplace_back(Tile(i * Tile::TILE_WEIGHT, SCREEN_HEIGHT - Tile::TILE_HEIGHT, Tile::TILE_GROUND));
	}
	for (int i = 0; i < SCREEN_WIDTH / Tile::TILE_WEIGHT; i++) {
		tiles.emplace_back(Tile(i * Tile::TILE_WEIGHT, SCREEN_HEIGHT - Tile::TILE_HEIGHT * 2, Tile::TILE_PLATFORM));
	}
	for (int i = 0; i < 15; i++) {
		tiles.emplace_back(
		    Tile(150 + i * Tile::TILE_WEIGHT, SCREEN_HEIGHT - Tile::TILE_HEIGHT * 6, Tile::TILE_PLATFORM));
	}
	for (int i = 0; i < 7; i++) {
		tiles.emplace_back(
		    Tile(374 + i * Tile::TILE_WEIGHT, SCREEN_HEIGHT - Tile::TILE_HEIGHT * 10, Tile::TILE_PLATFORM));
	}
	for (int i = 0; i < 7; i++) {
		tiles.emplace_back(
		    Tile(200 + i * Tile::TILE_WEIGHT, SCREEN_HEIGHT - Tile::TILE_HEIGHT * 14, Tile::TILE_PLATFORM));
	}
	for (int i = 0; i < 3; i++) {
		tiles.emplace_back(Tile(374, SCREEN_HEIGHT - 7*Tile::TILE_HEIGHT - i * Tile::TILE_HEIGHT, Tile::TILE_WALL));
	}
}

void Room::loadFromFile(std::string path, SDL_Renderer *renderer)
{

	std::ifstream map(path);

	if (map.is_open()) {

		std::string line;

		while (std::getline(map, line)) {
			std::istringstream iss(line);
			std::string key, value;
			if (!(iss >> key >> value)) {
				continue;
			} // error
			  // std::cout << key << ": " << value << std::endl;

			if (key == "MUSIC") {
				loadMusic(value, RoomSoundType::NORMAL);
				playMusic(this->music);
			} else if (key == "DANGER_MUSIC") {
				loadMusic(value, RoomSoundType::DANGER);
			} else if (key == "BACKGROUND") {
				loadBackground(value, renderer);
			} else if (key == "ENEMY") {
				addEnemy(value, renderer);
			} else if (key == "ENEMY_POS") {
				setEnemyPos(value);
			}
		}

		map.close();

	} else {
		std::cout << "Error loading map " << path << std::endl;
	}
}

void Room::playMusic(Mix_Music *music)
{
	Mix_VolumeMusic(MIX_MAX_VOLUME);
	Mix_PlayMusic(music, -1);
}

void Room::loadMusic(std::string path, RoomSoundType sound_type)
{
	// Load music

	switch (sound_type) {
	case RoomSoundType::NORMAL:
		music = Mix_LoadMUS(path.c_str());
		if (music == NULL) {
			printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		}
		break;
	case RoomSoundType::DANGER:
		danger_music = Mix_LoadMUS(path.c_str());
		if (danger_music == NULL) {
			printf("Failed to load beat danger_music! SDL_mixer Error: %s\n", Mix_GetError());
		}
		break;
	}
}

void Room::loadBackground(std::string path, SDL_Renderer *renderer)
{
	background_surface = IMG_Load(path.c_str());
	background_texture = SDL_CreateTextureFromSurface(renderer, background_surface);
}

void Room::addEnemy(std::string path, SDL_Renderer *renderer)
{
	enemy.loadFromFile(path, renderer);
}

void Room::setEnemyPos(std::string value)
{
	std::stringstream ss(value);
	std::vector<std::string> result;

	while (ss.good()) {
		std::string substr;
		getline(ss, substr, ',');
		result.push_back(substr);
	}

	enemy.setPosX(std::stoi(result.at(0)));
	enemy.setPosY(SCREEN_HEIGHT + std::stoi(result.at(1)) - enemy.getHeight() - 350);
}

void Room::renderEnemies(SDL_Renderer *renderer)
{
	this->enemy.render(renderer);
}

bool Room::checkIfEnemiesInRoom()
{
	// TODO check positions
	return true;
}

void Room::renderTiles(SDL_Renderer *renderer, SDL_Texture *texture)
{
	for (auto &tile : tiles) {
		tile.render(renderer, texture);
	}
}

void Room::collisionTiles(Moveable *character)
{
	for (auto &tile : tiles) {
		if (character->getPosX() > tile.getX() - 1 && character->getPosX() < (tile.getX() + Tile::TILE_WEIGHT + 1)) {
			if (character->getPosY() + character->getHeight() > tile.getY() - 3 &&
			    character->getPosY() + character->getHeight() < (tile.getY() + 3)) {
				character->setPosY(tile.getY() - character->getHeight());
				character->setContactPlatform(true);
				return;
			}
		}
	}
	character->setContactPlatform(false);
}

Room::~Room()
{
	// Free the music
	free();
}

void Room::free()
{
	this->enemy.free();
	// Free the music
	// if( music != nullptr ) {
	//	Mix_FreeMusic(music);
	//	music = NULL;
	//}
}

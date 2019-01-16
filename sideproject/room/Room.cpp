#include <Room.h>

#include "Sprite.h"
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

Room::Room() : background_texture(nullptr), music(nullptr), danger_music(nullptr)
{
	initTiles();
}

Room::Room(int index, int indexLeft, int indexRight, int indexAbove, int indexBelow)
    : background_texture(nullptr), music(nullptr), danger_music(nullptr)
{
	roomIndex = index;
	roomIndexAbove = indexAbove;
	roomIndexBelow = indexBelow;
	roomIndexLeft = indexLeft;
	roomIndexRight = indexRight;

	initTiles();
}

// TODO: should parse file
void Room::initTiles()
{

	for (int i = 0; i < SCREEN_WIDTH / Tile::TILE_WEIGHT; i++) {
		tiles.emplace_back(Tile(i * Tile::TILE_WEIGHT, SCREEN_HEIGHT - Tile::TILE_HEIGHT, Tile::TILE_GROUND));
	}
	for (int i = 0; i < SCREEN_WIDTH / Tile::TILE_WEIGHT; i++) {
		tiles.emplace_back(Tile(i * Tile::TILE_WEIGHT, SCREEN_HEIGHT - Tile::TILE_HEIGHT * 2, Tile::TILE_PLATFORM));
	}
	for (int i = 0; i < SCREEN_WIDTH / Tile::TILE_WEIGHT; i++) {
		tiles.emplace_back(Tile(i * Tile::TILE_WEIGHT, 0, Tile::TILE_GROUND));
	}
}

void Room::loadFromFile(std::string path, SDL_Renderer *renderer)
{

	starTexture.loadFromFile("assets/images/sprites/star.png", renderer);
	bowTexture.loadFromFile("assets/images/sprites/bow.png", renderer);

	arrows.loadTexture(renderer);
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
			} else if (key == "DANGER_MUSIC") {
				loadMusic(value, RoomSoundType::DANGER);
			} else if (key == "BACKGROUND") {
				loadBackground(value, renderer);
			} else if (key == "TILES" && value == "BEGIN") {
				for (int i = 0; i < 20; i++) {
					std::getline(map, line);
					std::istringstream iss(line);

					if (!(iss >> key >> value)) {
						continue;
					}

					if (key == "PLATFORM") {
						addTile(value, Tile::TILE_PLATFORM);
					} else if (key == "WALL") {
						addTile(value, Tile::TILE_WALL);
					} else if (key == "HOLE") {
						removeTile(value);
					} else if (key == "SKYHOLE") {
						removeSkyTile(value);
					} else if (key == "TILES" && value == "END") {
						break;
					}
				}
			} else if (key == "ENTITIES" && value == "BEGIN") {
				for (int i = 0; i < 20; i++) {
					std::getline(map, line);
					std::istringstream iss(line);

					if (!(iss >> key >> value)) {
						continue;
					}

					if (key == "ENEMY") {
						addEnemy(value, renderer);
					}

					if (key == "BOSS") {
						addBoss(value, renderer);

					} else if (key == "ENTITIES" && value == "END") {
						break;
					}
				}
			} else if (key == "SPRITES" && value == "BEGIN") {
				for (int i = 0; i < 20; i++) {
					std::getline(map, line);
					std::istringstream iss(line);

					if (!(iss >> key >> value)) {
						continue;
					}

					if (key == "STAR") {
						addSprite(value, renderer, SpriteType::STAR);
					} else if (key == "BOW") {
						addSprite(value, renderer, SpriteType::BOW);
					} else if (key == "SPRITES" && value == "END") {
						break;
					}
				}
			} else if (key == "COLOR") {
				setRGB(value);
			}
		}

		map.close();

	} else {
		std::cout << "Error loading map " << path << std::endl;
	}
}

void Room::playMusic()
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
	SDL_Surface *background_surface = IMG_Load(path.c_str());
	background_texture = SDL_CreateTextureFromSurface(renderer, background_surface);
	SDL_FreeSurface(background_surface);
}

void Room::setRGB(std::string value)
{
	std::stringstream ss(value);
	std::vector<std::string> result;

	while (ss.good()) {
		std::string substr;
		getline(ss, substr, ',');
		result.push_back(substr);
	}

	red = std::stoi(result.at(0));
	green = std::stoi(result.at(1));
	blue = std::stoi(result.at(2));
}

void Room::addEnemy(std::string value, SDL_Renderer *renderer)
{
	auto enemy = new Enemy();

	std::stringstream ss(value);
	std::vector<std::string> result;

	while (ss.good()) {
		std::string substr;
		getline(ss, substr, ',');
		result.push_back(substr);
	}

	enemy->setPosX(std::stoi(result.at(1)));
	enemy->setPosY(SCREEN_HEIGHT + std::stoi(result.at(2)) - enemy->getHeight() - 350);
	enemy->loadFromFile(result.at(0), renderer);

	enemies.emplace_back(enemy);
}

void Room::addBoss(std::string value, SDL_Renderer *renderer)
{
	auto boss = new Boss();

	std::stringstream ss(value);
	std::vector<std::string> result;

	while (ss.good()) {
		std::string substr;
		getline(ss, substr, ',');
		result.push_back(substr);
	}

	boss->setPosX(std::stoi(result.at(1)));
	boss->setPosY(SCREEN_HEIGHT + std::stoi(result.at(2)) - boss->getHeight() - 350);
	boss->loadFromFile(result.at(0), renderer);

	enemies.emplace_back(boss);
}

void Room::addSprite(std::string value, SDL_Renderer *renderer, SpriteType type)
{

	std::stringstream ss(value);
	std::vector<std::string> result;

	while (ss.good()) {
		std::string substr;
		getline(ss, substr, ',');
		result.push_back(substr);
	}

	int x = std::stoi(result.at(0));
	int y = std::stoi(result.at(1));

	switch (type) {
	case SpriteType::STAR:
		sprites.emplace_back(new Sprite(x, y, starTexture, renderer, type));
		break;
	case SpriteType::BOW:
		sprites.emplace_back(new Sprite(x, y, bowTexture, renderer, type));
		break;
	}
}

void Room::addTile(std::string value, int type)
{

	std::stringstream ss(value);
	std::vector<std::string> result;

	while (ss.good()) {
		std::string substr;
		getline(ss, substr, ',');
		result.push_back(substr);
	}

	int x = std::stoi(result.at(0));
	int y = std::stoi(result.at(1));
	int w = std::stoi(result.at(2)) / Tile::TILE_WEIGHT;

	if (type == Tile::TILE_PLATFORM || type == Tile::TILE_GROUND) {
		for (int i = 0; i < w; i++) {
			tiles.emplace_back(Tile(x + i * Tile::TILE_WEIGHT, SCREEN_HEIGHT - y, type));
		}
	} else if (type == Tile::TILE_WALL) {
		for (int i = 0; i < w; i++) {
			tiles.emplace_back(Tile(x, SCREEN_HEIGHT - (y + i * Tile::TILE_WEIGHT), type));
		}
	}
}

void Room::removeTile(std::string value)
{
	std::stringstream ss(value);
	std::vector<std::string> result;

	while (ss.good()) {
		std::string substr;
		getline(ss, substr, ',');
		result.push_back(substr);
	}

	int x_start = std::stoi(result.at(0));
	int x_end = std::stoi(result.at(1));

	if (x_end <= x_start) {
		return;
	}

	std::vector<Tile>::iterator iter;
	for (iter = tiles.begin(); iter != tiles.end();) {
		if (iter->getY() >= (SCREEN_HEIGHT - Tile::TILE_HEIGHT * 2)) {
			if (iter->getX() >= x_start && iter->getX() <= x_end) {
				iter = tiles.erase(iter);
			} else {
				iter++;
			}
		} else {
			iter++;
		}
	}
}

void Room::removeSkyTile(std::string value)
{
	std::stringstream ss(value);
	std::vector<std::string> result;

	while (ss.good()) {
		std::string substr;
		getline(ss, substr, ',');
		result.push_back(substr);
	}

	int x_start = std::stoi(result.at(0));
	int x_end = std::stoi(result.at(1));

	if (x_end <= x_start) {
		return;
	}

	std::vector<Tile>::iterator iter;
	for (iter = tiles.begin(); iter != tiles.end();) {
		if (iter->getY() == 0) {
			if (iter->getX() >= x_start && iter->getX() <= x_end) {
				iter = tiles.erase(iter);
			} else {
				iter++;
			}
		} else {
			iter++;
		}
	}
}

void Room::moveEnemies(Player *player)
{

	for (auto &enemy : enemies) {
		enemy->move();
		enemy->moveAI(player);
	}
}

// void Room::setEnemyPos(std::string value)
//{
//	std::stringstream ss(value);
//	std::vector<std::string> result;
//
//	while (ss.good()) {
//		std::string substr;
//		getline(ss, substr, ',');
//		result.push_back(substr);
//	}
//
//	enemy.setPosX(std::stoi(result.at(0)));
//	enemy.setPosY(SCREEN_HEIGHT + std::stoi(result.at(1)) - enemy.getHeight() - 350);
//}

void Room::renderSprites(SDL_Renderer *renderer)
{
	for (auto &sprite : sprites) {
		if (sprite->visible)
			sprite->render(renderer);
	}
}

void Room::renderEnemies(SDL_Renderer *renderer)
{
	for (auto &enemy : enemies) {
		enemy->render(renderer);
	}
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

void Room::collisionTilesEnemies()
{
	for (auto &enemy : enemies) {
		collisionTiles(enemy);
	}
}

void Room::collisionTiles(Character *character)
{
	for (auto &tile : tiles) {
		if (character->getPosX() > tile.getX() - 10 && character->getPosX() + 5 < (tile.getX() + Tile::TILE_WEIGHT + 1)) {
			if (character->getPosY() + character->getHeight() > tile.getY() - 3 &&
			    character->getPosY() + character->getHeight() < (tile.getY() + 3)) {
				character->setPosY(tile.getY() - character->getHeight());
				character->setContactPlatform(true);
				return;
			}
		}
	}
	character->setContactPlatform(false);
	character->setContactWall(false);
}

void Room::enter()
{
	playMusic();
	setVisited();
}

void Room::setVisited()
{
	visited = true;
}

bool Room::isVisited()
{
	return visited;
}

int Room::getIndex()
{
	return roomIndex;
}

Room::~Room()
{
	// Free the music
	free();
}

void Room::free()
{

	SDL_DestroyTexture(background_texture);

	for (auto &enemy : enemies) {
		enemy->free();
	}

	for (auto &sprite : sprites) {
		sprite->free();
	}

	for (auto &tile : tiles) {
		tile.free();
	}

	starTexture.free();
	bowTexture.free();
	// Free the music
	// if( music != nullptr ) {
	//	Mix_FreeMusic(music);
	//	music = NULL;
	//}
}

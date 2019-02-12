#include <Room.h>

#include "Sprite.h"
#include "Util.h"
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

Room::Room(int index, int indexLeft, int indexRight, int indexAbove, int indexBelow, int level)
    : background_texture(nullptr), music(nullptr), danger_music(nullptr)
{
	roomIndex = index;
	roomIndexAbove = indexAbove;
	roomIndexBelow = indexBelow;
	roomIndexLeft = indexLeft;
	roomIndexRight = indexRight;
	this->level = level;

	initTiles();
}

void Room::initTiles()
{
	for (int i = 0; i < SCREEN_HEIGHT / Tile::TILE_HEIGHT; i++) {
		// Left Wall
		tiles.emplace_back(Tile(0, i * Tile::TILE_HEIGHT, Tile::TILE_WALL, level));
		// Right Wall
		tiles.emplace_back(Tile(SCREEN_WIDTH - Tile::TILE_WIDTH, i * Tile::TILE_HEIGHT, Tile::TILE_WALL, level));
	}

	for (int i = 0; i < SCREEN_WIDTH / Tile::TILE_WIDTH; i++) {
		// GROUND
		tiles.emplace_back(Tile(i * Tile::TILE_WIDTH, SCREEN_HEIGHT - Tile::TILE_HEIGHT, Tile::TILE_GROUND, level));
		tiles.emplace_back(
		    Tile(i * Tile::TILE_WIDTH, SCREEN_HEIGHT - Tile::TILE_HEIGHT * 2, Tile::TILE_PLATFORM, level));

		// SKY
		tiles.emplace_back(Tile(i * Tile::TILE_WIDTH, 0, Tile::TILE_GROUND, level));
	}
}

void Room::loadFromFile(std::string path, SDL_Renderer *renderer)
{

	starTexture.loadFromFile("assets/images/sprites/star.png", renderer);
    heartTexture.loadFromFile("assets/images/sprites/heart.png", renderer);
    bowTexture.loadFromFile("assets/images/sprites/bow.png", renderer);
	flagTexture.loadFromFile("assets/images/sprites/flag.png", renderer);
	flyingItemTexture.loadFromFile("assets/images/sprites/chest.png", renderer);

	spriteSound1 = Mix_LoadWAV("assets/music/money-001.wav");
	spriteSound2 = Mix_LoadWAV("assets/music/406244.wav");

	arrows.loadTexture(renderer);
	fireball.loadTexture(renderer);
	std::ifstream map(path);

	if (map.is_open()) {

		std::string line;

		while (std::getline(map, line)) {
			std::istringstream iss(line);
			std::string key, value;
			if (!(iss >> key >> value)) {
				continue;
			}

			if (key == "#") {
				continue;
			} else if (key == "MUSIC") {
				loadMusic(value, RoomSoundType::NORMAL);
			} else if (key == "DANGER_MUSIC") {
				loadMusic(value, RoomSoundType::DANGER);
			} else if (key == "BACKGROUND") {
				loadBackground(value, renderer);
			} else if (key == "PLATFORM") {
				addTile(value, Tile::TILE_PLATFORM);
			} else if (key == "WALL") {
				addTile(value, Tile::TILE_WALL);
			} else if (key == "HOLE") {
				removeTile(value);
			} else if (key == "SKYHOLE") {
				removeSkyTile(value);
			} else if (key == "DOORRIGHT") {
				addDoorRight(value);
			} else if (key == "DOORLEFT") {
				addDoorLeft(value);
			} else if (key == "ENEMY") {
				addEnemy(value, renderer);
			} else if (key == "BOSS") {
				addBoss(value, renderer);
			} else if (key == "STAR") {
				addSprite(value, renderer, SpriteType::STAR);
			} else if (key == "HEART") {
				addSprite(value, renderer, SpriteType::HEART);
			} else if (key == "BOW") {
				addSprite(value, renderer, SpriteType::BOW);
			} else if (key == "COLOR") {
				setRGB(value);
			} else if (key == "SAVE_POINT") {
				hasSavePoint = true;
				addSprite(value, renderer, SpriteType::FLAG);
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
	std::vector<std::string> result = util::getValues(value);

	mapColor.red = std::stoi(result.at(0));
	mapColor.green = std::stoi(result.at(1));
	mapColor.blue = std::stoi(result.at(2));
}

void Room::addEnemy(std::string value, SDL_Renderer *renderer)
{
	auto enemy = new Enemy();

	std::vector<std::string> result = util::getValues(value);

	enemy->setPosX(std::stoi(result.at(1)));
	enemy->setPosY(SCREEN_HEIGHT + std::stoi(result.at(2)) - enemy->getHeight() - 350);
	if (enemy->loadFromFile(result.at(0), renderer)) {
		std::shared_ptr<std::vector<Projectile_vert *>> projectiles(new std::vector<Projectile_vert *>());
		//        for(int i = 0; i < 3; i++){
		Projectile_vert *project = new Projectile_vert("assets/profiles/skull.txt", renderer);
		projectiles->emplace_back(project);
		enemies.emplace_back(project);
		//        }

		enemy->setProjectiles(projectiles);
	}

	enemies.emplace_back(enemy);
}

void Room::addBoss(std::string value, SDL_Renderer *renderer)
{
	auto boss = new Boss();

	auto *item = new Sprite(50, 50, flyingItemTexture, renderer, SpriteType::SPECIAL, spriteSound2, roomIndex);
	item->visible = false;
	sprites.emplace_back(item);

	boss->setItem(item);

	std::shared_ptr<std::vector<Projectile *>> projectiles(new std::vector<Projectile *>());
	for (int i = 0; i < 3; i++) {
		Projectile *project = new Projectile("assets/profiles/fireball.txt", renderer);
		projectiles->emplace_back(project);
		enemies.emplace_back(project);
	}

	boss->setProjectiles(projectiles);

	std::vector<std::string> result = util::getValues(value);
	boss->setPosX(std::stoi(result.at(1)));
	boss->setPosY(SCREEN_HEIGHT + std::stoi(result.at(2)) - boss->getHeight() - 350);
	boss->loadFromFile(result.at(0), renderer);

	enemies.emplace_back(boss);
}

void Room::addSprite(std::string value, SDL_Renderer *renderer, SpriteType type)
{

	std::vector<std::string> result = util::getValues(value);

	int x = std::stoi(result.at(0));
	int y = std::stoi(result.at(1));

	switch (type) {
	case SpriteType::STAR:
		sprites.emplace_back(new Sprite(x, y, starTexture, renderer, type, spriteSound1, roomIndex));
		break;
	case SpriteType::HEART:
		sprites.emplace_back(new Sprite(x, y, heartTexture, renderer, type, spriteSound1, roomIndex));
		break;
	case SpriteType::BOW:
		sprites.emplace_back(new Sprite(x, y, bowTexture, renderer, type, spriteSound2, roomIndex));
		break;
	case SpriteType::FLAG:
		sprites.emplace_back(new Sprite(x, y, flagTexture, renderer, type, spriteSound2, roomIndex));
		break;
	}
}

void Room::addTile(std::string value, int type)
{

	std::vector<std::string> result = util::getValues(value);

	int x = std::stoi(result.at(0));
	int y = std::stoi(result.at(1));
	int w = std::stoi(result.at(2)) / Tile::TILE_WIDTH;

	if (type == Tile::TILE_PLATFORM || type == Tile::TILE_GROUND) {
		for (int i = 0; i < w; i++) {
			tiles.emplace_back(Tile(x + i * Tile::TILE_WIDTH, SCREEN_HEIGHT - y, type, level));
		}
	} else if (type == Tile::TILE_WALL) {
		for (int i = 0; i < w; i++) {
			tiles.emplace_back(Tile(x, SCREEN_HEIGHT - (y + i * Tile::TILE_WIDTH), type, level));
		}
	}
}

void Room::removeTile(std::string value)
{
	std::vector<std::string> result = util::getValues(value);

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
	std::vector<std::string> result = util::getValues(value);

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

void Room::addDoorRight(std::string value)
{
	std::vector<std::string> result = util::getValues(value);

	int y_start = std::stoi(result.at(0));
	int y_end = std::stoi(result.at(1));

	if (y_end <= y_start) {
		return;
	}

	std::vector<Tile>::iterator iter;
	for (iter = tiles.begin(); iter != tiles.end();) {
		if (iter->getX() >= (SCREEN_WIDTH - Tile::TILE_WIDTH)) {
			if (iter->getY() >= y_start && iter->getY() <= y_end) {
				if (iter->getTileType() == Tile::TILE_WALL)
					iter = tiles.erase(iter);
				else
					iter++;
			} else {
				iter++;
			}
		} else {
			iter++;
		}
	}
}

void Room::addDoorLeft(std::string value)
{
	std::vector<std::string> result = util::getValues(value);

	int y_start = std::stoi(result.at(0));
	int y_end = std::stoi(result.at(1));

	if (y_end <= y_start) {
		return;
	}

	std::vector<Tile>::iterator iter;
	for (iter = tiles.begin(); iter != tiles.end();) {
		if (iter->getX() == 0) {
			if (iter->getY() >= y_start && iter->getY() <= y_end) {
				if (iter->getTileType() == Tile::TILE_WALL)
					iter = tiles.erase(iter);
				else
					iter++;
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
		collisionWall(enemy);
	}
}

void Room::collisionTiles(Character *character)
{
	for (auto &tile : tiles) {
		if (tile.getTileType() == Tile::TILE_WALL)
			continue;
		if (character->getPosX() > tile.getX() - 10 &&
		    character->getPosX() + 5 < (tile.getX() + Tile::TILE_WIDTH + 1)) {
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

void Room::collisionWall(Character *character)
{
	auto characterPosY = character->getPosY() + character->getHeight();
	auto characterPosXRight = character->getPosX() + character->getWidth();

	for (auto &tile : tiles) {
		if (tile.getTileType() != Tile::TILE_WALL)
			continue;

		if (characterPosY > tile.getY() - 2 && characterPosY < (tile.getY() + Tile::TILE_HEIGHT + 2)) {
			if (character->getPosX() < tile.getX() + Tile::TILE_WIDTH && characterPosXRight > tile.getX()) {
				if (character->getFlipType() == SDL_FLIP_HORIZONTAL) {
					character->setPosX(tile.getX() + Tile::TILE_WIDTH + 1);
				} else {
					character->setPosX(tile.getX() - Tile::TILE_WIDTH - 1);
				}
				return;
			}
		}
	}
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
    heartTexture.free();
    bowTexture.free();
	flagTexture.free();
	flyingItemTexture.free();

    Mix_FreeChunk(spriteSound1);
	spriteSound1 = NULL;

	Mix_FreeChunk(spriteSound2);
	spriteSound2 = NULL;
	// Free the music
	// if( music != nullptr ) {
	//	Mix_FreeMusic(music);
	//	music = NULL;
	//}
}

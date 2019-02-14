#ifndef ROOM_H
#define ROOM_H

#include "Boss.h"
#include "Enemy.h"
#include "Projectile.h"
#include "IEnemy.h"
#include "Player.h"
#include "Sprite.h"
#include "Tile.h"
#include <Arrows.h>
#include <Fireball.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>

struct MapColor {
	int red = 0;
	int green = 0;
	int blue = 0;
};

enum class RoomSoundType { NORMAL, DANGER };

class Room {

  public:
	// Initializes variables
	Room();

	// Initializes variables
	Room(int index, int roomIndexLeft, int roomIndexRight, int roomIndexAbove, int roomIndexBelow, int level);

	// Deallocates memory
	~Room();

	void free();

	void loadFromFile(std::string path, SDL_Renderer *renderer);

	void renderTiles(SDL_Renderer *renderer, SDL_Texture *texture);

	void renderSprites(SDL_Renderer *renderer);

	void renderEnemies(SDL_Renderer *renderer);

	void collisionTiles(Character *character);

    void collisionWall(Character *character);

    void collisionTilesEnemies();

	void moveEnemies(Player *player);

	void resetRoom();

	SDL_Texture *background_texture;

	std::vector<IEnemy *> enemies;
	std::vector<Sprite *> sprites;
	std::vector<Projectile *> projectiles;

	Arrows arrows;
	Fireball fireball;
	// std::vector<LTexture *> backgroundTextures;

	int roomIndexLeft;
	int roomIndexRight;
	int roomIndexAbove;
	int roomIndexBelow;

	int level;

	MapColor mapColor;

	bool renderedInMap = false;
	bool hasSavePoint = false;

	void enter();
	void playMusic();
	void setVisited();
	bool isVisited();
	int getIndex();

  private:
	void loadMusic(std::string path, RoomSoundType sound_type);
	void loadBackground(std::string path, SDL_Renderer *renderer);
	void setRGB(std::string value);
	void setSavePoint(std::string value);
	void addEnemy(std::string path, SDL_Renderer *renderer);
	void addFirstBoss(std::string path, SDL_Renderer *renderer);
    void addSecondBoss(std::string path, SDL_Renderer *renderer);
    Projectile* addProjectile(std::string path, SDL_Renderer *renderer);
	void addSprite(std::string path, SDL_Renderer *renderer, SpriteType type);
	void addTile(std::string value, int type);
	void setEnemyPos(std::string value);
	bool checkIfEnemiesInRoom();
	void initTiles();
	void removeTile(std::string value);
	void removeSkyTile(std::string value);
    void addDoorRight(std::string value);
    void addDoorLeft(std::string value);

	std::vector<std::string> getValues(std::string value);

	std::vector<Tile> tiles;

	Sprite flag;

	Texture starTexture;
    Texture heartTexture;
    Texture bowTexture;
    Texture silverBowTexture;
    Texture goldenBowTexture;
    Texture flagTexture;
    Texture flameTexture;
    Texture flyingItemTexture;
    Texture fireItemTexture;

	int roomIndex;
	bool visited = false;
	// The music that will be played
	Mix_Music *music;
	Mix_Music *danger_music;

	Mix_Chunk *spriteSound1;
	Mix_Chunk *spriteSound2;
};

#endif

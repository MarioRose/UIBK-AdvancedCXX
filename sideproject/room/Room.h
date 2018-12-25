#ifndef ROOM_H
#define ROOM_H

#include "Enemy.h"
#include "Tile.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include "../player/Player.h"

enum class RoomSoundType { NORMAL, DANGER };

class Room {

  public:
	// Initializes variables
	Room();

	// Deallocates memory
	~Room();

	void free();

	void loadFromFile(std::string path, SDL_Renderer *renderer);

	void renderTiles(SDL_Renderer *renderer, SDL_Texture *texture);

    void renderEnemies(SDL_Renderer *renderer);

    void collisionTiles(Moveable* character);

    void collisionTilesEnemies();

    void moveEnemies(Player* player);

    // The music that will be played
	Mix_Music *music;
	Mix_Music *danger_music;

	SDL_Surface *background_surface;
	SDL_Texture *background_texture;

	std::vector<Enemy*> enemies;
	// std::vector<LTexture *> backgroundTextures;

  private:
	void playMusic(Mix_Music *music);
	void loadMusic(std::string path, RoomSoundType sound_type);
	void loadBackground(std::string path, SDL_Renderer *renderer);
	void addEnemy(std::string path, SDL_Renderer *renderer);
	void setEnemyPos(std::string value);
	bool checkIfEnemiesInRoom();
	std::vector<Tile> tiles;
};

#endif

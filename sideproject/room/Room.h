#ifndef ROOM_H
#define ROOM_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include "Enemy.h"

enum class RoomSoundType {
	NORMAL,
    DANGER
};

class Room {

public:

	//Initializes variables
	Room();

	//Deallocates memory
	~Room();

	void free();

	void loadFromFile(std::string path, SDL_Renderer* renderer);

	//The music that will be played
	Mix_Music *music;
	Mix_Music *danger_music;

	SDL_Surface *background_surface;
	SDL_Texture *background_texture;

	SDL_Renderer *renderer;

	Enemy enemy;
    void renderEnemies(SDL_Renderer *renderer);
	//std::vector<LTexture *> backgroundTextures;
private:
	void playMusic(Mix_Music *music);
    void loadMusic(std::string path, RoomSoundType sound_type);
	void loadBackground(std::string path, SDL_Renderer *renderer);
	void addEnemy(std::string path, SDL_Renderer *renderer);
    void setEnemyPos(std::string value);
    bool checkIfEnemiesInRoom();
};

#endif

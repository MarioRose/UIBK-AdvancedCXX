#ifndef ROOM_H
#define ROOM_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include "../enemy/Enemy.h"

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

	SDL_Surface *background_surface;
	SDL_Texture *background_texture;

	SDL_Renderer *renderer;

	Enemy enemy;
	//std::vector<LTexture *> backgroundTextures;
private:
		void playMusic(std::string path);
		void loadBackground(std::string path, SDL_Renderer *renderer);
		void addEnemy(std::string path, SDL_Renderer *renderer);

};

#endif

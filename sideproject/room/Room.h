#ifndef ROOM_H
#define ROOM_H


#include <SDL.h>
#include <SDL_mixer.h>
#include <string>

class Room {

public:

	//Initializes variables
	Room();

	//Deallocates memory
	~Room();

	void free();

	void playMusic();

	//Loads image at specified path
	bool loadTeture( std::string path, SDL_Renderer* renderer );

	//The music that will be played
	Mix_Music *music;

	SDL_Surface *background_surface;
	SDL_Texture *background_texture;

	SDL_Renderer *renderer;
	//std::vector<LTexture *> backgroundTextures;

};

#endif

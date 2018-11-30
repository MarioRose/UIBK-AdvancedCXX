#include <Room.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <Settings.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

Room::Room() {
	SDL_Surface *background_surface = NULL;
	SDL_Texture *background_texture = NULL;
	SDL_Renderer *renderer;
	Mix_Music *music = NULL;

}

void Room::loadFromFile(std::string path, SDL_Renderer *renderer) {

	std::ifstream map(path);

	if(map.is_open()){

		std::string line;

		while (std::getline(map, line))
		{
		    std::istringstream iss(line);
		    std::string key, value;
		    if (!(iss >> key >> value)) { continue; } // error
		    //std::cout << key << ": " << value << std::endl;

				if(key == "MUSIC")
				{
						playMusic(value);
				}
				else if(key == "BACKGROUND")
				{
						loadBackground(value, renderer);
				}
		}

		map.close();
	}
	else {
		std::cout << "Error loading map " << path << std::endl;
	}

}

void Room::playMusic(std::string path) {
	//Load music
	music = Mix_LoadMUS( path.c_str() );

	if( music == NULL ) {
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	else {
		Mix_VolumeMusic(MIX_MAX_VOLUME);
		Mix_PlayMusic( music, -1 );
	}
}

void Room::loadBackground(std::string path, SDL_Renderer *renderer) {
	background_surface = IMG_Load(path.c_str());
	background_texture = SDL_CreateTextureFromSurface(renderer, background_surface);
}

Room::~Room(){
	//Free the music
	free();
}

void Room::free() {
	//Free the music
	if( music != NULL ) {
		Mix_FreeMusic(music);
		music = NULL;
	}
}

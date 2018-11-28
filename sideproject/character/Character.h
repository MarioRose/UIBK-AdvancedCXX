#ifndef CHARACTER_H
#define CHARACTER_H

#include "../moveable/Moveable.h"
#include <CharacterStatus.h>
#include <SDL.h>
#include <Texture.h>
#include <vector>
//The character that will move around on the screen

class Character: public Moveable {
public:

	//Initializes the variables
	Character();

	~Character();

	//Takes key presses and adjusts the Character's velocity
	void control( SDL_Event& e );

	//Lets the Character jump
	void jump();

	void free();

	//Shows the Character on the screen
	void render(int spriteNumber, SDL_Renderer* gRenderer);

	CharacterStatus getStatus();

	bool loadTexture(std::string path, SDL_Renderer* renderer);
	bool loadIdleTextures(std::vector<std::string> paths, SDL_Renderer* renderer);
	bool loadIdleTextures(SDL_Renderer* renderer);
	bool loadRunningTextures(std::vector<std::string> paths, SDL_Renderer* renderer);
	bool loadRunningTextures(SDL_Renderer* renderer);

	//Scene textures
	Texture cTexture;

	std::vector<Texture *> idleTextures;
	std::vector<Texture *> runningTextures;

private:
	//The force of the Character
	//double forceY;

	//Status (e.g. idle or running) the Character
	CharacterStatus status;
};

#endif

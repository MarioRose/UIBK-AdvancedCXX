#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>
#include <vector>
#include "Texture.h"
#include "Moveable.h"
//The character that will move around on the screen

enum class CharacterStatus {
	IDLE,
	RUNNING
};


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

	void loadFromFile(std::string path, SDL_Renderer *renderer);


	//Scene textures
	std::vector<Texture *> idleTextures;
	std::vector<Texture *> runningTextures;

protected:
	//The force of the Character
	//double forceY;

	int keypressCount = 0;
	//Status (e.g. idle or running) the Character
	CharacterStatus status;

	bool loadIdleTextures(std::vector<std::string> paths, SDL_Renderer* renderer);
	bool loadRunningTextures(std::vector<std::string> paths, SDL_Renderer* renderer);
};

#endif

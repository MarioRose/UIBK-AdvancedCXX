#ifndef CHARACTER_H
#define CHARACTER_H


#include <SDL.h>
#include <texture.h>
#include <vector>
//The character that will move around on the screen





class Character {
public:
	//The dimensions of the Character
	static const int CHAR_WIDTH = 20;
	static const int CHAR_HEIGHT = 32;

	//Maximum axis velocity of the Character
	static const int CHAR_VEL = 5;

	//Initializes the variables
	Character();

	~Character();

	//Takes key presses and adjusts the Character's velocity
	void handleEvent( SDL_Event& e );

	//Moves the Character
	void move();

	//Lets the Character jump
	void jump();

	//Shows the Character on the screen
	void render(SDL_Renderer* gRenderer);

	//Shows the Character on the screen
	void render(int spriteNumber, SDL_Renderer* gRenderer);

	int getStatus();

	bool loadTexture(std::string path, SDL_Renderer* renderer);
	bool loadIdleTextures(std::vector<std::string> paths, SDL_Renderer* renderer);
	bool loadIdleTextures(SDL_Renderer* renderer);
	bool loadRunningTextures(std::vector<std::string> paths, SDL_Renderer* renderer);
	bool loadRunningTextures(SDL_Renderer* renderer);

	void free();

	//Scene textures
	LTexture cTexture;

	std::vector<LTexture *> idleTextures;
	std::vector<LTexture *> runningTextures;

private:
	//The X and Y offsets of the Character
	double mPosX, mPosY;

	//The velocity of the Character
	double mVelX, mVelY;

	//The force of the Character
	double mForceY;

	/*TODO: add enum of static*/
	//Status (e.g. idle or running) the Character
	int status;

};

#endif

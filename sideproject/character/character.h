#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>
//The character that will move around on the screen
class Character
{
public:
	//The dimensions of the Character
	static const int CHAR_WIDTH = 20;
	static const int CHAR_HEIGHT = 32;

	//Maximum axis velocity of the Character
	static const int CHAR_VEL = 5;

    //Initializes the variables
    Character();

	//Takes key presses and adjusts the Character's velocity
	void handleEvent( SDL_Event& e );

	//Moves the Character
	void move();

	//Lets the Character jump
	void jump();

	//Shows the Character on the screen
	void render();

    //Shows the Character on the screen
    void render(int spriteNumber);

    int getStatus();


private:
	//The X and Y offsets of the Character
	double mPosX, mPosY;

	//The velocity of the Character
	double mVelX, mVelY;

	//The force of the Character
	double mForceY;

    /*TODO: add enum of stati*/
    //Status (e.g. idle or running) the Character
    int status;
};

#endif

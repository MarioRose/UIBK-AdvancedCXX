#ifndef CHARACTER_H
#define CHARACTER_H

#include "GameObject.h"
#include "Texture.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
// The character that will move around on the screen

enum class CharacterStatus { IDLE, RUNNING, DEAD, DYING, ATTACK, FALLING, JUMPING };

enum class CharacterSoundType { SHOUT };

class Character : public GameObject {
  public:
	// Initializes the variables
	Character();
	Character(double x, double y);

	~Character();

	// Takes key presses and adjusts the Character's velocity
	void control(SDL_Event &e);

	// Lets the Character jump
	void jump();
	void move();

	bool contactsPlatform();
	void setContactPlatform(bool);

	bool contactsWall();
	void setContactWall(bool b);

	void changeDirection();

	void free();

	// Shows the Character on the screen
	void render(SDL_Renderer *gRenderer);

	SDL_RendererFlip getFlipType() const;

	CharacterStatus getStatus();

	void loadFromFile(std::string path, SDL_Renderer *renderer);

	Mix_Chunk *shout_sound;

	// Scene textures
	std::vector<Texture *> idleTextures;
	std::vector<Texture *> runningTextures;
	std::vector<Texture *> dyingTextures;
	std::vector<Texture *> attackTextures;
	std::vector<Texture *> fallingTextures;
	std::vector<Texture *> jumpingTextures;

	void nextSpriteIndex();
	void shout();

	void setHealth(int health);
	void loseHealth();
	void setDeath();
    bool isAlive();
	int getHealth();

  protected:
	// The velocity of the moveable Object
	double velX;
	// The velocity of the moveable Object
	double velY;
	// Max velocity of moveable Object
	const double max_vel = 5;
	// The velocity of the moveable Object
	double forceY;

	// Flip type (for flipping objects when facing left)
	SDL_RendererFlip flipType;
	SDL_Rect *clip = NULL;

	bool contactPlatform = false;
	bool contactWall = false;

	int keypressCount = 0;
	int spriteIndexIdle = 0;
	int spriteIndexRunning = 0;
	int spriteIndexDying = 0;
	int spriteIndexAttack = 0;
	int spriteIndexFalling = 0;
	int spriteIndexJumping = 0;

	// Status (e.g. idle or running) the Character
	CharacterStatus status;
	int health;

	bool loadTextures(std::vector<std::string> paths, TextureType texture_type, SDL_Renderer *renderer);
	void addSound(std::string path, CharacterSoundType sound_type);
};

#endif

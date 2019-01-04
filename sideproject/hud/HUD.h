#ifndef HUD_H
#define HUD_H

#include <Moveable.h>
#include <Player.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <Texture.h>
#include <vector>

class HUD {
  public:
	// Initializes the variables
	HUD(SDL_Renderer *renderer);

	~HUD();

	void free();

	// Shows the Character on the screen
	void render(SDL_Renderer *gRenderer, Player *player);

	SDL_Renderer *renderer;
	// Scene textures
	std::vector<Texture *> liveCountTextures;
	Texture starTexture;
    SDL_Surface* pointsSurface;
    SDL_Texture* pointsTexture;

  protected:
	// The force of the Character
	// double forceY;
	bool loadTextures();
};

#endif

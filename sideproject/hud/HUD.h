#ifndef HUD_H
#define HUD_H

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
	void render(Player *player, bool updateHud);

    void updatePoints(Player *player);
    void updateLiveCount(Player *player);

	SDL_Renderer *renderer;
	// Scene textures
	Texture hearthTexture;
	Texture starTexture;
	Texture itemHolder;
    Texture bow;
    Texture silverBow;
    Texture goldenBow;
    Texture doubleJump;

  protected:
	// The force of the Character
	// double forceY;
	bool loadTextures();

  private:
     SDL_Texture* pointsTexture = NULL;
     SDL_Texture* pointsTextureBlack = NULL;

     SDL_Texture* liveCountTexture = NULL;
     SDL_Texture* liveCountTextureBlack = NULL;

    bool firstRender = false;

    void renderEquippedItem(Player* player);
    void renderEquippedAbility(Player* player);

};

#endif

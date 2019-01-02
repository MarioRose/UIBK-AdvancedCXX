#include <HUD.h>
#include <Player.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <Settings.h>
#include <exception>
#include <sstream>
#include <string>
#include <vector>

HUD::HUD(SDL_Renderer *renderer) : renderer(renderer)
{
	loadTextures();
}

HUD::~HUD()
{
	free();
}

bool HUD::loadTextures()
{
	// Loading success flag
	bool success = true;

	// TODO doesnt work with live count higher than 5
	for (size_t i = 0; i < 6; i++) {
		auto liveTexture = new Texture;
		std::stringstream ss;
		ss << "../../assets/images/heart-" << i << ".png";
		liveTexture->loadFromFile(ss.str(), renderer);
		liveTexture->scaleToHeight(SCREEN_HEIGHT * 0.05);
		liveCountTextures.push_back(liveTexture);
	}

	return success;
}

void HUD::render(SDL_Renderer *renderer, Player *player)
{
	int index = std::max(0, player->getLifeCount());
	liveCountTextures.at(index)->render(10, 10, renderer, NULL, 0, NULL, SDL_FLIP_NONE);
}

void HUD::free()
{
	for (auto t : liveCountTextures) {
		t->free();
	}
}

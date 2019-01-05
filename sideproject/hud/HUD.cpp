#include <HUD.h>
#include <Player.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
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
    starTexture.loadFromFile("../../assets/images/sprites/star.png", renderer);
    starTexture.scaleToHeight(SCREEN_HEIGHT * 0.05);

	return success;
}

void HUD::render(SDL_Renderer *renderer, Player *player)
{
	int index = std::max(0, player->getLifeCount());
	liveCountTextures.at(index)->render(10, 10, renderer, NULL, 0, NULL, SDL_FLIP_NONE);

    starTexture.render(70, 10, renderer, NULL, 0, NULL, SDL_FLIP_NONE);

    // TODO nicht ideal dass immer wieder neue Texturen erstellt werden!
    TTF_Font *font = TTF_OpenFont("../../assets/fonts/OpenSans-Bold.ttf", 18);

    SDL_Surface* pointsSurface = TTF_RenderText_Solid(font, std::to_string(player->getPoints()).c_str(), {255, 255, 255, 100});
    SDL_Surface* pointsSurfaceBlack = TTF_RenderText_Solid(font, std::to_string(player->getPoints()).c_str(), {0, 0, 0, 100});

    pointsTexture = SDL_CreateTextureFromSurface(renderer, pointsSurface);
    pointsTextureBlack = SDL_CreateTextureFromSurface(renderer, pointsSurfaceBlack);
    SDL_FreeSurface(pointsSurface);
    SDL_FreeSurface(pointsSurfaceBlack);

    SDL_Rect renderQuadBlack = {104, 8, 12, 30};
    SDL_RenderCopy(renderer, pointsTextureBlack, NULL, &renderQuadBlack);
    SDL_Rect renderQuad = {102, 6, 12, 30};
    SDL_RenderCopy(renderer, pointsTexture, NULL, &renderQuad);

}

void HUD::free()
{
	for (auto t : liveCountTextures) {
		t->free();
	}
    SDL_DestroyTexture(pointsTexture);
    SDL_DestroyTexture(pointsTextureBlack);
}

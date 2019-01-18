#include "HUD.h"
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
		Texture *liveTexture = new Texture;
		std::stringstream ss;
		ss << "assets/images/heart-" << i << ".png";
		liveTexture->loadFromFile(ss.str(), renderer);
		liveTexture->scaleToHeight(SCREEN_HEIGHT * 0.05);
		liveCountTextures.push_back(liveTexture);
	}
	starTexture.loadFromFile("assets/images/sprites/star.png", renderer);
	starTexture.scaleToHeight(SCREEN_HEIGHT * 0.05);

	itemHolder.loadFromFile("assets/images/item_holder.png", renderer);
	itemHolder.scaleToHeight(SCREEN_HEIGHT * 0.1);

	bow.loadFromFile("assets/images/sprites/bow.png", renderer);
	bow.scaleToHeight(SCREEN_HEIGHT * 0.1);

	updatePoints(NULL);

	return success;
}

void HUD::render(Player *player, bool updateScore)
{
	int index = std::max(0, player->getHealth());
	liveCountTextures.at(index)->render(10, 10, renderer, NULL, 0, NULL, SDL_FLIP_NONE);

	starTexture.render(70, 10, renderer, NULL, 0, NULL, SDL_FLIP_NONE);

	itemHolder.render(SCREEN_WIDTH - 60, 10, renderer, NULL, 0, NULL, SDL_FLIP_NONE);

	renderEquippedItem(player);

	if (updateScore) {
		updatePoints(player);
	}

	SDL_Rect renderQuadBlack = {104, 8, 12, 30};
	SDL_RenderCopy(renderer, pointsTextureBlack, NULL, &renderQuadBlack);
	SDL_Rect renderQuad = {102, 6, 12, 30};
	SDL_RenderCopy(renderer, pointsTexture, NULL, &renderQuad);
}

void HUD::updatePoints(Player *player)
{
	TTF_Init();
	TTF_Font *font = TTF_OpenFont("./assets/fonts/OpenSans-Bold.ttf", 18);

	SDL_Surface *pointsSurface;
	SDL_Surface *pointsSurfaceBlack;

	if (player != NULL) {
		pointsSurface = TTF_RenderText_Solid(font, std::to_string(player->getPoints()).c_str(), {255, 255, 255, 100});
		pointsSurfaceBlack = TTF_RenderText_Solid(font, std::to_string(player->getPoints()).c_str(), {0, 0, 0, 100});
	} else {
		pointsSurface = TTF_RenderText_Solid(font, "0", {255, 255, 255, 100});
		pointsSurfaceBlack = TTF_RenderText_Solid(font, "0", {0, 0, 0, 100});
	}

	if (pointsTexture != NULL) {
		SDL_DestroyTexture(pointsTexture);
	}

	pointsTexture = SDL_CreateTextureFromSurface(renderer, pointsSurface);
	if (pointsTexture == NULL) {
		fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
		exit(1);
	}

	if (pointsTextureBlack != NULL) {
		SDL_DestroyTexture(pointsTextureBlack);
	}

	pointsTextureBlack = SDL_CreateTextureFromSurface(renderer, pointsSurfaceBlack);
	if (pointsTextureBlack == NULL) {
		fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_FreeSurface(pointsSurface);
	SDL_FreeSurface(pointsSurfaceBlack);
}

void HUD::free()
{
	for (auto &t : liveCountTextures) {
		t->free();
	}
	starTexture.free();
	SDL_DestroyTexture(pointsTexture);
	SDL_DestroyTexture(pointsTextureBlack);
}

void HUD::renderEquippedItem(Player *player)
{
	switch (player->getEquippedItem()) {
	case EquippedItem::BOW:
		bow.render(SCREEN_WIDTH - 50, 10, renderer, NULL, 0, NULL, SDL_FLIP_NONE);
        return;
        default :
            return;
	}
}

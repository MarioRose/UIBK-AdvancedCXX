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

	hearthTexture.loadFromFile("assets/images/heart.png", renderer);
	hearthTexture.scaleToHeight(SCREEN_HEIGHT * 0.05);

	starTexture.loadFromFile("assets/images/sprites/star.png", renderer);
	starTexture.scaleToHeight(SCREEN_HEIGHT * 0.05);

	itemHolder.loadFromFile("assets/images/item_holder.png", renderer);
	itemHolder.scaleToHeight(SCREEN_HEIGHT * 0.1);

	bow.loadFromFile("assets/images/sprites/bow.png", renderer);
	bow.scaleToHeight(SCREEN_HEIGHT * 0.1);

    silverBow.loadFromFile("assets/images/sprites/silverBow.png", renderer);
    silverBow.scaleToHeight(SCREEN_HEIGHT * 0.1);

    goldenBow.loadFromFile("assets/images/sprites/goldenBow.png", renderer);
    goldenBow.scaleToHeight(SCREEN_HEIGHT * 0.1);

    doubleJump.loadFromFile("assets/images/sprites/chest.png", renderer);
	doubleJump.scaleToHeight(SCREEN_HEIGHT * 0.04);

	return success;
}

void HUD::render(Player *player, bool updateHud)
{
	hearthTexture.render(10, 10, renderer, NULL, 0, NULL, SDL_FLIP_NONE);
	starTexture.render(70, 10, renderer, NULL, 0, NULL, SDL_FLIP_NONE);

	itemHolder.render(SCREEN_WIDTH - 90, 10, renderer, NULL, 0, NULL, SDL_FLIP_NONE);
	itemHolder.render(SCREEN_WIDTH - 60, 10, renderer, NULL, 0, NULL, SDL_FLIP_NONE);

	renderEquippedItem(player);
	renderEquippedAbility(player);

	if (updateHud | !firstRender) {
		updatePoints(player);
		updateLiveCount(player);
	}

	int widthPoints = ((int)log10((double)player->getPoints()) + 1) * 12;

	SDL_Rect renderQuadBlack = {104, 8, widthPoints, 30};
	SDL_RenderCopy(renderer, pointsTextureBlack, NULL, &renderQuadBlack);
	SDL_Rect renderQuad = {102, 6, widthPoints, 30};
	SDL_RenderCopy(renderer, pointsTexture, NULL, &renderQuad);

	int widthHealth = ((int)log10((double)player->getHealth()) + 1) * 12;

	renderQuadBlack = {44, 8, widthHealth, 30};
	SDL_RenderCopy(renderer, liveCountTextureBlack, NULL, &renderQuadBlack);
	renderQuad = {42, 6, widthHealth, 30};
	SDL_RenderCopy(renderer, liveCountTexture, NULL, &renderQuad);

	if (!firstRender) {
		firstRender = true;
	}
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

void HUD::updateLiveCount(Player *player)
{
	TTF_Init();
	TTF_Font *font = TTF_OpenFont("./assets/fonts/OpenSans-Bold.ttf", 18);

	SDL_Surface *liveCountSurface;
	SDL_Surface *liveCountSurfaceBlack;

	if (player != NULL) {
		liveCountSurface =
		    TTF_RenderText_Solid(font, std::to_string(player->getHealth()).c_str(), {255, 255, 255, 100});
		liveCountSurfaceBlack = TTF_RenderText_Solid(font, std::to_string(player->getHealth()).c_str(), {0, 0, 0, 100});
	} else {
		liveCountSurface = TTF_RenderText_Solid(font, "5", {255, 255, 255, 100});
		liveCountSurfaceBlack = TTF_RenderText_Solid(font, "5", {0, 0, 0, 100});
	}

	if (liveCountTexture != NULL) {
		SDL_DestroyTexture(liveCountTexture);
	}

	liveCountTexture = SDL_CreateTextureFromSurface(renderer, liveCountSurface);
	if (liveCountTexture == NULL) {
		fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
		exit(1);
	}

	if (liveCountTextureBlack != NULL) {
		SDL_DestroyTexture(liveCountTextureBlack);
	}

	liveCountTextureBlack = SDL_CreateTextureFromSurface(renderer, liveCountSurfaceBlack);
	if (liveCountTextureBlack == NULL) {
		fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_FreeSurface(liveCountSurface);
	SDL_FreeSurface(liveCountSurfaceBlack);
}

void HUD::free()
{
	starTexture.free();
	SDL_DestroyTexture(pointsTexture);
	SDL_DestroyTexture(pointsTextureBlack);

	SDL_DestroyTexture(liveCountTexture);
	SDL_DestroyTexture(liveCountTextureBlack);
}

void HUD::renderEquippedItem(Player *player)
{
	switch (player->getEquippedItem()) {
	case EquippedItem::BOW:
		bow.render(SCREEN_WIDTH - 50, 10, renderer, NULL, 0, NULL, SDL_FLIP_NONE);
		return;
	case EquippedItem::SILVERBOW:
		silverBow.render(SCREEN_WIDTH - 50, 10, renderer, NULL, 0, NULL, SDL_FLIP_NONE);
		return;
	case EquippedItem::GOLDENBOW:
		goldenBow.render(SCREEN_WIDTH - 50, 10, renderer, NULL, 0, NULL, SDL_FLIP_NONE);
		return;
	default:
		return;
	}
}

void HUD::renderEquippedAbility(Player *player)
{
	switch (player->getEquippedAbility()) {
	case EquippedAbility::JUMP:
		doubleJump.render(SCREEN_WIDTH - 80, 20, renderer, NULL, 0, NULL, SDL_FLIP_NONE);
		return;
	default:
		return;
	}
}
#include <Arrows.h>

#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>

Arrows::Arrows() {
}


Arrows::~Arrows() {
	for(auto& arrow : arrows) {
		delete arrow;
	}
	arrowTexture.free();
	bowTexture.free();

}

bool Arrows::loadTexture(SDL_Renderer *renderer)
{
	// Loading success flag
	bool success = true;

	arrowTexture.loadFromFile("assets/images/arrow.png", renderer);
	arrowTexture.scaleToWidth(35);
	bowTexture.loadFromFile("assets/images/sprites/bow.png", renderer);
	bowTexture.scaleToHeight(35);
	return success;
}

void Arrows::shootArrow(int playerX, int playerY, SDL_RendererFlip flipType) {
    if(arrowCount == arrowMax)
        return;

	Arrow* arrow = new Arrow(flipType);
	if(flipType == SDL_FLIP_NONE) {
		arrow->shoot(playerX, playerY+15);
	} else {
		arrow->shoot(playerX-15, playerY+15);
	}
	arrows.push_back(arrow);
	renderBow = true;
	arrowCount++;
}


void Arrows::render(SDL_Renderer *renderer, int playerX, int playerY, SDL_RendererFlip flipType) {
	if(renderBow && framesSinceLastArrowShot < 10) {
		if(flipType == SDL_FLIP_NONE) {
			bowTexture.render(playerX+10, playerY, renderer, NULL, 0, NULL, flipType);
		} else {
			bowTexture.render(playerX-15, playerY, renderer, NULL, 0, NULL, flipType);
		}
		framesSinceLastArrowShot++;
		if(framesSinceLastArrowShot >= 10) {
			renderBow = false;
			framesSinceLastArrowShot = 0;
		}
	}
	for(std::size_t i = 0; i<arrows.size(); i++) {
		if(!arrows[i]->isActive()) {
			delete arrows[i];
			arrows.erase(arrows.begin() + i);
			arrowCount--;
		} else {
			arrows[i]->render(renderer, arrowTexture);
		}
	}
}

void Arrows::collisionDetectionEnemies(std::vector<IEnemy*> enemies) {
	for (auto &enemy : enemies) {
		collisionDetection(enemy);
	}
}


void Arrows::collisionDetection(IEnemy* enemy) {
	for(std::size_t i = 0; i<arrows.size(); i++) {
		auto hit = arrows[i]->collisionDetection(enemy);
		if(hit) {
			delete arrows[i];
			arrows.erase(arrows.begin() + i);
			arrowCount--;
		}
	}
}
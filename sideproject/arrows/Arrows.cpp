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

}

bool Arrows::loadTexture(SDL_Renderer *renderer)
{
	// Loading success flag
	bool success = true;

	arrowTexture.loadFromFile("assets/images/arrow.png", renderer);
	arrowTexture.scaleToWidth(35);
	return success;
}

void Arrows::shootArrow(int x, int y, SDL_RendererFlip flipType) {
	Arrow* arrow = new Arrow(flipType);
	arrow->shoot(x, y);
	arrows.push_back(arrow);
}


void Arrows::render(SDL_Renderer *renderer) {
	for(auto& arrow : arrows) {
		arrow->render(renderer, arrowTexture);
	}
}

void Arrows::collisionDetectionEnemies(std::vector<Enemy*> enemies) {
	for (auto &enemy : enemies) {
		collisionDetection(enemy);
	}
}


void Arrows::collisionDetection(Enemy* enemy) {
	for(std::size_t i = 0; i<arrows.size(); i++) {
		auto hit = arrows[i]->collisionDetection(enemy);
		if(hit) {
			delete arrows[i];
			arrows.erase(arrows.begin() + i);
		}
	}
}
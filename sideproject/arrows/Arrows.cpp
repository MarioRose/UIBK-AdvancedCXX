#include <Arrows.h>

#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>

Arrows::Arrows() {

}


Arrows::~Arrows() {
	for(auto& arrow : arrows) {
		arrow->free();
	}
}

void Arrows::shootArrow(int x, int y, SDL_RendererFlip flipType, SDL_Renderer* renderer) {
	Arrow* arrow = new Arrow(renderer, flipType);
	arrow->shoot(x, y);
	arrows.push_back(arrow);
}


void Arrows::render(SDL_Renderer *renderer) {
	for(auto& arrow : arrows) {
		arrow->render(renderer);
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
			arrows.erase(arrows.begin() + i);
		}
	}
}
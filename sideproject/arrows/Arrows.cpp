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

void Arrows::shootArrow(int x, int y, SDL_Renderer* renderer) {
	Arrow* arrow = new Arrow(renderer);
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
	for(auto& arrow : arrows) {
		arrow->collisionDetection(enemy);
	}
}
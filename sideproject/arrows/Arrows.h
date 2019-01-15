#ifndef SIDESCROLLER_ARROWS_H
#define SIDESCROLLER_ARROWS_H

#include <Arrow.h>
#include <vector>

class Arrows {
public:
	Arrows();

	~Arrows();

	// Shows the Character on the screen
	void render(SDL_Renderer *renderer);

	void collisionDetectionEnemies(std::vector<IEnemy*> enemies);
	void collisionDetection(IEnemy* enemy);

	void shootArrow(int x, int y, SDL_RendererFlip flipType);

	bool loadTexture(SDL_Renderer *renderer);

private:
	std::vector<Arrow*> arrows;

	// Scene textures
	Texture arrowTexture;

};

#endif //SIDESCROLLER_ARROWS_H
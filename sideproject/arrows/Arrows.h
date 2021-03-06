#ifndef SIDESCROLLER_ARROWS_H
#define SIDESCROLLER_ARROWS_H

#include <Arrow.h>
#include <vector>

class Arrows {
public:
	Arrows();

	~Arrows();

	// Shows the Character on the screen
	void render(SDL_Renderer *renderer, int playerX, int playerY, SDL_RendererFlip flipType);

	void collisionDetectionEnemies(std::vector<IEnemy*> enemies);
	void collisionDetection(IEnemy* enemy);

	void shootArrow(int playerX, int playerY, SDL_RendererFlip flipType, int maxArrow, int speed, int strength);

	bool loadTexture(SDL_Renderer *renderer);

private:
	bool renderBow = false;
	int framesSinceLastArrowShot = 0;
	int arrowCount = 0;
	std::vector<Arrow*> arrows;

	// Scene textures
	Texture arrowTexture;
	Texture bowTexture;

};

#endif //SIDESCROLLER_ARROWS_H
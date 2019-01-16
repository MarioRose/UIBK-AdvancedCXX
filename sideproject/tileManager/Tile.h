#ifndef SIDESCROLLER_TILE_H
#define SIDESCROLLER_TILE_H

#include <SDL_render.h>
#include <Settings.h>

class Tile {

  public:
	Tile(int xCord, int yCord, int tileT) : x(xCord), y(yCord), tileType(tileT) {}

	~Tile();

	void free();

	int getX();
	int getY();

	int getTileType();

	void render(SDL_Renderer *renderer, SDL_Texture *texture);

	static SDL_Rect croppedtiles[3];
	static void initCroppedTiles();

	// Constants:
	static const int TILE_HEIGHT = 16;
	static const int TILE_WEIGHT = 16;

	static const int TILE_GROUND = 0;
	static const int TILE_WALL = 1;
	static const int TILE_PLATFORM = 2;

  private:
	int x, y;
	int tileType;
};

#endif // SIDESCROLLER_TILE_H

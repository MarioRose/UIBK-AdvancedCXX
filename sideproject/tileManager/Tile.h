#ifndef SIDESCROLLER_TILE_H
#define SIDESCROLLER_TILE_H

#include <SDL_render.h>
#include <Settings.h>

class Tile {

  public:
	Tile(int xCord, int yCord, int tileT, int level) : x(xCord), y(yCord), tileType(tileT), level(level) {}

	~Tile();

	void free();

	int getX();
	int getY();

	int getTileType();

	void render(SDL_Renderer *renderer, SDL_Texture *texture);

	static SDL_Rect croppedtiles[6];
	static void initCroppedTiles();

	// Constants:
	static const int TILE_HEIGHT = 16;
	static const int TILE_WIDTH = 16;

	static const int TILE_GROUND = 0;
	static const int TILE_WALL = 1;
	static const int TILE_PLATFORM = 2;

    static const int TILE_JUNGLE = 0;
    static const int TILE_MOUNTAIN = 1;
    static const int TILE_HELL = 2;


private:
	int x, y;
	int tileType;
	int level;
};

#endif // SIDESCROLLER_TILE_H

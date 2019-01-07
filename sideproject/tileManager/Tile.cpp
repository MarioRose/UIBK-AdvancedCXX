#include "Tile.h"

SDL_Rect Tile::croppedtiles[3] = {0};

Tile::~Tile(){
    free();
}

void Tile::free(){

}

void Tile::initCroppedTiles() {
    croppedtiles[TILE_GROUND].x = TILE_WEIGHT * 0;
    croppedtiles[TILE_GROUND].y = TILE_HEIGHT * 13;
    croppedtiles[TILE_GROUND].w = TILE_WEIGHT;
    croppedtiles[TILE_GROUND].h = TILE_WEIGHT;

    croppedtiles[TILE_PLATFORM].x = TILE_WEIGHT * 10;
    croppedtiles[TILE_PLATFORM].y = TILE_HEIGHT * 3;
    croppedtiles[TILE_PLATFORM].w = TILE_WEIGHT;
    croppedtiles[TILE_PLATFORM].h = TILE_WEIGHT;

    croppedtiles[TILE_WALL].x = TILE_WEIGHT * 0;
    croppedtiles[TILE_WALL].y = TILE_HEIGHT * 1;
    croppedtiles[TILE_WALL].w = TILE_WEIGHT;
    croppedtiles[TILE_WALL].h = TILE_WEIGHT;
}

void Tile::render(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_Rect renderQuad = { x, y, TILE_WEIGHT, TILE_HEIGHT };
    SDL_RenderCopyEx( renderer, texture, &Tile::croppedtiles[tileType], &renderQuad, 0, NULL, SDL_FLIP_NONE );
}

int Tile::getX() {
    return x;
}

int Tile::getY() {
    return y;
}

int Tile::getTileType() {
    return tileType;
}

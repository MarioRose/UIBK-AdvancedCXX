#include "Tile.h"

SDL_Rect Tile::croppedtiles[6] = {0};

Tile::~Tile(){
    free();
}

void Tile::free(){

}

void Tile::initCroppedTiles() {
    croppedtiles[TILE_JUNGLE + TILE_GROUND].x = TILE_WIDTH * 0;
    croppedtiles[TILE_JUNGLE + TILE_GROUND].y = TILE_HEIGHT * 13;
    croppedtiles[TILE_JUNGLE + TILE_GROUND].w = TILE_WIDTH;
    croppedtiles[TILE_JUNGLE + TILE_GROUND].h = TILE_HEIGHT;

    croppedtiles[TILE_JUNGLE + TILE_PLATFORM].x = TILE_WIDTH * 10;
    croppedtiles[TILE_JUNGLE + TILE_PLATFORM].y = TILE_HEIGHT * 3;
    croppedtiles[TILE_JUNGLE + TILE_PLATFORM].w = TILE_WIDTH;
    croppedtiles[TILE_JUNGLE + TILE_PLATFORM].h = TILE_HEIGHT;

    croppedtiles[TILE_JUNGLE + TILE_WALL].x = TILE_WIDTH * 0;
    croppedtiles[TILE_JUNGLE + TILE_WALL].y = TILE_HEIGHT * 1;
    croppedtiles[TILE_JUNGLE + TILE_WALL].w = TILE_WIDTH;
    croppedtiles[TILE_JUNGLE + TILE_WALL].h = TILE_HEIGHT;

    croppedtiles[(TILE_MOUNTAIN*3) + TILE_GROUND].x = 0;
    croppedtiles[(TILE_MOUNTAIN*3) + TILE_GROUND].y = TILE_HEIGHT * 5;
    croppedtiles[(TILE_MOUNTAIN*3) + TILE_GROUND].w = TILE_WIDTH;
    croppedtiles[(TILE_MOUNTAIN*3) + TILE_GROUND].h = TILE_HEIGHT;

    croppedtiles[(TILE_MOUNTAIN*3) + TILE_PLATFORM].x = 0;
    croppedtiles[(TILE_MOUNTAIN*3) + TILE_PLATFORM].y = TILE_HEIGHT * 4;
    croppedtiles[(TILE_MOUNTAIN*3) + TILE_PLATFORM].w = TILE_WIDTH;
    croppedtiles[(TILE_MOUNTAIN*3) + TILE_PLATFORM].h = TILE_HEIGHT;

    croppedtiles[(TILE_MOUNTAIN*3) + TILE_WALL].x = 0;
    croppedtiles[(TILE_MOUNTAIN*3) + TILE_WALL].y = 0;
    croppedtiles[(TILE_MOUNTAIN*3) + TILE_WALL].w = TILE_WIDTH;
    croppedtiles[(TILE_MOUNTAIN*3) + TILE_WALL].h = TILE_HEIGHT;

    croppedtiles[(TILE_HELL*3) + TILE_GROUND].x = TILE_WIDTH * 4;
    croppedtiles[(TILE_HELL*3) + TILE_GROUND].y = TILE_HEIGHT * 5;
    croppedtiles[(TILE_HELL*3) + TILE_GROUND].w = TILE_WIDTH;
    croppedtiles[(TILE_HELL*3) + TILE_GROUND].h = TILE_HEIGHT;

    croppedtiles[(TILE_HELL*3) + TILE_PLATFORM].x = TILE_WIDTH * 4;
    croppedtiles[(TILE_HELL*3) + TILE_PLATFORM].y = TILE_HEIGHT * 4;
    croppedtiles[(TILE_HELL*3) + TILE_PLATFORM].w = TILE_WIDTH;
    croppedtiles[(TILE_HELL*3) + TILE_PLATFORM].h = TILE_HEIGHT;

    croppedtiles[(TILE_HELL*3) + TILE_WALL].x = 0;
    croppedtiles[(TILE_HELL*3) + TILE_WALL].y = 0;
    croppedtiles[(TILE_HELL*3) + TILE_WALL].w = TILE_WIDTH;
    croppedtiles[(TILE_HELL*3) + TILE_WALL].h = TILE_HEIGHT;
}

void Tile::render(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_Rect renderQuad = { x, y, TILE_WIDTH, TILE_HEIGHT };
    SDL_RenderCopyEx( renderer, texture, &Tile::croppedtiles[(level * 3) + tileType], &renderQuad, 0, NULL, SDL_FLIP_NONE );
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

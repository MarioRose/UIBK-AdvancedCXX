#include "sprite.h"
#include <string.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include "Texture.h"

Sprite::Sprite(){

}

Sprite::Sprite(int x, int y, std::string path, SDL_Renderer *renderer) : GameObject(x,y,10,10), renderer(renderer) {
    cTexture.loadFromFile(path, renderer);
    cTexture.scaleToHeight(SCREEN_HEIGHT * 0.05);
}

Sprite::~Sprite(){
    this->free();
}

#include "GameObject.h"

GameObject::GameObject(int x, int y, int height, int width): posX(x),
                                                 posY(y),
                                                 object_height(height),
                                                 object_width(width) {};

GameObject::GameObject() {}

GameObject::~GameObject() {}


void GameObject::render(SDL_Renderer* renderer) {
    //Show the Object
    cTexture.render( posX, posY, renderer );
}

#include "GameObject.h"
#include "Settings.h"

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

void GameObject::setPosX(double x) {
    this->posX = x;
}

void GameObject::setPosY(double y) {
    this->posY = y;
}

double GameObject::getPosX() {
    return posX;
}

double GameObject::getPosY() {
    return posY;
}

void GameObject::setHeight(int px)
{
    this->object_height = px;
}

void GameObject::setWidth(int px)
{
    this->object_width = px;
}

int GameObject::getHeight()
{
    return this->object_height;
}

int GameObject::getWidth()
{
    return this->object_width;
}

#ifndef SIDESCROLLER_GAMEOBJECT_H
#define SIDESCROLLER_GAMEOBJECT_H


#include <SDL_render.h>
#include <vector>
#include "Texture.h"

enum class Direction {
    LEFT,
    RIGHT
};

class GameObject {

public:
    GameObject();
    GameObject(int x, int y, int height, int width);
    ~GameObject();

    //Shows the Object on the screen
    void render(SDL_Renderer* gRenderer);

    void free();

    //Scene textures
    Texture cTexture;

    void setPosX(double x);
    void setPosY(double y);

    double getPosX();
    double getPosY();


    void setHeight(int px);
    void setWidth(int px);

    int getHeight();
    int getWidth();

	Direction direction = Direction::RIGHT;

protected:
    //The X and Y offsets of the Object
    double posX;
    //The X and Y offsets of the Object
    double posY;
    int object_width = 19;
    //The dimensions of the Object
    int object_height = 34;


};


#endif //SIDESCROLLER_GAMEOBJECT_H

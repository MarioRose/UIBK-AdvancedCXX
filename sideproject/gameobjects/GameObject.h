#ifndef SIDESCROLLER_GAMEOBJECT_H
#define SIDESCROLLER_GAMEOBJECT_H


#include <SDL_render.h>
#include <../texture/Texture.h>
#include <vector>

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

    void setPosY(double y);
    void setPosX(double x);

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

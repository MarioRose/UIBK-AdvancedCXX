#ifndef SIDESCROLLER_MOVEABLE_H
#define SIDESCROLLER_MOVEABLE_H

#include "../gameobjects/GameObject.h"

class Moveable: public GameObject {

public:
    Moveable();
    Moveable(int x, int y, int height, int width);

    void move();

protected:
    //The velocity of the moveable Object
    double velX;
//The velocity of the moveable Object
double velY;
//Max velocity of moveable Object
const double max_vel = 5;
//The velocity of the moveable Object
double forceY;
};


#endif //SIDESCROLLER_MOVEABLE_H

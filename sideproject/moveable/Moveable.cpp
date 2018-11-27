#include "Moveable.h"
#include "../settings/Settings.h"


Moveable::Moveable(int x, int y, int height, int width) : GameObject(x, y, height, width) {}

Moveable::Moveable() {}

void Moveable::move() {
    //Move the character left or right
    posX += velX;

    //If the character went too far to the left or right
    if( ( posX < 0 ) || ( posX + object_width > SCREEN_WIDTH ) ) {
        //Move back
        posX -= velX;
    }

    //Jumping of Falling
    if(forceY != 0 || posY != SCREEN_HEIGHT - object_height) {

        //Calculate Force
        if( posY + object_height < SCREEN_HEIGHT ) {
            forceY = forceY + GRAVITATION + ((SCREEN_HEIGHT - object_height) - posY)/4.5;
        }

        //Calculate acceleration
        float a = forceY + velY;

        //Calculate velocity
        velY = velY + a;

        //Move the character up or down
        posY += velY;


        //Maximum Velocity downwards
        if(velY >= MAX_VELOCITY_DOWN) {
            velY = MAX_VELOCITY_DOWN;
        }

        //If the character went too far up or down
        if( ( posY < 0 ) || ( posY + object_height >= SCREEN_HEIGHT ) ) {
            //Move back
            //mPosY -= mVelY;
            velY = 0;
            posY = SCREEN_HEIGHT - object_height;
        }

        forceY = 0;
    }
}

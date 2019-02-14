//
// Created by christoph on 10.02.19.
//

#include "Projectile_vert.h"
#include <Settings.h>

Projectile_vert::Projectile_vert(std::string path, SDL_Renderer *renderer)
{
    status = CharacterStatus::DEAD;
    loadFromFile(path, renderer);
    posY = SCREEN_HEIGHT;
    posX = SCREEN_WIDTH;
}

void Projectile_vert::moveAI(Character *character)
{
    if (forceX > 0) {
        posY -= 5;
        if(shoot_direction){
            posX += 5;
        } else {
            posX -= 5;
        }
        forceX -= 2;
    } else {
        outOfBounds = true;
        status = CharacterStatus::DEAD;
    }
}

void Projectile_vert::setStartPosition(int x, int y, int force, bool direction)
{
    this->posX = x;
    this->posY = y;

    forceY = 0;
    forceX = force;

    outOfBounds = false;
    status = CharacterStatus::RUNNING;
    shoot_direction = direction;
}

void Projectile_vert::reset()
{
    outOfBounds = true;
    status = CharacterStatus::DEAD;
    forceX = 0;
}
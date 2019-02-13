//
// Created by christoph on 08.02.19.
//

#include "Projectile.h"
#include <Settings.h>

Projectile::Projectile(std::string path, SDL_Renderer *renderer)
{
	status = CharacterStatus::DEAD;
    loadFromFile(path, renderer);
    posY = SCREEN_HEIGHT;
    posX = SCREEN_WIDTH;
}

void Projectile::moveAI(Character *character)
{
	if (outOfBounds) {
		return;
	}

    if(contactPlatform){
        outOfBounds = true;
        status = CharacterStatus::DEAD;
        return;
    }

    if(shootLeft){
        if (forceY > 0) {
            forceY--;
            posY -= 2;
            posX -= forceX;
        }

        if (forceY == 0) {
            posY += 2;
            posX -= forceX;
        }
    } else {
        if (forceY > 0) {
            forceY--;
            posY -= 2;
            posX += forceX;
        }

        if (forceY == 0) {
            posY += 2;
            posX += forceX;
        }
    }

}

void Projectile::setStartPosition(int x, int y, int force)
{
	this->posX = x;
	this->posY = y;

	forceY = 35;
	forceX = force;

	outOfBounds = false;
	status = CharacterStatus::RUNNING;
}

void Projectile::setStartPosition(int x, int y, int force, bool shootLeft)
{
    this->posX = x;
    this->posY = y;

    forceY = 35;
    forceX = force;

    outOfBounds = false;
    status = CharacterStatus::RUNNING;
    this->shootLeft = shootLeft;
}
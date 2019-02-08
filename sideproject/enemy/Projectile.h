//
// Created by christoph on 08.02.19.
//

#ifndef SIDESCROLLER_PROJECTILE_H
#define SIDESCROLLER_PROJECTILE_H


#include "IEnemy.h"

class Projectile : public IEnemy{
public:
    // Initializes the variables
    Projectile(std::string path, SDL_Renderer *renderer);

    ~Projectile();

    void moveAI(Character *character) override;
    void setStartPosition(int x, int y, int force);

private:
    bool outOfBounds = true;

};


#endif //SIDESCROLLER_PROJECTILE_H

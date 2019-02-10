//
// Created by christoph on 10.02.19.
//

#ifndef SIDESCROLLER_PROJECTILE_VERT_H
#define SIDESCROLLER_PROJECTILE_VERT_H

#include "IEnemy.h"

class Projectile_vert : public IEnemy{
public:
    // Initializes the variables
    Projectile_vert(std::string path, SDL_Renderer *renderer);

    ~Projectile_vert();

    void moveAI(Character *character) override;
    void setStartPosition(int x, int y, int force, bool direction);

private:
    bool outOfBounds = true;
    bool shoot_direction;

};


#endif //SIDESCROLLER_PROJECTILE_VERT_H

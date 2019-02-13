#ifndef SIDESCROLLER_SECONDBOSS_H
#define SIDESCROLLER_SECONDBOSS_H

#include <memory>
#include "../sprite/Sprite.h"
#include "IEnemy.h"
#include "Projectile.h"
#include "Boss.h"


class SecondBoss : public IEnemy {
public:
    // Initializes the variables
    SecondBoss();

    ~SecondBoss();

    void moveAI(Character *character) override;

    bool isGoingUp();
    void setIsGoingUp(bool up);
    void setGoRight(bool right);
    void setItem(Sprite *sprite);
    void setProjectiles(std::shared_ptr<std::vector<Projectile*>> projectiles);
    void shootProjectiles();
    bool isGoingRight();

private:
    void attackOne();
    void attackTwo();

    Sprite *item = nullptr;
    AttackPhase currentPhase = AttackPhase::NONE;
    std::shared_ptr<std::vector<Projectile *>> projectiles;

    bool goingUp;
    bool goRight;
    bool itemNotShowed = true;
    void goBackAndForth();
    int frameCount = 0;
    bool returnToMiddle = false;
    bool isDead = false;
};


#endif //SIDESCROLLER_SECONDBOSS_H

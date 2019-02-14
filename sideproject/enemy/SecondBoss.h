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

    void setStartPos();
    void reset() override;

private:
    void attackOne();
    void attackTwo();

    Sprite *item = nullptr;
    AttackPhase currentPhase = AttackPhase::NONE;
    std::shared_ptr<std::vector<Projectile *>> projectiles;

    int startX;
    int startY;
    int startHealth;

    bool goingUp;
    bool goRight;
    bool itemNotShowed = true;
    void goBackAndForth();
    int frameCount = 0;
    bool returnToMiddle = false;
};


#endif //SIDESCROLLER_SECONDBOSS_H

#include "Boss.h"
#include "Settings.h"

Boss::Boss()
{
	flipType = SDL_FLIP_HORIZONTAL;
	clip = new SDL_Rect{0, 0, 60, 108};
}

bool Boss::isGoingRight()
{
	return goingRight;
}

void Boss::setIsGoingRight(bool right)
{
	goingRight = right;
}

void Boss::setItem(Sprite *sprite)
{
	this->item = sprite;
}

void Boss::goBackAndForth()
{
	status = CharacterStatus::RUNNING;

	if (isGoingRight()) {
		if (posX >= SCREEN_WIDTH - 30) {
			setIsGoingRight(false);
		} else {
			posX += 3;
		}
	} else {
		if (posX < SCREEN_WIDTH - 200) {
			setIsGoingRight(true);
		} else {
			posX -= 3;
		}
	}
}

void Boss::moveAI(Character *character)
{
    if(status == CharacterStatus::DEAD){
        if(!itemNotShowed)
            return;
        item->showSprite(this->posX, this->posY, itemNotShowed);
        itemNotShowed = false;
        return;
    }

	if (status == CharacterStatus::DYING || status == CharacterStatus::ATTACK) {
		return;
	}

	goBackAndForth();
}

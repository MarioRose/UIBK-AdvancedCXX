#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Enemy.h"
#include "Sprite.h"

enum class EquippedItem { NONE, BOW };

enum class EquippedAbility { NONE, JUMP };

struct SavePoint {
	int x = 0;
	int y = 0;
	int roomIndex = 0;
};

// The player that will move around on the screen
class Player : public Character {
  public:
	// Initializes the variables
	Player();

	~Player();

	// Takes key presses and adjusts the Character's velocity
	void control(SDL_Event &e);
    // Lets the Character move
    void move() override;

	bool collisionDetectionEnemies(std::vector<IEnemy *> enemies);
	bool collisionDetectionSprites(std::vector<Sprite *> sprites);

	void takeDamage();

	int getPoints();
	void setPoints(int points);

	bool onLeftBorder();
	bool onRightBorder();
	bool onTopBorder();
	bool onBottomBorder();
	bool inSkyHole() const;

	bool getHasBow() const;
	void setHasBow(bool hasBow);

    bool hasDoubleJump() const;
    void setDoubleJumpItem(bool hasItem);

	EquippedItem getEquippedItem() const;
    EquippedAbility getEquippedAbility() const;

	void setLastSavePoint(int x, int y, int roomIndex);
	SavePoint lastSavePoint;

  private:
	bool hasBow = false;

	bool hasDoubleJumpItem = true;

	int keypressCount = 0;
	int points = 0;

	EquippedItem equippedItem = EquippedItem::NONE;;
    EquippedAbility equippedAbility = EquippedAbility::JUMP;

	bool collisionDetection(IEnemy *enemy);
	bool collisionDetection(Sprite *sprite);

	void jump();
};

#endif

#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Enemy.h"
#include "Sprite.h"

enum class EquippedItem { NONE, BOW, SILVERBOW, GOLDENBOW };

enum class EquippedAbility { NONE, JUMP, FIRE };

struct SavePoint {
	int x = 20;
	int y = 400;
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
	bool collisionDetectionSprites(std::vector<Sprite *> sprites, bool& flameCollision);

	void takeDamage();

	int getPoints();
	void setPoints(int points);

	bool onLeftBorder();
	bool onRightBorder();
	bool onTopBorder();
	bool onBottomBorder();

	bool getHasBow() const;
	void setHasBow(bool hasBow);

	bool getHasSilverBow() const;
	void setHasSilverBow(bool hasBow);

	bool getHasGoldenBow() const;
	void setHasGoldenBow(bool hasBow);

	bool hasDoubleJump() const;
	void setDoubleJumpItem(bool hasItem);

	bool hasFireItem() const;
	void setFireItem(bool hasItem);

	void increaseMaxHealth();

    void setFallingDown(bool);
    bool getFallingDown() const;

	EquippedItem getEquippedItem() const;
	void setEquippedItem(EquippedItem);
	EquippedAbility getEquippedAbility() const;
	void setEquippedAbility(EquippedAbility);

	void setLastSavePoint(int x, int y, int roomIndex);
	SavePoint lastSavePoint;

	void resetPlayer();
	void setKeypressCount(int count);
	void setStatus(CharacterStatus);

  private:
    bool fallingDown = false;

	bool hasBow = false;
	bool hasSilverBow = false;
	bool hasGoldenBow = false;

	bool hasDoubleJumpItem = true;
	bool hasFireImmunity = true;

	int keypressCount = 0;
	int points = 0;
	int maxHealth = 5;

	EquippedItem equippedItem = EquippedItem::NONE;
	EquippedAbility equippedAbility = EquippedAbility::JUMP;

	bool collisionDetection(IEnemy *enemy);
	bool collisionDetection(Sprite *sprite, bool& flameCollision);

	void jump();
};

#endif

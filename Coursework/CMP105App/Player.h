#pragma once

#include "Framework/GameObject.h"
#include "Framework/Animation.h"
#include<iostream>
#include "Framework/AudioManager.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	void update(float dt) override;
	void handleInput(float dt) override;
	void collisionResponse(GameObject* collider);
	bool getMovingRight();
	bool getMovingLeft();
	void setIsFalling(bool l_isFalling);
	void setIsOnGround(bool l_isOnGround);

protected:
	Animation attack;
	Animation idle;
	Animation jump;
	Animation run;
	Animation walk;
	Animation death;

private:
	const int attackFrames = 10;
	const int idleFrames = 10;
	const int jumpFrames = 10;
	const int runFrames = 10;
	const int walkFrames = 10;
	const int deathFrames = 7;

	// Although the sprites are sized 587 x 707, ive reduced the frame size.
	const int FRAME_WIDTH = 589;
	const int FRAME_HEIGHT = 680;

	float gravityScalar;
	sf::Vector2f gravitationalAccel;
	sf::Vector2f stepVelocity;
	float xDiffPlayerColBox;
	float YDiffPlayerColBox;
	AudioManager audioMan;


	bool movingLeft;
	bool movingRight;
	bool isJumping;
	bool isFalling;
	bool onGround;
	bool isAttacking;

	void addFrames();
	void gravityFall(float dt);
	void setAllAnimsFalse();
	void checkMovingRight(float dt);
	void checkMovingLeft(float dt);
	void checkRunning(float dt);
	void checkJumping(float dt);
	void checkAttacking(float dt);
	//void checkGround();
	void updateCollisionBox();
	void checkTileCollisions(GameObject* col);
	void initAudio();

};


#pragma once

#include <iostream>
#include "Framework/GameObject.h"
#include "Framework/Animation.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"

static bool sfxMuted = false;

class Player : public GameObject
{
public:
	Player();
	~Player();

	void update(float dt) override;
	void handleInput(float dt) override;

	void collisionResponse(GameObject* collider);
	void checkLiftCollisions(GameObject* col);


	// Getters / Setters
	bool getMovingRight();
	bool getMovingLeft();
	bool getIdling();
	void setIsFalling(bool l_isFalling);
	void setIsOnGround(bool l_isOnGround);
	bool getIsOnGround();
	bool getIsDead();
	void setIsDead(bool l_isDead);
	void setRespawned(bool l_respawned);
	int getLives();
	void setLives(int l_lives);
	void setHitPoints(int l_hitPoints);
	void respawn();

	// Might not need this but dont delete it yet! it could be handy for the player to be able to access gamestate.
	/*void passAndSetCurrentSateFromScreen(GameState* currGameState);
	State getCurrentGameState();*/

	static void setSFXMuteAudio(bool l_muted);


protected:
	Animation attack;
	Animation idle;
	Animation jump;
	Animation run;
	Animation walk;
	Animation death;

private:
	// DATA MEMBERS.
	const int attackFrames = 10;
	const int idleFrames = 10;
	const int jumpFrames = 10;
	const int runFrames = 10;
	const int walkFrames = 10;
	const int deathFrames = 7;

	// Although the sprites are sized 587 x 707, ive reduced the frame size.
	const int FRAME_WIDTH = 589;
	const int FRAME_HEIGHT = 680;

	int lives;
	int hitPoints;

	float gravityScalar;
	sf::Vector2f gravitationalAccel;
	sf::Vector2f stepVelocity;
	float xDiffPlayerColBox;
	float YDiffPlayerColBox;
	AudioManager audioMan;

	float attackDelay;
	float deathAnimDelay;
	float hitPointReductionDelay;

	State currentState;
	GameState* currentGameState;

	bool movingLeft;
	bool movingRight;
	bool idling;
	bool isJumping;
	bool isFalling;
	bool onGround;
	bool isAttacking;
	bool isDead;
	bool respawned;
	bool flashRed;
	float flashRedDelay;

	// MEMBER FUNCTIONS.
	void resetPlayerValues();
	void addFrames();
	void gravityFall(float dt);
	void setAllAnimsFalse();
	void checkMovingRight(float dt);
	void checkMovingLeft(float dt);
	void checkRunning(float dt);
	void checkJumping(float dt);
	void checkAttacking(float dt);
	void updateCollisionBox();
	void checkTileCollisions(GameObject* col);
	void initAudio();
	void spikeCollision();
};


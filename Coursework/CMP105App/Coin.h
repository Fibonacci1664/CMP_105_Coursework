#pragma once
#include"Framework/GameObject.h"
#include"Framework/Animation.h"

class Coin : public GameObject
{
public:
	Coin();
	~Coin();

	void update(float dt) override;

	Animation* getCoinAnimation();

private:
	// Everthing to do with exit door animations.
	Animation coin;

	const int COIN_FRAME_WIDTH = 32;
	const int COIN_FRAME_HEIGHT = 32;
	const int coin_frames = 10;

	void addCoinAnimFrames();
};


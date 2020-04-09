#pragma once
#include"Framework/GameObject.h"
#include"Framework/Animation.h"

class Key : public GameObject
{
public:
	Key();
	~Key();

	void update(float dt) override;

	Animation* getKeyAnimation();

private:
	// Everthing to do with lamp animations.
	Animation key;

	const int KEY_FRAME_WIDTH = 64;
	const int KEY_FRAME_HEIGHT = 64;
	const int key_frames = 7;

	void addKeyAnimFrames();
};


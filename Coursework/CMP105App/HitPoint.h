#pragma once
#include"Framework/GameObject.h"
#include"Framework/Animation.h"

class HitPoint : public GameObject
{
public:
	HitPoint();
	~HitPoint();

	void update(float dt) override;

	Animation* getHitPointAnimation();

private:
	// Everthing to do with exit door animations.
	Animation hitPoint;

	const int KEY_FRAME_WIDTH = 32;
	const int KEY_FRAME_HEIGHT = 32;
	const int key_frames = 10;

	void addHitPointAnimFrames();
};


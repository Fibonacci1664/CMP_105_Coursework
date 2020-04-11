#pragma once
#include"Framework/GameObject.h"
#include"Framework/Animation.h"

class ExitDoor : public GameObject
{
public:
	ExitDoor();
	~ExitDoor();

	void update(float dt) override;

	Animation* getExitDoorAnimation();

private:
	// Everthing to do with exit door animations.
	Animation exitDoor;

	const int KEY_FRAME_WIDTH = 128;
	const int KEY_FRAME_HEIGHT = 128;
	const int key_frames = 5;

	void addExitDoorAnimFrames();
};


#pragma once
#include"Framework/GameObject.h"
#include"Framework/Animation.h"

class FireLamp : public GameObject
{
public:
	FireLamp();
	~FireLamp();

	void update(float dt) override;

	Animation* getLamp_1();
	Animation* getLamp_2();
	Animation* getLamp_3();
	Animation* getLamp_4();

private:
	// Everthing to do with lamp animations.
	Animation lamp_1;
	Animation lamp_2;
	Animation lamp_3;
	Animation lamp_4;
	const int LAMPS_FRAME_WIDTH = 64;
	const int LAMPS_FRAME_HEIGHT = 64;
	const int lamp_1_frames = 4;
	const int lamp_2_frames = 4;
	const int lamp_3_frames = 4;
	const int lamp_4_frames = 4;

	void addLampAnimFrames();
};


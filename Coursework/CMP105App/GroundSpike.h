#pragma once
#include"Framework/GameObject.h"
#include"Framework/Animation.h"

class GroundSpike : public GameObject
{
public:
	GroundSpike();
	~GroundSpike();

	void update(float dt) override;

	Animation* getGroundSpikeAnimation();

private:
	// Everthing to do with exit door animations.
	Animation groundSpike;

	const int GROUND_SPIKE_FRAME_WIDTH = 128;
	const int GROUND_SPIKE_FRAME_HEIGHT = 128;
	const int ground_spike_frames = 5;

	void addGroundSpikeAnimFrames();
};


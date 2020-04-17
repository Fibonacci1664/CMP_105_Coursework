/*
 * This class controls
 *		- Setting up the ground spike trap animation.
 *		- Returning the animation.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2020.
 */

 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES.
#pragma once
#include"Framework/GameObject.h"
#include"Framework/Animation.h"

 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GroundSpike : public GameObject
{
public:
	GroundSpike();
	~GroundSpike();

	void update(float dt) override;

	Animation* getGroundSpikeAnimation();

private:
	// Everthing to do with ground spike trap animations.
	Animation groundSpike;

	const int GROUND_SPIKE_FRAME_WIDTH = 128;
	const int GROUND_SPIKE_FRAME_HEIGHT = 128;
	const int ground_spike_frames = 5;

	void addGroundSpikeAnimFrames();
};
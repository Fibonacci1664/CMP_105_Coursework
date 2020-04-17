/*
 * This class controls
 *		- Setting up the heart animation.
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

class Heart : public GameObject
{
public:
	Heart();
	~Heart();

	void update(float dt) override;

	Animation* getHeartAnimation();

private:
	// Everthing to do with heart animations.
	Animation heart;

	const int HEART_FRAME_WIDTH = 32;
	const int HEART_FRAME_HEIGHT = 32;
	const int heart_frames = 10;

	void addHeartAnimFrames();
};
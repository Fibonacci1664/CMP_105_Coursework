/*
 * This class controls
 *		- Setting up the hit point animation.
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

	const int HITPOINT_FRAME_WIDTH = 32;
	const int HITPOINT_FRAME_HEIGHT = 32;
	const int hitPoint_frames = 10;

	void addHitPointAnimFrames();
};
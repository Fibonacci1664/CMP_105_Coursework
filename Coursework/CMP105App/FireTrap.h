/*
 * This class controls
 *		- Setting up the fire trap animation.
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

class FireTrap : public GameObject
{
public:
	FireTrap();
	~FireTrap();

	void update(float dt) override;

	Animation* getFireTrapAnimation();

private:
	// Everthing to do with fire trap animations.
	Animation fireTrap;

	const int FIRE_TRAP_FRAME_WIDTH = 128;
	const int FIRE_TRAP_FRAME_HEIGHT = 128;
	const int fire_trap_frames = 10;

	void addFireTrapAnimFrames();
};
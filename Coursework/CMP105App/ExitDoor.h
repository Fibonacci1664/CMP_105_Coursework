/*
 * This class controls
 *		- Setting up the exit door animation.
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

	const int EXITDOOR_FRAME_WIDTH = 128;
	const int EXITDOOR_FRAME_HEIGHT = 128;
	const int exitDoor_frames = 5;

	void addExitDoorAnimFrames();
};
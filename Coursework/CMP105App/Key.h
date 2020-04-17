/*
 * This class controls
 *		- Setting up the key animation.
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

class Key : public GameObject
{
public:
	Key();
	~Key();

	void update(float dt) override;

	Animation* getKeyAnimation();

private:
	// Everthing to do with key animations.
	Animation key;

	const int KEY_FRAME_WIDTH = 64;
	const int KEY_FRAME_HEIGHT = 64;
	const int key_frames = 7;

	void addKeyAnimFrames();
};
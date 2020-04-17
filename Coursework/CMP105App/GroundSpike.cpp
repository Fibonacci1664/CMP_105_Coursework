/*
 * This class controls
 *		- Setting up the ground spike trap animation.
 *		- Returning the animation.
 *
 * Original @author D. Green.
 *
 * � D. Green. 2020.
 */

 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GroundSpike.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTORS & DESTRUCTOR.
GroundSpike::GroundSpike()
{
	addGroundSpikeAnimFrames();
}

GroundSpike::~GroundSpike()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.
void GroundSpike::update(float dt)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GroundSpike::addGroundSpikeAnimFrames()
{
	for (int i = 0; i < (ground_spike_frames * GROUND_SPIKE_FRAME_WIDTH); i += GROUND_SPIKE_FRAME_WIDTH)
	{
		groundSpike.addFrame(sf::IntRect(i, 0, GROUND_SPIKE_FRAME_WIDTH, GROUND_SPIKE_FRAME_HEIGHT));
	}

	// Set how fast you want each of your animations to play, divide by a higher number for a faster animation.
	groundSpike.setFrameSpeed(1.0f / 5.0f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Animation* GroundSpike::getGroundSpikeAnimation()
{
	return &groundSpike;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
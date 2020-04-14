#include "FireTrap.h"

FireTrap::FireTrap()
{
	addFireTrapAnimFrames();
}

FireTrap::~FireTrap()
{

}

void FireTrap::update(float dt)
{

}

void FireTrap::addFireTrapAnimFrames()
{
	for (int i = 0; i < (fire_trap_frames * FIRE_TRAP_FRAME_WIDTH); i += FIRE_TRAP_FRAME_WIDTH)
	{
		fireTrap.addFrame(sf::IntRect(i, 0, FIRE_TRAP_FRAME_WIDTH, FIRE_TRAP_FRAME_HEIGHT));
	}

	// Set how fast you want each of your animations to play, divide by a higher number for a faster animation.
	fireTrap.setFrameSpeed(1.0f / 5.0f);
}

Animation* FireTrap::getFireTrapAnimation()
{
	return &fireTrap;
}
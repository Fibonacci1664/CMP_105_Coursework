#include "ExitDoor.h"

ExitDoor::ExitDoor()
{
	addExitDoorAnimFrames();
}

ExitDoor::~ExitDoor()
{

}

void ExitDoor::update(float dt)
{

}

void ExitDoor::addExitDoorAnimFrames()
{
	for (int i = 0; i < (exitDoor_frames * EXITDOOR_FRAME_WIDTH); i += EXITDOOR_FRAME_WIDTH)
	{
		exitDoor.addFrame(sf::IntRect(i, 0, EXITDOOR_FRAME_WIDTH, EXITDOOR_FRAME_HEIGHT));
	}

	// Set how fast you want each of your animations to play, divide by a higher number for a faster animation.
	exitDoor.setFrameSpeed(1.0f / 5.0f);
}

Animation* ExitDoor::getExitDoorAnimation()
{
	return &exitDoor;
}
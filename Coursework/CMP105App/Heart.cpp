#include "Heart.h"

Heart::Heart()
{
	addHeartAnimFrames();
}

Heart::~Heart()
{

}

void Heart::update(float dt)
{

}

void Heart::addHeartAnimFrames()
{
	for (int i = 0; i < (heart_frames * HEART_FRAME_WIDTH); i += HEART_FRAME_WIDTH)
	{
		heart.addFrame(sf::IntRect(i, 0, HEART_FRAME_WIDTH, HEART_FRAME_HEIGHT));
	}

	// Set how fast you want each of your animations to play, divide by a higher number for a faster animation.
	heart.setFrameSpeed(1.0f / 10.0f);
}

Animation* Heart::getHeartAnimation()
{
	return &heart;
}
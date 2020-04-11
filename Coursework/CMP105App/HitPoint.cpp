#include "HitPoint.h"

HitPoint::HitPoint()
{
	addHitPointAnimFrames();
}

HitPoint::~HitPoint()
{

}

void HitPoint::update(float dt)
{

}

void HitPoint::addHitPointAnimFrames()
{
	for (int i = 0; i < (hitPoint_frames * HITPOINT_FRAME_WIDTH); i += HITPOINT_FRAME_WIDTH)
	{
		hitPoint.addFrame(sf::IntRect(i, 0, HITPOINT_FRAME_WIDTH, HITPOINT_FRAME_HEIGHT));
	}

	// Set how fast you want each of your animations to play, divide by a higher number for a faster animation.
	hitPoint.setFrameSpeed(1.0f / 10.0f);
}

Animation* HitPoint::getHitPointAnimation()
{
	return &hitPoint;
}
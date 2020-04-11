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
	for (int i = 0; i < (key_frames * KEY_FRAME_WIDTH); i += KEY_FRAME_WIDTH)
	{
		hitPoint.addFrame(sf::IntRect(i, 0, KEY_FRAME_WIDTH, KEY_FRAME_HEIGHT));
	}

	// Set how fast you want each of your animations to play, divide by a higher number for a faster animation.
	hitPoint.setFrameSpeed(1.0f / 10.0f);
}

Animation* HitPoint::getHitPointAnimation()
{
	return &hitPoint;
}
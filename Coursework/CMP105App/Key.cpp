#include "Key.h"

Key::Key()
{
	addKeyAnimFrames();
}

Key::~Key()
{

}

void Key::update(float dt)
{

}

void Key::addKeyAnimFrames()
{
	for (int i = 0; i < (key_frames * KEY_FRAME_WIDTH); i += KEY_FRAME_WIDTH)
	{
		key.addFrame(sf::IntRect(i, 0, KEY_FRAME_WIDTH, KEY_FRAME_HEIGHT));
	}

	// Set how fast you want each of your animations to play, divide by a higher number for a faster animation.
	key.setFrameSpeed(1.0f / 10.0f);
}

Animation* Key::getKeyAnimation()
{
	return &key;
}
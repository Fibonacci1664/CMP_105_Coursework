#include "Coin.h"

Coin::Coin()
{
	addCoinAnimFrames();
}

Coin::~Coin()
{

}

void Coin::update(float dt)
{

}

void Coin::addCoinAnimFrames()
{
	for (int i = 0; i < (key_frames * KEY_FRAME_WIDTH); i += KEY_FRAME_WIDTH)
	{
		coin.addFrame(sf::IntRect(i, 0, KEY_FRAME_WIDTH, KEY_FRAME_HEIGHT));
	}

	// Set how fast you want each of your animations to play, divide by a higher number for a faster animation.
	coin.setFrameSpeed(1.0f / 15.0f);
}

Animation* Coin::getCoinAnimation()
{
	return &coin;
}
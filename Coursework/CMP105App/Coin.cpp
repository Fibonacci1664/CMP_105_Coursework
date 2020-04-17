/*
 * This class controls
 *		- Setting up the coin animation.
 *		- Returning the animation.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2020.
 */

 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES.
#include "Coin.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTORS & DESTRUCTOR
Coin::Coin()
{
	addCoinAnimFrames();
}

Coin::~Coin()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.
void Coin::update(float dt)
{

}

void Coin::addCoinAnimFrames()
{
	for (int i = 0; i < (coin_frames * COIN_FRAME_WIDTH); i += COIN_FRAME_WIDTH)
	{
		coin.addFrame(sf::IntRect(i, 0, COIN_FRAME_WIDTH, COIN_FRAME_HEIGHT));
	}

	// Set how fast you want each of your animations to play, divide by a higher number for a faster animation.
	coin.setFrameSpeed(1.0f / 15.0f);
}

Animation* Coin::getCoinAnimation()
{
	return &coin;
}
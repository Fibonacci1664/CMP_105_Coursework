/*
 * This class controls all aspects of animation including:
 *		- Controls a single animation.
 *		- Stores a list of frames.
 *		- Frame rate
 *		- Looping and lipped states.
 *
 * Returns current frame of animation based on time and updating.
 *
 * Original @author Dr Paul Robertson.
 * Updated by @author D. Green.
 *
 * © D. Green. 2020.
 */

 //////////////////////////////////////////////////////////////////////////////////////////////////////////////

 // INCLUDES.
#include "Animation.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTORS & DESTRUCTOR.
Animation::Animation()
{
	currentFrame = 0;
	elapsedTime = 0.f;
	isPlaying = true;
	isLooping = true;
	animationSpeed = 0.0f;
	flipped = false;
}

Animation::~Animation()
{
	std::cout << "Animation object destroyed!\n";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS.

// Adds animation frame. Rect object represent a single sprite frame
void Animation::addFrame(sf::IntRect rect)
{
	frames.push_back(rect);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Check if the animation should progress, if so move to next frame, or loop back to the start
void Animation::animate(float dt)
{
	if (isPlaying)
	{
		elapsedTime += dt;

		if (elapsedTime >= animationSpeed)
		{
			currentFrame++;

			if (currentFrame >= (int)frames.size())
			{
				if (isLooping)
				{
					currentFrame = 0;
				}
				else
				{
					currentFrame--;
					setPlaying(false);
				}
			}

			elapsedTime = 0;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Reset animation.
void Animation::reset()
{
	currentFrame = 0;
	elapsedTime = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Animation::stop()
{
	setPlaying(false);
	currentFrame = 0;
	elapsedTime = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// GETTERS & SETTERS.

// Returns the size of the animation. The number of frames.
int Animation::getSize()
{
	return (int)frames.size();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Get current frame from the animation. Flip the returning frame if required.
sf::IntRect Animation::getCurrentFrame()
{
	frame = frames[currentFrame];

	if (flipped)
	{
		frame = sf::IntRect(frame.left + frame.width, frame.top, -frame.width, frame.height);
	}

	return frame;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Animation::getCurrentFrameNumber()
{
	return currentFrame;
}

// Set animation speed, in Frames per Second.
void Animation::setFrameSpeed(float speed)
{
	animationSpeed = speed;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
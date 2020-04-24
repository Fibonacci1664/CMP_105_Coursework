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
	playForward = true;
	playBackwards = false;
}

Animation::~Animation()
{
	//std::cout << "Animation object destroyed!\n";
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
		// Increment the delay.
		elapsedTime += dt;

		// If the delay gets passed the threshold and the anim is playing forwards.
		if ((elapsedTime >= animationSpeed) && playForward)
		{
			// Move to the next frame.
			++currentFrame;

			// If we get to the end.
			if (currentFrame >= (int)frames.size())
			{
				// If we're looping go back to the first frame.
				if (isLooping)
				{
					currentFrame = 0;
				}
				else	// Otherwise set up for playing the animation backwards.
				{
					--currentFrame;
					playBackwards = true;
					playForward = false;
				}
			}

			// Reset the delay.
			elapsedTime = 0;
		}

		// Same as above but in reverse.
		if ((elapsedTime >= animationSpeed) && playBackwards)
		{
			--currentFrame;

			if (currentFrame < 0)
			{
				++currentFrame;
				playForward = true;
				playBackwards = false;
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
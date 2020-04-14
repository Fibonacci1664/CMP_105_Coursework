#pragma once

#include "SFML\Graphics.hpp"
#include <vector>
#include<iostream>

class Animation
{
public:
	Animation();
	~Animation();

	// Create animation.
	void addFrame(sf::IntRect rect);
	void setFrameSpeed(float speed);
	int getSize();

	// Update/reset animation.
	void animate(float dt);
	void reset();

	// Get current frame of animation.
	sf::IntRect getCurrentFrame();
	int getCurrentFrameNumber();

	// Animation controls.
	float getFrameSpeed() { return animationSpeed; };
	void setFlipped(bool flip) { flipped = flip; };
	bool getFlipped() {	return flipped;	};
	void setLooping(bool loop) { isLooping = loop; };
	bool getLooping() { return isLooping; };
	void setPlaying(bool play) { isPlaying = play; };
	bool getPlaying() { return isPlaying; };
	void play(bool play) { isPlaying = true; };
	void stop();
	void pause() { isPlaying = false; };

protected:
	std::vector<sf::IntRect>frames;
	sf::IntRect frame;
	int currentFrame;
	float animationSpeed;
	bool flipped;
	bool isLooping;
	bool isPlaying;
	bool playForward;
	bool playBackwards;
	float elapsedTime;
};
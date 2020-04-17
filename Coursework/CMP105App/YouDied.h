/*
 * This class controls
 *		- Creating an image for showing when the player dies.
 *		- Fading in/out.
 *		- Playing audio for death.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2020.
 */

 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES.
#pragma once
#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include <string>
#include <iostream>
#include "Screen.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class YouDied : public Screen
{
public:
	YouDied(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~YouDied();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:
	// Init stuff.
	void initYouDiedRect();
	void initTransFadeRect();
	void initAudio();

	// Misc.
	void fadeIn();
	void fadeOut();

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	sf::RectangleShape youDiedRect;
	sf::RectangleShape transFade;
	sf::Texture youDiedTexture;

	bool fadedIn;
	bool fadedOut;
	bool switchedStates;

	int hasDiedCount;	
};
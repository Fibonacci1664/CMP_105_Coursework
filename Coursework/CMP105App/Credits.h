/*
 * This class controls
 *		- Creating a bg image for the credits screen.
 *		- Rolling credits.
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

class Credits : public Screen
{
public:
	Credits(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~Credits();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:
	// Init stuff.
	void initAudio();
	void initCreditsBg();

	// Misc.
	void rollCredits(float& dt);

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	sf::RectangleShape creditsBg;
	sf::Texture creditsBgTexture;

	float bottomOfCreditsPos;
};
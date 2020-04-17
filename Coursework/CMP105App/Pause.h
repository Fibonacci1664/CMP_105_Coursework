/*
 * This class controls
 *		- Creating an image for the pause menu.
 *		- Checking button collisions.
 *		- Changing textures when collisions detected.
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
#include "Framework/GameObject.h"
#include <string>
#include <iostream>
#include "Screen.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Pause : public Screen
{
public:
	Pause(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~Pause();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:
	// Init stuff.
	void initAudio();
	void initPauseText();
	void initScroll();
	void initMainMenuButton();
	void initContinueButton();
	void initQuitButton();
	void initTransLayer();

	// Check stuff.
	void checkMainMenuButtonCollisions();
	void checkContinueCollisions();
	void checkQuitButtonCollisions();

	// Update stuff.
	void updatePauseMenuPosition();

	// Misc.

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	sf::Vector2f mousePos;
	bool quitClicked;
	bool mainMenuClicked;
	bool continueClicked;
	bool pressedLetterP;
	bool unpaused;
	bool resetView;

	sf::RectangleShape pausedTextBox;
	sf::Texture pausedTexture;

	sf::RectangleShape scrollBox;
	sf::Texture scrollTexture;

	GameObject mainMenuButton;
	sf::Texture mainMenuButtonTexture;
	sf::Texture mainMenuButtonHoverTexture;
	sf::Texture mainMenuButtonClickedTexture;

	GameObject continueButton;
	sf::Texture continueButtonTexture;
	sf::Texture continueButtonHoverTexture;
	sf::Texture continueButtonClickedTexture;

	GameObject quitButton;
	sf::Texture quitButtonTexture;
	sf::Texture quitButtonHoverTexture;
	sf::Texture quitButtonClickedTexture;

	sf::RectangleShape transLayer;

	sf::View originalViewPos;
	float xTranslation;
};
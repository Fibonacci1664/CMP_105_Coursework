/*
 * This class controls
 *		- Creating a bg image for the Options screen.
 *		- Checking button collisions.
 *		- Checking for check box collisions.
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
#include <string>
#include <iostream>
#include "Screen.h"

class Options : public Screen
{
public:
	Options(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~Options();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:
	// Init stuff.
	void initAudio();
	void initOptionsBg();
	void initBackButton();
	void initMuteMusicCheckBox();
	void initMuteSFXCheckBox();
	void initGodModeCheckBox();
	void initCheckBoxTextures();

	// Check stuff.
	void checkBackButtonCollisions();
	void checkMuteMusicBoxCollisions();
	void checkMuteSFXBoxCollisions();
	void checkGodModeBoxCollisions();

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	sf::Vector2f mousePos;

	sf::RectangleShape optionsBg;
	sf::Texture optionsBgTexture;

	sf::RectangleShape backButton;
	sf::Texture backButtonTexture;
	sf::Texture backButtonHoverTexture;
	sf::Texture backButtonClickedTexture;

	sf::RectangleShape muteMusicCheckBox;
	sf::RectangleShape muteSFXCheckBox;
	sf::RectangleShape godModeCheckBox;

	sf::Texture blankCheckBoxTexture;
	sf::Texture crossedCheckBoxTexture;

	bool musicChecked;
	bool sfxChecked;
	bool godModeChecked;
	bool backButtonClicked;
};
/*
 * This class controls
 *		- Creating a bg image for the How To Play screen.
 *		- Checking button collisions.
 *		- Changing textures when button collisions detected.
 *
 * Original @author D. Green.
 *
 * ? D. Green. 2020.
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

class HowToPlay : public Screen
{
public:
	HowToPlay(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~HowToPlay();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:
	void initAudio();
	void initHowToPlayBg();
	void initBackButton();
	void checkBackButtonCollisions();

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	bool backButtonClicked;

	sf::Vector2f mousePos;

	sf::RectangleShape howToPlayBg;
	sf::Texture howToPlayBgTexture;

	sf::RectangleShape backButton;
	sf::Texture backButtonTexture;
	sf::Texture backButtonHoverTexture;
	sf::Texture backButtonClickedTexture;	
};
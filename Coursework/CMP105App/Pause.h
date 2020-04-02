#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include <string>
#include <iostream>
#include "Screen.h"

class Pause : public Screen
{
public:
	Pause(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~Pause();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:
	sf::Vector2f mousePos;
	bool quitClicked;
	bool mainMenuClicked;
	bool continueClicked;
	bool pressedLetterP;

	sf::RectangleShape pausedTextBox;
	sf::Texture pausedTexture;

	sf::RectangleShape scrollBox;
	sf::Texture scrollTexture;

	sf::RectangleShape mainMenuButton;
	sf::Texture mainMenuButtonTexture;
	sf::Texture mainMenuButtonHoverTexture;
	sf::Texture mainMenuButtonClickedTexture;

	sf::RectangleShape continueButton;
	sf::Texture continueButtonTexture;
	sf::Texture continueButtonHoverTexture;
	sf::Texture continueButtonClickedTexture;

	sf::RectangleShape quitButton;
	sf::Texture quitButtonTexture;
	sf::Texture quitButtonHoverTexture;
	sf::Texture quitButtonClickedTexture;

	sf::RectangleShape transLayer;

	void initAudio();
	void initPauseText();
	void initScroll();
	void initMainMenuButton();
	void initContinueButton();
	void initQuitButton();
	void initTransLayer();
	void checkMainMenuButtonCollisions();
	void checkContinueCollisions();
	void checkQuitButtonCollisions();



	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();
};
/*
 * This class controls
 *		- Creating a bg image for the Game Over screen.
 *		- Fading in and out of the Game Over image.
 *		- Playing audio on game over.
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

class GameOver : public Screen
{
public:
	GameOver(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~GameOver();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;
	bool getGameOver();
	void setGameOver(bool l_gameOver);

private:
	// Init stuff.
	void initGameOverRect();
	void initTransFadeRect();
	void initAudio();

	// Misc.
	void fadeIn();
	void fadeOut();

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	sf::RectangleShape gameOverRect;
	sf::RectangleShape transFade;
	sf::Texture gameOverTexture;

	bool fadedIn;
	bool fadedOut;
	bool switchedStates;
	bool gameOver;	
};
#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include <string>
#include <iostream>
#include "Screen.h"

class GameOver : public Screen
{
public:
	GameOver(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~GameOver();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:
	sf::RectangleShape gameOverRect;
	sf::RectangleShape transFade;
	sf::Texture gameOverTexture;

	bool fadedIn;
	bool fadedOut;
	bool switchedStates;

	void initGameOverRect();
	void initTransFadeRect();
	void initAudio();
	void fadeIn();
	void fadeOut();

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();
};